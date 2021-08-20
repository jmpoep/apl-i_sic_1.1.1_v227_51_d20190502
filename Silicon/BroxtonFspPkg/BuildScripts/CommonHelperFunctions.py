##
# 
# @package CommonHelperFunctions
# Contains the OS Related functions used by both BuildIfwi and IfwiStitch, or other modules included
# in the build or stitch process. Also includes the common argument parsing functionality between the processes.
# 
# Copyright (c) 2015, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
# 
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN 'AS IS' BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
# 
# 

import os
import re
import errno
import sys
import subprocess
import argparse
import ConfigParser
import glob
import time
import shutil
import runpy
import logging
logger = logging.getLogger(__name__)

##
# 
#     Inherieted Exception class that can be raised, and will print out a passed in error message
#     and then exit with traceback errors.
#     
class InvalidSetupError(Exception):
    def __init__(self, ErrorMessage):
        self.ErrorMessage = ErrorMessage + \
                            '\n -- Error:   Incorrect Setup!' +  '\n'
        logger.critical(self.ErrorMessage)
    def __str__(self):
        return self.ErrorMessage


##
# 
#     Will take a path formatted string (e.g. 'a/file/path') that uses backslashes, forwardslashes, or both
#     and return a properly formatted string path that the current operating system can recognize.
#     !!! NOTE: The only reason this exists is observed inability of os.path.normpath to handle backslashes
#               on a Unbuntu machine. I can only think this a bug.
#     @param Path A path formatting string. e.g. 'a/file/path'
#     @return The operating system normalized version of the path.
#     
def OsPath(Path):
    Path = re.sub(r'\\', '/', Path)
    NewPath = os.path.normpath(Path)
    return NewPath


##
# 
#     Models a directory. Contains functions that help with directory related issues in a operating system
#     independent fashion.
#     
class CurrentDirectory:
    ##
    # 
    #         Will parse the directory path given so it is independent of OS formatting.
    #         Then will verify the directory exists, and raise an exception if not.
    #         @param Directory The absolute path to the directory.
    #         @return None
    #         
    def __init__(self, Directory):
        self.Path = re.sub(r'\\', '/', Directory)
        self.Path = os.path.normpath(self.Path)

        if not os.path.isdir(self.Path):
            raise InvalidSetupError(Directory + ' is NOT a valid directory.')

    ##
    # 
    #         Returns the path to the directory.
    #         @return The absolute path associated with this directory.
    #         
    def __call__(self):
        return self.Path

    ##
    # 
    #         Gives an absolute path version of a path relative to the 'Current Directory'.
    #         If given an absolute path, does nothing but give it OS formatting.
    #         @param RelativePath A path relative to the directory.
    #         @return The absolute path with current OS formatting.
    #         
    def GenAbsPath(self, RelativePath):
        FinalAbsolutePath = os.path.join(self.Path, RelativePath)
        FinalAbsolutePath = re.sub(r'\\', '/', FinalAbsolutePath)
        FinalAbsolutePath = os.path.normpath(FinalAbsolutePath)
        return FinalAbsolutePath


##
# 
#     A make directory function that suppresses the already exists error.
# 
#     @param Directory The directory to make.
#     @return None
#     
def MakeDir(Directory):
    try:
        os.makedirs(Directory)
    except OSError as OsException:
        if OsException.errno != errno.EEXIST:
            raise OSError(OsException)


##
# 
#     This function will copy a source file to a directory only if the file exists.
#     Will not raise an exception if the files does not exist, but prints a message
#     as an error.
# 
#     @param Source Absolute path to a source file.
#     @param DestDirectory Absolute path to a destination directory (NOT FILE).
#     @param Quiet Will suppress error message on unsuccessful copy.
#     @return True if successful, false if not.
#     
def CopyToDirIfExists(Source, DestDirectory, Quiet=False):
    CopiedSuccess = False
    if os.path.isfile(Source):
        # Cannot use shutil.copy because it copies permissions as well.
        shutil.copyfile(Source, os.path.join(DestDirectory, os.path.basename(Source)))
        CopiedSuccess = True
    elif not Quiet:
        logger.warning('Source file %s does not exist. Cannot copy to %s', Source, DestDirectory)
    return  CopiedSuccess


##
# 
#     Removes a file only if it exists
# 
#     @param FilePath The absolute path to the file to be deleted
#     @return True if file existed and was deleted, false otherwise
#     
def RemoveFileIfExists(FilePath):
    DeleteSuccess = False

    if os.path.isfile(FilePath):
        os.remove(FilePath)
        DeleteSuccess = True

    return DeleteSuccess



##
# 
#     Since shutil.copytree does not work if the destination directory already exists
#     we had to make our own. Copied from: http://tinyurl.com/q9xc492
# 
#     @param Source The absolute path of the source tree directory.
#     @param Destination The absolute path of the destination directory
#     @param SymLinks
#     @param Ignore Ignore errors
#     @return None
#     
def CopyTree(Source, Destination, SymLinks=False, Ignore=None):
    if not os.path.exists(Destination):
        os.makedirs(Destination)
    for item in os.listdir(Source):
        s = os.path.join(Source, item)
        d = os.path.join(Destination, item)
        if os.path.isdir(s):
            CopyTree(s, d, SymLinks, Ignore)
        else:
            shutil.copyfile(s, d)


##
# 
#     An OS-independent CLI like call function, using wine for *nix exe support and running python scripts
#     with the currently running version of python.
# 
#     @param CallList Pass this in as a LIST of arguments. Subprocess.call is more
#                     stable when given a list.
#     @param Log Default is None - Provide the name of the file you want the log to be in as a string.
#                 callExecutable will open this as a file and call will redirect stdout to this file.
#     @param WantShell Default is True. Allows shell like commands and arguments.
#                       Set to false when not needed. (I haven't run into a case where it wasn't yet)
#     @return The return value from subprocess.call. Non-zero for error, 0 for success.
#     
def CliLikeCall(CallList, Log=None, WantShell=True, IgnoreError=False, PythonScript=False):

    # If there is a log file, open it
    LogFile = None
    if Log:
        LogFile = open(Log, 'a+')

    Prefix = 'NOT SET'

    if PythonScript:
        # if it is a python script, call it with the currently running python interpreter
        Prefix = sys.executable
    elif sys.platform.startswith('linux'):
        # If this running on linux, run the exe with wine
        Prefix = 'wine'

    # Keep this function flexible by pre-fixing for strings and pushing for lists
    if Prefix is not 'NOT SET':
        if isinstance(CallList, list):
            CallList.insert(0, Prefix)
        else:
            CallList = Prefix + ' ' + CallList

    # Call the exe with arguments, raising an error with the appropriate handling
    Error = subprocess.call(CallList, stdout=LogFile, shell=WantShell)
    if Error != 0 and not IgnoreError:
        logger.critical('Error code: %s', str(Error))
        if isinstance(CallList, list):
            logger.critical('Failed to call: %s', ' '.join(CallList))
        else:
            logger.critical('Failed to call: %s', CallList)
        raise subprocess.CalledProcessError(Error, CallList)

    # Close the log file
    if Log:
        LogFile.close()

    return Error


##
# 
#     Makes a text file at the path provided that contains the text passed in.
# 
#     @param FilePath Absolute path to the file's destination including file name.
#     @param Text The text to write into the text file.
#     @return None
#     
def MakeTextFile(FilePath, Text):
    FileHandle = open(FilePath, 'w')
    FileHandle.write(Text)
    FileHandle.close()


##
# 
#     This function will clean a folder of files, and supports wildcards.
# 
#     @param FolderPath The absolute path to the folder needing cleaing.
#     @param ListOfFiles A list of files to search for and delete. Supports use of wildcards like *
#     @return None
#     
def CleanFolderOfFiles(FolderPath, ListOfFiles):

    FolderPath =  OsPath(FolderPath)

    for Extensions in ListOfFiles:
        # Glob returns a list of file paths matching the search query
        ListOfFiles = glob.glob(os.path.join(FolderPath, Extensions))

        for EachFile in ListOfFiles:
            os.remove(EachFile) #SilentRemove(EachFile)
            
            
##
# 
#     Grabs the IFWI Configuration file passed in by CLI, and if none, uses the default passed in.
#     Parses the IFWI configuration file and returns a ConfigParser object that is populated
#     with defaults from the config file.
# 
#     @param DefaultIfwiConfig The absolute path to the default IFWI configuration file
#     @return A ConfigParser object that is populated with defaults from the config file.
#     
def ParseConfigFile(DefaultIfwiConfig):

    # Check the CLI for a config file argument
    CfgFileParser = argparse.ArgumentParser(add_help=False)
    CfgFileParser.add_argument('--IfwiConfig', dest='CfgFile', metavar='FILEPATH',
                               help='Specify alternate IFWI config file to use', default=DefaultIfwiConfig)
    Args, RemainingArgv = CfgFileParser.parse_known_args()

    # Open the configuration file
    if os.path.isfile(Args.CfgFile):
        logger.info('\nParsing IFWI Configuration file ... (%s)\n', Args.CfgFile )
        ConfigOptions = ConfigParser.SafeConfigParser()
        ConfigOptions.read(Args.CfgFile)
    else:
        raise InvalidSetupError('\nConfig File ' + Args.CfgFile + ' not found!')

    # Set default for IFWI Suffix
    ConfigOptions.set('IfwiSuffix', 'Value', time.strftime('%Y_%m_%d_%H%M'))

    return ConfigOptions


##
# 
#     Check is a string represents a boolean value. If so, return true. If not, return false.
# 
#     @param StringValue A string that
#     @return True if string represents a boolean value. False otherwise.
#     
def CheckIfStringIsBoolean(StringValue):
    ValidBoolStrings = ['True', 'False', 'On', 'Off', '0', '1']
    IsBoolean = False

    if StringValue in ValidBoolStrings:
        IsBoolean = True

    return IsBoolean


##
# 
#     This will use a properly formatted ConfigParser.SafeConfigParser instance to
#     generate argparse objects that reflect the configuration outlined in the IFWI configuration file.
#     It does this dynamically so that it does not need updating when the IFWI configuration file changes.
# 
#     @param ConfigOptions A ConfigParser object that was created from parsing a
#                           properly formatted IFWI configuration file.
#     @return An argsparse object that is populated with the defaults gleamed from the IFWI Configuration file.
#     
def ParseArguments(ConfigOptions, TargetScripts, ScriptDescription):

    logger.debug('Number of arguments: %i arguments.', len(sys.argv))
    logger.debug('Argument List: %s', str(sys.argv))
    logger.info('Processing CLI arguments')

    assert isinstance(ConfigOptions, ConfigParser.SafeConfigParser)
    assert isinstance(TargetScripts, list)

    # Setup a parser for the command line arguments and the
    # default option group.
    Parser = argparse.ArgumentParser(description=ScriptDescription, prefix_chars= '/')
    CurrentOptionGroup = Parser

    logger.debug('Merging IFWI Configuration file into CLI arguments...')
    for EachOption in ConfigOptions.sections():
        # For each option, verify that that the option is valid
        if ConfigOptions.has_option(EachOption, 'SectionHeader'):
            # If there is a section header, make an option group and move on.
            CurrentOptionGroup = Parser.add_argument_group(ConfigOptions.get(EachOption, 'SectionHeader'))
            continue

        # Verify that the option targets the right scripts.
        IgnoreOption = True
        for EachScript in TargetScripts:
            if re.search(EachScript, ConfigOptions.get(EachOption, 'Target')):
                IgnoreOption = False
                break

        # Skip options not meant for the targeted script
        if IgnoreOption:
            continue

        # If the Option value is of bool type, the argument action should be to store a bool
        # otherwise, it should store a string value.
        ArgAction = 'store_true'
        if CheckIfStringIsBoolean(ConfigOptions.get(EachOption, 'Value')):
            DefaultValue =  ConfigOptions.getboolean(EachOption, 'Value')
        else:
            ArgAction = 'store'
            DefaultValue = ConfigOptions.get(EachOption, 'Value')

        # If the config file option has a set of valid choices, pass them onto the
        # ArgParse argument. Otherwise, do not give the ArgParse argument a Choices entry.
        # Redundant, but argparser freaks when I try choices= None
        if ConfigOptions.get(EachOption, 'Flag') == 'Positional':
            CurrentOptionGroup.add_argument(
                                            EachOption,
                                            action= ArgAction,
                                            nargs= '?',
                                            default= DefaultValue,
                                            help= ConfigOptions.get(EachOption, 'Help')
                                           )
        elif ConfigOptions.has_option(EachOption, 'Choices'):
            ArgChoices = ConfigOptions.get(EachOption, 'Choices')
            ArgChoices = re.findall(r'(?<=\')\w+(?=\')', ArgChoices)
            # Has choices
            CurrentOptionGroup.add_argument(
                                            ConfigOptions.get(EachOption, 'Flag'),
                                            dest= EachOption,
                                            action= ArgAction,
                                            choices= ArgChoices,
                                            default= DefaultValue,
                                            help= ConfigOptions.get(EachOption, 'Help')
                                           )
        else:
            # No choices
            CurrentOptionGroup.add_argument(
                                            ConfigOptions.get(EachOption, 'Flag'),
                                            dest= EachOption,
                                            action= ArgAction,
                                            default= DefaultValue,
                                            help= ConfigOptions.get(EachOption, 'Help')
                                           )

    # Get the CLI arguments
    logger.info('Parsing CLI Arguments...')
    Args, RemainingArgv = Parser.parse_known_args()

    return Args

##
# 
#     Dynamic argument requirement checking. Currently only
#     supports the == operator and this format. Can be extended for additional operators.
# 
#     @param ConfigOptions A ConfigParser object that was created from parsing a
#                           properly formatted IFWI configuration file.
#     @param Args An argsparse object that is populated with the defaults gleamed
#                  from the IFWI Configuration file.
#     @return None
#     
def CheckArgRequirements(ConfigOptions, Args):
    print(str(Args))
    DictOfArgs = vars(Args)

    # Make sure the passed in arguments pass requirements
    for Argument in DictOfArgs:
        # For each argument,
        if DictOfArgs[Argument] and ConfigOptions.has_option(Argument, 'Required'):
            # If it is both required and has the attribute Required
            Requirement = ConfigOptions.get(Argument, 'Required')
            logger.debug('Section ' + Argument + ' requires ' + Requirement)

            # Check the requirement
            RequiredSection, RequiredValue = Requirement.split(' == ')
            if not DictOfArgs[RequiredSection] == RequiredValue:
                raise InvalidSetupError(
                                        'Requirement not met. ' + RequiredSection + ' == ' +
                                        ConfigOptions.get(RequiredSection, 'Value')
                                        )
            else:
                logger.debug('... Requirement check passed ...')

