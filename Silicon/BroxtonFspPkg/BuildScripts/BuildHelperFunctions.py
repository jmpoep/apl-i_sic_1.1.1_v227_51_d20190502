##
#
# @package BuildHelperFunctions
# Contains the helper functions for the BuildIfwi script.
# BuildHelperFunctions.py is a python module that holds independent helper functions to help
#  organize functionality and increase readability and manageability of BuildIFWI.py. These
#  tools are made extensible by the variables that are passed in, so that their internal code
#  rarely needs to be modified. The idea here is that unnecessary details are encapsulated from
#  anyone using, editing, or updating the EDK Build process.
#
# Copyright (c) 2015-2016, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
#
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN 'AS IS' BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
#

import sys
import os
from os import path
import platform
import subprocess
import ConfigParser
import re
import multiprocessing
import shutil
import operator
import zipfile
import CommonHelperFunctions
from CommonHelperFunctions import OsPath
import logging
logger = logging.getLogger(__name__)
import subprocess

##
#
#     This function will call a batch file (WINDOWS ONLY) and update
#     any enviroment changes made by the batch file.
#
#     @param CallStr A previously assembled string that is composed like the Batch file
#                     was being called from command prompt.
#     @return None
#
def CallBatchAndUpdateEnvironment(CallStr):

    BatchOutputDelimiter = 'END_BATCH_OUTPUT'
    # Run the batch file with set and collect the output
    CallStr += ' && echo ' + BatchOutputDelimiter + ' && set'
    ChildOutput = subprocess.check_output(CallStr)

    BatchOutput, SetOutput = ChildOutput.split(BatchOutputDelimiter)
    SetOutput = SetOutput.lstrip()      # Get rid of leading newline character

    # Go through the output and get all environment variables and the values
    # that they have, and update the current environment with the new values.
    for line in SetOutput.splitlines():
        EnvironVar, Equals, Value = map(str.strip, line.strip().partition('='))
        if EnvironVar.startswith('?'):
            continue
        os.environ[EnvironVar] = Value


##
#
#     Inherieted Exception class that can be raised, and will logger out a passed in error message
#     and then exit with traceback errors.
#
class BuildFailureError(Exception):
    def __init__(self, ErrorMessage):
        self.ErrorMessage = ErrorMessage + \
                            '\n -- Error:  EDKII BIOS Build has failed!' +  '\n'
        logger.critical(self.ErrorMessage)
    def __str__(self):
        return self.ErrorMessage


##
#
#     RECURSIVE FUNCTION ALERT
#     Recurse case: Macro value is other macros.
#     Base case: Macro value is a literal hexidecimal value.
#     Will search for a hexidecimal (0x...) macro value in a string of text. If the value is non-literal,
#     It will recursively search the text for the composing Macros. This will continue until it has a literal value.
#     It supports multiple operators, so if a function finds MACRO_NAME = ANOTHER_MACRO + MACRO_TASTIC - MACRO_FUL
#     It can find the correct value. Does not support PEMDOS (left right execution).
#
#     @param TextToSearch The text to be searched
#     @param MacroName The macro to search for.
#     @return The value of MacroName as an integer.
#
def RecursiveSearchForMacroValue(TextToSearch, MacroName):
    # Get the Macro value from the text
    # If not found, group() will raise an exception
    SearchPattern = r'(' + MacroName + '\s*=\s).*'      # Pattern is MACRO_NAME = (STRING OF CHARACTERS UNTIL NEWLINE)
    MacroMatch = re.search(SearchPattern, TextToSearch)
    MacroValueStr = MacroMatch.group(0)                 # Will fail here if no macro name is found.
    Trash, Value = re.split(r'=\s', MacroValueStr)      # Put the (STRING OF CHARACTERS UNTIL NEWLINE) into Value
    MacroValue = 0

    try:
        # Check if it is a hex number (literal value)
        MacroValue = int(Value, 16)
    except ValueError:
        # If the macro value is not a literal value, extract the macros
        # that compose it and recursively search for their values.
        ops = {'+': operator.add, '-': operator.sub}                                # LUT for operator support
        logger.debug(MacroName + ' is not a literal value. '
                                 'Looking up composing macros')                     # The operators are every single...
        ListOfOperators = re.findall(r'(?<=\s).(?=\s)', Value)                      #... character surrounded by spaces.
        ListOfOperators.insert(0, '+')                                              # Push an add for first operator
        ListOfMacros = re.findall(r'(?<=\()\w+', Value)                             # Get the macro values
        for EachMacro, EachOperator in zip(ListOfMacros, ListOfOperators):          # In the case 3 + 5 - 1:
            TempValue = RecursiveSearchForMacroValue(TextToSearch, EachMacro)       # 1st: temp + 3 2nd: temp + 5 ...
            MacroValue = ops[EachOperator](MacroValue, TempValue)                   # ... 3rd: temp - 1

    logger.debug(MacroName + ' macro value is ' + str(MacroValue))
    return MacroValue


##
#
#     Given a list of macro names, will find them in a file and then return a summation of their values.
#     Can support left to right addition and subraction in the macro definition.
#     Utilizes the recursive function RescursiveSearchForMacroValue.
#
#     @param FilePath The path to the file to create the macro
#     @param ListOfMacroNames A list of strings, each string being a
#     @return The sum of all the values for each Macro in Macronames
#
def GetMacroValuesFromFile(FilePath, ListOfMacroNames):
    assert isinstance(ListOfMacroNames, list)

    logger.debug('Fetching and summing together the values for the following Macros: ')
    logger.debug(', '.join(ListOfMacroNames))
    logger.debug('From the file: ' + FilePath)

    # Get File Text
    FileHandle = open(FilePath, 'r')
    FileText = FileHandle.read()
    FileHandle.close()

    # Get Macro Value
    MacroValue = 0
    for EachMacro in ListOfMacroNames:
        logger.debug('Finding ' + EachMacro + ' macro value in: ' + FilePath)
        MacroValue += RecursiveSearchForMacroValue(FileText, EachMacro)

    return MacroValue

##
#
#     Will take a chunk of binary data from a source file and put it into
#     a destination file.
#
#     @param Source The binary file to be read from
#     @param Start The byte to start reading from
#     @param NumBytes Number of bytes to read
#     @param Destination the binary file the data is put into
#     @return
#
def ChunkBinary(Source, Start, NumBytes, Destination):
    # Normalize path variables
    Source = OsPath(Source)
    Destination = OsPath(Destination)

    # Read the binary data
    SourceBin = open(Source, 'rb')
    SourceBin.seek(Start)
    Data = SourceBin.read(NumBytes)
    SourceBin.close()

    # Write the binary data
    DestinationBin = open(Destination, 'wb')
    DestinationBin.write(Data)
    DestinationBin.close()


##
#
#     Organizational function (runs once) that will setup the visual studio compiler
#     that will be used for the build. Will run vsvars32.bat for the respective compiler
#     and update the kernel environment.
#     @param Compiler The compiler version, formatted as 'VS20XX'
#     @return Tool Chain Tag string of the compiler being used.
#
def SetupVisualStudio(Compiler):
    VsVersions = {
                  'VS2013': 'VS120COMNTOOLS',
                  'VS2012': 'VS110COMNTOOLS',
                  'VS2010': 'VS100COMNTOOLS',
                  'VS2008': 'VS90COMNTOOLS',
                  'VS2005': 'VS80COMNTOOLS',
                  'VS2003': 'VS71COMNTOOLS'
                 }

    VsComnTools = os.getenv(VsVersions[Compiler], False)

    # Call the respective vsvars32.bat
    if VsComnTools:
        CallStr = VsComnTools + 'vsvars32.bat'
        CallBatchAndUpdateEnvironment(CallStr)
    else:
        raise BuildFailureError('!!! WARNING !!! Cannot find Visual Studio')

    # If this is a 64 bit system, we need to append x86 to specify so
    if platform.machine().endswith('64'):
        Compiler += 'x86'

    logger.debug('Setting compiler to ' + Compiler)
    return Compiler


##
#
#     Detect currect GCC version and map to corresponding toolchain.
#
#     @return GCC toolchain version
#
def SetupGccVersion():
    cmd = subprocess.Popen(["gcc -dumpversion"], stdout=subprocess.PIPE, shell=True)
    (output, err) = cmd.communicate()
    version = output.split('.')[0]+'.'+output.split('.')[1]
    if version == '4.5':
      toolchain = 'GCC45'
    elif version == '4.6':
      toolchain = 'GCC46'
    elif version == '4.7':
      toolchain = 'GCC47'
    elif version == '4.8':
      toolchain = 'GCC48'
    elif version == '4.9':
      toolchain = 'GCC49'
    else:
      logger.info('GCC version : ' + str(version) + ' is unsupported!')
      sys.exit(1)

    return toolchain
#
#     For a given target script (currently either BuildIfwi or IfwiStitch,
#     build the argument strings
#
#     @param ConfigOptions An already populated ConfigParser object from a properly formatted
#                           IFWI Configuration file (see IfwiConfiguration.txt)
#     @param Args An already populated ParseArgs object that was generated from ConfigOptions
#     @param TargetScript The target script (Currently either BuildIfwi or IfwiStitch)
#     @return The properly formatted call string of argument flags.
#
def AssembleArgumentFlags(ConfigOptions, Args, TargetScript):
    assert isinstance(ConfigOptions, ConfigParser.SafeConfigParser)

    TargetFlags = ''

    DictOfArgs = vars(Args)

    # For each option that is not a section header and exists in the CLI arguments.
    for Option in ConfigOptions.sections():
        if ConfigOptions.has_option(Option, 'SectionHeader') or not Option in DictOfArgs:
            continue

        # If the option is set in CLI args and it is targeting the right script
        if DictOfArgs[Option] and TargetScript in ConfigOptions.get(Option, 'Target'):
            OptionFlag = ConfigOptions.get(Option, 'Flag')
            # Add it to the argument flag string
            if not OptionFlag == 'Positional':
                TargetFlags += ConfigOptions.get(Option, 'Flag') + ' '
            OptionValue = DictOfArgs[Option]
            if isinstance(OptionValue, str):
                TargetFlags += OptionValue + ' '

    return TargetFlags


# ---------------------------- Pre-Build Functions --------------------------------

##
#
#     Writes a list of Build Macros into a file with formatting.
#
#     @param BuildMacros A dictionary of macros and their values to write.
#     @param DscPath The path to the file to write them into.
#     @return None
#
def GenerateBuildMacros(BuildMacros, DscPath):

    BuildMacrosFile = open(OsPath(DscPath), 'w+')

    TempStr = ''

    # Make sure Build Macros is a dictionary
    assert isinstance(BuildMacros, dict)

    # This writes the build macros into the dsc in no particular order
    for Key, Value in BuildMacros.iteritems():
        TempStr += '{:<7}{:<25}'.format('DEFINE', Key) + "= " + Value + '\n'        # Pretty print

    BuildMacrosFile.write(TempStr)
    BuildMacrosFile.close()


##
#
#     Will verify that all strings in the Vfr files are referenced in the file that UqiListPath points to.
#     Note that UqiListPath must be relative to PlatformPackage.
#
#     @param PlatformPackage Absolute path to the Platform Package directory
#     @param UqiListPath Path to the UqiList .uni file relative to the Platform Package
#     @param UniToolExe Absolute path to UniTool.exe
#     @param UniToolArgs Arguments for UniTool.exe
#     @param UniToolExcludes List of directories seperated by spaces to exclude from UniTool.exe's search.
#     @return None
#
def VerifyHiiStrings(PlatformPackage, UqiListPath, UniToolExe, UniToolArgs = '-b -u -x', UniToolExcludes = ''):

    logger.info('Running UniTool...')

    # Normalize Directory Variables
    PlatformPackage = OsPath(PlatformPackage)
    UniToolExe = OsPath(UniToolExe)
    UqiListPath = OsPath(UqiListPath)

    UniToolCmd = UniToolExe + ' ' + \
                 UniToolArgs + ' ' + \
                 PlatformPackage + ' ' + \
                 UqiListPath + ' ' + \
                 UniToolExcludes

    if CommonHelperFunctions.CliLikeCall(UniToolCmd, 'Unitool.log') != 0:
        raise BuildFailureError(' -- ERROR Parsing uni. Please see ' + path.join(os.getcwd(), 'Unitool.log') +
                             ' for more info.')


##
#
#     Generate the reset vectors by using nasm.exe and FixupForRawSection.exe.
#     Please ensure both of these tools are in the directory that NasmToolFolder points to.
#     This will put the reset vectors into the file specified by RawOut relative to the /Vtf0 and /Vtf1
#     folders inside of the ResetVectPath directory.
#
#     @param ResetVectPath The absolute directory path to the ResetVector folder
#     @param RawOut Reset vectors will go into this file relative to the /Vtf0 and /Vtf1 folders
#                    inside of the specified ResetVectorPath directory
#     @param AsmIn Asm file that will be used to generate the reset vectors relative to ResetVectPath
#     @param NasmToolFolder The folder that contains nasm.exe and FixupForRawSection.exe
#     @param NasmExeFlags The flags used while executing nasm.exe
#     @return None
#
def GenerateResetVectors(ResetVectPath, RawOut, AsmIn, NasmToolFolder, NasmExeFlags):

    # Normalize directory paths
    ResetVectPath = OsPath(ResetVectPath)
    RawOut = OsPath(RawOut)
    AsmIn = OsPath(AsmIn)
    NasmPath = OsPath(NasmToolFolder + '/nasm.exe')
    FixupRawPath = OsPath(NasmToolFolder + '/FixupForRawSection.exe')

    # Assemble output and input flags
    NasmOutIn = ' -o ' + RawOut + ' ' + AsmIn

    # Generate call strings
    FixupRawCallStr = FixupRawPath + ' ' + RawOut
    NasmCallStr = NasmPath + ' ' + NasmExeFlags + NasmOutIn

    # Save the current directory (wherever it is)
    SavedWorkingDirectory = os.getcwd()

    # Change the working directory and generate the reset vectors using nasm and FixupRaw executables.
    os.chdir(OsPath(ResetVectPath + '/Vtf0'))
    CommonHelperFunctions.CliLikeCall(NasmCallStr)
    CommonHelperFunctions.CliLikeCall(FixupRawCallStr)

    os.chdir(OsPath(ResetVectPath + '/Vtf1'))
    CommonHelperFunctions.CliLikeCall(NasmCallStr)
    CommonHelperFunctions.CliLikeCall(FixupRawCallStr)
    os.chdir(SavedWorkingDirectory)


##
#
#     Extracts board information from the Master BiosId.env, and then saves modifications
#     in the Destination BiosId.env file (Such as the board ID and the build target).
#     Updates the file name for the final ROM that contains stitch-relevant build configuration options
#     with the Version Major and Version Minor strings found in the Master BiosId.env.
#
#     @param BoardId The ID for the board to be inserted into BiosId.env file
#     @param BuildTarget Release or Debug
#     @param RomName A string representing most of the build configuration options to be updated
#                     with the Version Major and the Version Minor.
#     @param BiosIdEnvMaster The absolute path to the master BiosId.env
#     @param BiosIdEnvDest The absolute path to the destination BiosId.env
#     @return RomName suffixed with the version major and version minor found in the master BiosId.env.
#
def SetupBiosId(BoardId, BuildTarget, RomName, BiosIdEnvMaster, BiosIdEnvDest):
    logger.info('Setting ' + BoardId + ' platform configuration and BIOS ID...')

    # Normalize directory variables
    BiosIdEnvMaster = OsPath(BiosIdEnvMaster)
    BiosIdEnvDest = OsPath(BiosIdEnvDest)

    # Ensure that the destination directory exists.
    CommonHelperFunctions.MakeDir(os.path.dirname(BiosIdEnvDest))

    # Stages copy of BiosId.env and modifies it
    # By deleting all the comments and changing the board id and build type
    shutil.copyfile(BiosIdEnvMaster, BiosIdEnvDest)
    BiosIdFile = open(BiosIdEnvDest, 'r+')

    BiosIdFileText = BiosIdFile.read()

    # Replaces default built type with ours
    BiosIdFileText = re.sub(r'(BUILD_TYPE\s*=\s*).*(\n|$)', '\g<1>' + BuildTarget[0].upper() + '\n', BiosIdFileText)

    # Replaces default board id with ours
    BiosIdFileText = re.sub(r'(BOARD_ID\s*=\s*).*(\n|$)', '\g<1>' + BoardId + '\n', BiosIdFileText)
    BiosIdFile.seek(0)
    BiosIdFile.write(BiosIdFileText)
    BiosIdFile.truncate()
    BiosIdFile.close()

    # Find and suffix the Version Major and Version Minor tokens.
    VersionMajorMatch = re.search(r'(?<=VERSION_MAJOR = )\w+', BiosIdFileText)
    RomName += VersionMajorMatch.group(0) + '_'
    VersionMinorMatch= re.search(r'(?<=VERSION_MINOR = )\w+', BiosIdFileText)
    RomName += VersionMinorMatch.group(0)

    return RomName


##
#
#     Generates the BiosId binary for a platform based off the settings in the file at BiosIdEnvPath.
#     Will always generate a 32 bit version in *BuildPath*/IA32/*BiosIdBinName* and if the architecture is 64 bit,
#     it will generate a 64 bit version in *BuildPath*/X64/*BiosIdBinName*.
#
#     @param BuildPath The path the EDK Build output directory.
#     @param Arch X64 or IA32
#     @param BiosIdEnvPath Absolute path to the Bios ID Settings folder (typically .../Conf/BiosId.bin)
#     @param GenBiosIdExe Absolute path to the GenBiosId.exe executable file.
#                          (Typically  .../PlatformTools/GenBiosId/GenBiosId.exe)
#     @param BiosIdBinName Optional argument. The name of the final Bios ID Binary file (default = 'BiosId.bin')
#     @return None
#
def GenerateBiosId(BuildPath, Arch, BiosIdEnvPath, GenBiosIdExe, BiosIdBinName='BiosId.bin'):

    # Normalize directory variables
    BuildPath = OsPath(BuildPath)
    BiosIdEnvPath = OsPath(BiosIdEnvPath)
    GenBiosIdExe = OsPath(GenBiosIdExe)

    logger.debug('Ensuring correct build directory is present for GenBiosId...')

    CommonHelperFunctions.MakeDir(OsPath(BuildPath + '/IA32'))

    # GenIdCmd is composed of the Exe Path
    #                         the path to the input BiosId.env
    #                         the path to the output file
    GenIdCmd = OsPath(GenBiosIdExe) + \
               ' -i ' + OsPath(BiosIdEnvPath) + \
               ' -o ' + BuildPath

    logger.info('Creating BiosId...')
    CommonHelperFunctions.CliLikeCall(GenIdCmd + OsPath('/IA32/' + BiosIdBinName))

    # If the target architecture is 64 bit, run again
    if Arch == 'X64':
        CommonHelperFunctions.MakeDir(OsPath(BuildPath + '/X64'))
        CommonHelperFunctions.CliLikeCall(GenIdCmd + OsPath('/X64/' + BiosIdBinName))


#######################################################################################################################
# Pre-Build Actions/Setup Stage 2
#######################################################################################################################
##
#
#     Preforms the setup required for a Code Coverage enabled build.
#     Assembles code covereage required build flags.
#     Requires BullsEye installed on the host system.
#
#     @param BuildFlags The current flags that will be passed to EDK Build.
#     @param CodeCoveragePath The absolute path to the Code Coverage Package
#     @param BullsEyePath The absolute path to the Bullseye Program Directory.
#     @return Updated BuildFlags
#
def CodeCoverageSetup(BuildFlags, Compiler, PlatformPkg, CodeCoveragePath, BullsEyePath):
    # Existence and os-friendliness happens during CurrentDirectory initializaiton.
    CodeCoverageFolder = CommonHelperFunctions.CurrentDirectory(CodeCoveragePath)
    BullsEyeFolder = CommonHelperFunctions.CurrentDirectory(BullsEyePath)
    PlatformPkg = CommonHelperFunctions.CurrentDirectory(PlatformPkg)

    # Touch the Code Coverage configuration files / Make new ones if none exist.
    open(CodeCoverageFolder.GenAbsPath('covc.cfg'), 'a').close()
    open(CodeCoverageFolder.GenAbsPath('BxtCov.ucda'), 'a').close()

    ItsToolChain = 'ITSCOVTOOLCHAIN'
    ItsVisualStudio = None
    # Set ITSCOVTOOLCHAIN enviromental variable with the desired version of Visual Studio
    if re.search('VS2012|VS2013', Compiler):
        # Code Coverage's library in folder VS2012 is supported to VS2012/VS2013.
        ItsVisualStudio = 'VS2012'
    elif re.search('VS2008', Compiler):
        # Code Coverage's library in folder VS2008 is ONLY supported to VS2008.
        ItsVisualStudio = 'VS2008'
    else:
        raise BuildFailureError('Compiler: ' + Compiler + ' is not compatible with Code Coverage enabled build.')

    logger.debug('###Setting ' + ItsToolChain + '=' + ItsVisualStudio + '###')
    os.environ[ItsToolChain] = ItsVisualStudio
    BuildFlags += ' -Z -E ' + PlatformPkg.GenAbsPath('CodeCov.ini') + ' -D COV_TOOLS=' + ItsVisualStudio

    return BuildFlags


##
#
#     Sets up the target.txt configuration files for
#     the EDK2 build executable. Should be ran after setup_vs_comp(our_build)
#     Edits target.txt
#
#     @param Arch Target Architecture for the build (IA32 or X64)
#     @param BuildTarget Release or Debug
#     @param Compiler Version of the compiler used to build (VS20xx or GCC)
#     @param PlatformPkgDscPath Path to the PlatformPkg.dsc
#     @param TargetTxtFile Path to the target.txt file.
#     @return None
#
def ModifyTargetConfFile(Arch, BuildTarget, Compiler, PlatformPkgDscPath, TargetTxtFile):

    logger.info('Modifying Conf file ' + TargetTxtFile + ' for this build...')

    # Normalize path variables
    PlatformPkgDscPath = OsPath(PlatformPkgDscPath)
    TargetTxtFile = OsPath(TargetTxtFile)

    TargetTxtFile = open(OsPath(TargetTxtFile), 'r+')

    # Get number of processors
    BuildThreads = multiprocessing.cpu_count() + 1

    FileText = TargetTxtFile.read()

    # Handle target architecture macro
    if Arch == 'IA32':
        FileText = re.sub(r'(TARGET_ARCH\s*=\s*).*(\n|$)', '\g<1>IA32', FileText)
    elif Arch == 'X64':
        FileText = re.sub(r'(TARGET_ARCH\s*=\s*).*(\n|$)', '\g<1>IA32 X64', FileText)
    else:
        raise BuildFailureError('Invalid Architecture: ' + Arch)

    # Insert platform specific details.
    FileText = re.sub(r'(TARGET\s*=\s*).*(\n|$)', '\g<1>' + BuildTarget, FileText)
    # THE LINE BELOW RELIES ON THE FUNCTION setup_vs_comp RUNNING FIRST
    FileText = re.sub(r'(TOOL_CHAIN_TAG\s*=\s*).*(\n|$)', '\g<1>' + Compiler, FileText)
    FileText = re.sub(r'(ACTIVE_PLATFORM\s*=\s*).*(\n|$)', '\g<1>' + PlatformPkgDscPath, FileText)
    FileText = re.sub(r'(MAX_CONCURRENT_THREAD_NUMBER\s*=\s).*(\n|$)', '\g<1>' +
                      str(BuildThreads), FileText)

    # Write to the file and then close it
    TargetTxtFile.seek(0)
    TargetTxtFile.write(FileText)
    TargetTxtFile.truncate()
    TargetTxtFile.close()



##
#
#     Sets up the tools_def.txt configuration file for
#     the EDK2 build executable. Should be ran after setup_vs_comp(our_build)
#     Edits  tools_def.txt.
#
#     @param Compiler Version of the compiler used to build (VS20xx or GCC)
#     @param ToolsDefTxtFile Path the tools_def.txt file.
#     @param IaslExePath Path to the iasl.exe function
#     @return None
#
def ModifyToolsDefConfFile(Compiler, ToolsDefTxtFile, IaslExePath):

    logger.info('Modifying configuration file ' + ToolsDefTxtFile + ' for this build...')

    # Normalize directory variables
    IaslExePath = OsPath(IaslExePath)
    ToolsDefTxtFile = OsPath(ToolsDefTxtFile)

    ToolsDefFile = open(OsPath(ToolsDefTxtFile), 'a')

    # Ensure that x86 is appended to the compiler string
    CompStr = Compiler
    if not CompStr.endswith('x86'):
        CompStr += 'x86'

    # If on Linux, prefix the ASL Path with wine.
    UseWine = 'wine' + ' '
    if sys.platform.startswith('win'):
        UseWine = ''

    # Compose the Asl_path
    AslPath = '*_' + Compiler + '_*_ASL_PATH = ' + UseWine + IaslExePath + os.linesep

    ToolsDefFile.write(AslPath)
    ToolsDefFile.close()


#######################################################################################################################
# Post-Build Steps
#######################################################################################################################

##
# Will make a Flash Map file named after the Rom filename that contains the contents of
# the AutoGen PCD values.
#
# @param AutoGenPath The absolute path to the AutoGen file containing the PCD values.
# @param FlashMapFile The filename of the final Flash Map file.
# @return None
def MakeFlashMap(AutoGenPath, FlashMapFilePath):

    # Normalize directory variables
    AutoGenPath = OsPath(AutoGenPath)

    if os.path.isfile(AutoGenPath):
        # Write each line that includes PCD_VALUE into the Map File
        with open(FlashMapFilePath, 'w') as MapFile:
          with open(AutoGenPath, 'r') as AutoGenFile:
            for Line in AutoGenFile.readlines():
              if '_PCD_VALUE_' in Line:
                MapFile.write(Line)
    else:
        logger.warning('Flash Map Generation Error: File does not exist at ' + AutoGenPath)



##
#
#     Use FCE to insert the HII setup variable store with defaults into the BIOS binary.
#
#     @param BuildPath The absolute path to the directory that contains the Build deliveriables.
#     @param Arch The architecture of the platform built.
#     @param LogFile The log file to use for FCE
#     @param SocPath The absolute path to the Soc binary data.
#     @param HiiPath The absolute path to the Hii setup variable store data. (created by FCE)
#     @param FcePath The absolute path to the FCE executable
#     @return The file name of the resulting BIOS binary
#
def SetupDefaultHiiData(BuildPath, Arch, LogFile, SocPath, HiiPath, FcePath):
    logger.info('Running FCE...')

    # Normalize directory variables
    FcePath = OsPath(FcePath)
    SocPath = OsPath(BuildPath + SocPath)
    HiiPath = OsPath(BuildPath + HiiPath)

    # Make sure FCE is in the PATH environment variable
    FceFolder = path.dirname(FcePath)
    os.environ['PATH'] += os.pathsep + FceFolder

    # Compose the call string and call it
    # Look for the setup variable section in the Soc BIOS binary
    # and output the section to HiiPath
    FceCallString = FcePath + ' read -i ' + SocPath + ' > ' + HiiPath
    CommonHelperFunctions.CliLikeCall(FceCallString, LogFile)

    # Compose the path to the .fd file
    # This is where the final  version with the defaults inject will reside
    ArchPath = OsPath(BuildPath + '/FV/Bxt' + Arch + '.fd')

    # Compose the call string and call it
    # Inject the setup defaults into the binary.
    FceFdCall = FcePath + ' mirror -i ' + SocPath + ' -o ' + ArchPath + ' Setup SetupDefault'
    CommonHelperFunctions.CliLikeCall(FceFdCall, LogFile)\

    logger.info('... Success')

    return ArchPath


##
#
#     Will copy a list of files to a Folder, and then zip that folder.
#
#     @param FolderPath Absolute path to the directory that will be zipped
#     @param ZipFileName The absolute path to the final zipped file. (to generate)
#     @param FilesToCopy A list of paths that point to the files to be copied.
#     @return None
#
def PackIntoZip(FolderPath, ZipFileName, FilesToCopy):
    # Normalize directory variables
    FolderPath = OsPath(FolderPath)

    ZipBinaries = zipfile.ZipFile(ZipFileName, 'w')
    ZipBinaries.debug = 3

    # Save the current working directory and the change it
    WorkSpace = os.getcwd()
    os.chdir(FolderPath)

    # Add the files to the zip file
    for fv in FilesToCopy:
        if path.isfile(fv):
            ZipBinaries.write(fv)
        else:
            logger.warning('Zipping Warning: Cannot find file ' + fv + ' in ' + FolderPath +
                            '\nSkipping, will not put into ' + ZipFileName)

    ZipBinaries.close()

    os.chdir(WorkSpace)


