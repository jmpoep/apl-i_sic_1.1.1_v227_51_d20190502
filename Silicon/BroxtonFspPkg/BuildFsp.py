##
#
# @package BuildFsp
# Builds the FSP IAFW.
#
# Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.<BR>
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
import sys
import shutil
import argparse
import Clean
import stat
import logging
import logging.config

import edksetup
edksetup.Run(CodeCoveragePath=False, Reconfig= False)
sys.path.append(os.path.join(os.environ['WORKSPACE'], 'BuildScripts'))

import CommonHelperFunctions
import BuildHelperFunctions
from CommonHelperFunctions import OsPath
import platform

# Setup Environment
RootDirectory = CommonHelperFunctions.CurrentDirectory(os.environ['WORKSPACE'])
logging.config.fileConfig(RootDirectory.GenAbsPath('BuildScripts/logging.conf'), disable_existing_loggers= False)

# Create logger - getLogger treats loggers like singletons (will not make more than one instance)
logger = logging.getLogger('BuildIfwi')
OS = platform.system()

def OverrideBaseTools():

    BuildRuleTemplate = RootDirectory.GenAbsPath('BroxtonFspPkg/Override/BaseTools/Conf/build_rule.template')
    ToolsDefTemplate  = RootDirectory.GenAbsPath('BroxtonFspPkg/Override/BaseTools/Conf/tools_def.template')
    ScriptTemplate    = RootDirectory.GenAbsPath('BroxtonFspPkg/Override/BaseTools/Scripts/gcc4.9-ld-script')

    if not os.path.exists('Conf'):
      os.makedirs('Conf')

    if os.path.isfile(BuildRuleTemplate):
      shutil.copyfile(BuildRuleTemplate, RootDirectory.GenAbsPath('Conf/build_rule.txt'))

    if os.path.isfile(ToolsDefTemplate):
      shutil.copyfile(ToolsDefTemplate, RootDirectory.GenAbsPath('Conf/tools_def.txt'))

    if os.path.isfile(ScriptTemplate):
      shutil.copyfile(ScriptTemplate, RootDirectory.GenAbsPath('BaseTools/Scripts/gcc4.9-ld-script'))

    shutil.copyfile(
            RootDirectory.GenAbsPath('BaseTools/Conf/target.template'),
            RootDirectory.GenAbsPath('Conf/target.txt')
            )

def BuildVpd(PlatDataTextPath, VpdToolPath):

    PlatDataPath = PlatDataTextPath.split('.txt')[0]
    PlatDataBinPath = PlatDataPath + '.bin'
    PlatDataMapPath = PlatDataPath + '.map'
    PlatDataTextPath = PlatDataPath + '.txt'

    # Delete the VPD/UPD Header binary and map files if they exist.
    CommonHelperFunctions.RemoveFileIfExists(PlatDataBinPath)
    CommonHelperFunctions.RemoveFileIfExists(PlatDataMapPath)

    BdpgCallString = VpdToolPath + \
                         ' ' + PlatDataTextPath + \
                         ' -o ' + PlatDataBinPath + \
                         ' -m ' + PlatDataMapPath
    if OS == 'Windows':
        CommonHelperFunctions.CliLikeCall(BdpgCallString)
    else :
        os.system(BdpgCallString)

##
#
#     When FSP fails during the pre-build stage, this method should be used.
#     It will delete all the files passed in before raising a Build Failure Error.
#
#     @param ErrorMessage The message to display upon raising the Build Failure Error.
#     @param ListOfFilesToDelete A list of files to delete (if they exist) before failing.
#     @return None
#
def PreBuildFail(ErrorMessage, ListOfFilesToDelete):

    for EachFile in ListOfFilesToDelete:
        CommonHelperFunctions.RemoveFileIfExists(EachFile)

    raise BuildHelperFunctions.BuildFailureError(ErrorMessage)


def Run(PlatformType='BXT', BuildTarget='Debug', Compiler='VS2013', CleanBuild=False):

    MasterFspBuildSettings = {
        'Compiler'          : 'NOT SET',
        'BaseName'          : 'NOT SET',
        'BuildTarget'       : 'NOT SET',
        'BuildLog'          : 'NOT SET',
        'PlatformPkg'       : 'NOT SET',
        'BinPkg'            : 'NOT SET',
        'BuildFvPath'       : 'NOT SET',
        'PlatformPkgDscPath': 'NOT SET',
        'BuildMacros'       : 'NOT SET',
        'BuildArgs'         : 'NOT SET',
        'FspTUpdGuid'       : '34686CA3-34F9-4901-B82A-BA630F0714C6',
        'FspMUpdGuid'       : '39A250DB-E465-4DD1-A2AC-E2BD3C0E2385',
        'FspSUpdGuid'       : 'CAE3605B-5B34-4C85-B3D7-27D54273C40F',
        'KeyVpdProducts'    : ['NOT SET'],
        'PythonPath'        : sys.executable
    }

    if sys.platform.startswith('win'):
        MasterFspBuildSettings['Compiler'] = BuildHelperFunctions.SetupVisualStudio(Compiler)
    else:
        MasterFspBuildSettings['Compiler'] = BuildHelperFunctions.SetupGccVersion()

    edksetup.Run(CodeCoveragePath=False, Reconfig= False)

    CommonHelperFunctions.CopyToDirIfExists(
                                            RootDirectory.GenAbsPath('BroxtonFspPkg\Include\SmipGenerated.h'),
                                            RootDirectory.GenAbsPath('Silicon\BroxtonSiPkg\Include')
                                           )

    if PlatformType == 'BXT':
        MasterFspBuildSettings['BaseName'] = 'Broxton'
        PackageName = MasterFspBuildSettings['BaseName'] + 'FspPkg'
        BinPackageName = MasterFspBuildSettings['BaseName'] + 'FspBinPkg'

    else:
        raise BuildHelperFunctions.BuildFailureError("Bad Package Name " + PlatformType)

    if BuildTarget.lower() == 'debug':
        MasterFspBuildSettings['BuildTarget'] = 'DEBUG'
        MasterFspBuildSettings['BuildLog'] = 'ReportDebug.log'
        BuildType=0x0000
        ReleaseType=0x0000
    elif BuildTarget.lower() == 'release':
        MasterFspBuildSettings['BuildTarget'] = 'RELEASE'
        MasterFspBuildSettings['BuildLog'] = 'ReportRelease.log'
        BuildType=0x0001
        ReleaseType=0x0002
    elif BuildTarget.lower() == 'testrelease':
        MasterFspBuildSettings['BuildTarget'] = 'RELEASE'
        MasterFspBuildSettings['BuildLog'] = 'ReportRelease.log'
        BuildType=0x0001
        ReleaseType=0x0000
    else:
        raise BuildHelperFunctions.BuildFailureError("Bad build argument value: " + BuildTarget)

    MasterFspBuildSettings['PlatformPkg'] = RootDirectory.GenAbsPath(PackageName)
    MasterFspBuildSettings['BinPkg']      = RootDirectory.GenAbsPath(BinPackageName)
    OutDirectory = 'Build'
    MasterFspBuildSettings['BuildFvPath'] = RootDirectory.GenAbsPath(OutDirectory +
                                                 '/' + PackageName +
                                                 '/' + MasterFspBuildSettings['BuildTarget'] +
                                                 '_' + MasterFspBuildSettings['Compiler'] +
                                                 '/' + 'FV')

    MasterFspBuildSettings['PlatformPkgDscPath'] = OsPath(
                                                          MasterFspBuildSettings['PlatformPkg'] +
                                                          '/' + PackageName + '.dsc'
                                                         )

    MasterFspBuildSettings['KeyVpdProducts'] = [
                                                OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h'),
                                                OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FsptUpd.h'),
                                                OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspmUpd.h'),
                                                OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspsUpd.h'),
                                               ]

    ############################ Assemble Build Call Strings ###########################################################

    FspTest = '0'
    BuildCommonArgs = '-p ' + OsPath(MasterFspBuildSettings['PlatformPkgDscPath']) + \
                                            ' -D FSP_FLAG' \
                                            ' -D FSP_TEST=' + FspTest + \
                                            ' -a IA32' \
                                            ' -n 4' \
                                            ' -t ' + MasterFspBuildSettings['Compiler'] + \
                                            ' -Y PCD' \
                                            ' -Y LIBRARY'

    MasterFspBuildSettings['BuildMacros'] = '-DFSP_FLAG'

    MasterFspBuildSettings['BuildArgs'] = BuildCommonArgs + \
                                          ' -b ' + MasterFspBuildSettings['BuildTarget'] + \
                                          ' ' + MasterFspBuildSettings['BuildMacros'] + \
                                          ' -y ' + MasterFspBuildSettings['BuildLog']



    ########################## Prebuild Steps #########################################################################

    if CleanBuild:
        Clean.Run(RootDirectory, IntermediateOnly=False, BinaryDelete=False)

    logger.info('PREBUILD')

    # !!!!!!!!! Override BaseTools !!!!!!!!!!!!!!
    OverrideBaseTools()
    # !!!!!!!!! Build the FspHeader.inf file !!!!!!!!!!!!!!
    # Generates FspHeader objects as well as VPD header information.
    FspHeaderInfPath = OsPath(MasterFspBuildSettings['PlatformPkg'] + '/FspHeader/FspHeader.inf')
    if OS == 'Windows' :
        BuildCall = RootDirectory.GenAbsPath('BaseTools/Bin/Win32/build.exe')
    else:
        BuildCall = 'build'

    BuildCall = BuildCall + \
                ' -m' + \
                ' ' + FspHeaderInfPath + \
                ' ' + MasterFspBuildSettings['BuildArgs'] + \
                ' ' + '-DCFG_PREBUILD'

    if OS == 'Windows':
        CommonHelperFunctions.CliLikeCall(BuildCall)
    else :
        ErrorLevel = os.system(BuildCall)

    if ErrorLevel != 0 :
        sys.exit(1)

    # !!!!!!!! Update VPD/UPD Information !!!!!!!!
    logger.info('Preparing VPD/UPD Information...')

    GenCfgOptPath = RootDirectory.GenAbsPath('IntelFsp2Pkg\Tools\GenCfgOpt.py')

    UpdTxtCallString = GenCfgOptPath + \
                       ' ' + 'UPDTXT' + \
                       ' ' + MasterFspBuildSettings['PlatformPkgDscPath'] + \
                       ' ' + OsPath(MasterFspBuildSettings['BuildFvPath']) + \
                       ' ' + MasterFspBuildSettings['BuildMacros']


    ErrorLevel = CommonHelperFunctions.CliLikeCall(UpdTxtCallString, IgnoreError= True, PythonScript= True)
    if ErrorLevel == 1:
        PreBuildFail(
                     'UPD TXT file was unable to be generated successfully',
                     MasterFspBuildSettings['KeyVpdProducts']
                    )
    logger.info('UPD TXT file was generated successfully')

    if not ErrorLevel == 256:
        # If there is no existing UPD text file that is newer than the platform DSC file,
        # update the UPD .map and .bin files
        logger.info('Updating UPD information files...')

        if OS == 'Windows':
            BdpgExePath = RootDirectory.GenAbsPath('BaseTools\Bin\Win32\BPDG.exe')
        else :
            BdpgExePath = RootDirectory.GenAbsPath('BaseTools\BinWrappers\PosixLike\BPDG')

        # Update FSP - T (IBBL)
        FspTUpdGuidPath = os.path.join(MasterFspBuildSettings['BuildFvPath'], MasterFspBuildSettings['FspTUpdGuid'])
        BuildVpd(PlatDataTextPath= FspTUpdGuidPath, VpdToolPath= BdpgExePath)

        # Update FSP - M (IBBM)
        FspMUpdGuidPath = os.path.join(MasterFspBuildSettings['BuildFvPath'], MasterFspBuildSettings['FspMUpdGuid'])
        BuildVpd(PlatDataTextPath= FspMUpdGuidPath, VpdToolPath= BdpgExePath)

        # Update FSP - S (IBBR)
        FspSUpdGuidPath = os.path.join(MasterFspBuildSettings['BuildFvPath'], MasterFspBuildSettings['FspSUpdGuid'])
        BuildVpd(PlatDataTextPath= FspSUpdGuidPath, VpdToolPath= BdpgExePath)

        logger.info('... done')

    # Update VPD Header and generate UPD information text files
    HeaderCallString = GenCfgOptPath + \
                       ' ' + 'HEADER' + \
                       ' ' + MasterFspBuildSettings['PlatformPkgDscPath'] + \
                       ' ' + OsPath(MasterFspBuildSettings['BuildFvPath']) + \
                       ' ' + OsPath(MasterFspBuildSettings['PlatformPkg'] +
                                            '/Include/BootLoaderPlatformData.h') + \
                       ' ' + MasterFspBuildSettings['BuildMacros']
    os.system('touch ' + MasterFspBuildSettings['PlatformPkgDscPath'])

    if os.path.exists('BroxtonFspPkg/Include/FspmUpd.h'):
        os.chmod('BroxtonFspPkg/Include/FspmUpd.h', stat.S_IWRITE)
    if os.path.isfile(OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h'), stat.S_IWRITE)
    ErrorLevel = CommonHelperFunctions.CliLikeCall(HeaderCallString, IgnoreError= True, PythonScript= True)
    if ErrorLevel == 1:
        PreBuildFail(
                     'Vpd header file was unable to be generated successfully !',
                     MasterFspBuildSettings['KeyVpdProducts']
                    )
    logger.info('Vpd header file was generated successfully !')
    if not ErrorLevel == 256:
        # If there is no existing UPD text file that is newer than the platform DSC file,
        # update the Boot Settings File (BSF)
        logger.info('Generate BSF File ...')

        FspBsfPath = OsPath(MasterFspBuildSettings['BinPkg'] + '/Fsp.bsf')

        GenBsfCallString = MasterFspBuildSettings['PythonPath'] + \
                           ' ' + GenCfgOptPath + \
                           ' ' + 'GENBSF' + \
                           ' ' + MasterFspBuildSettings['PlatformPkgDscPath'] + \
                           ' ' + OsPath(MasterFspBuildSettings['BuildFvPath']) + \
                           ' ' + FspBsfPath + \
                           ' ' + MasterFspBuildSettings['BuildMacros']
        if OS == 'Windows':
            Error = CommonHelperFunctions.CliLikeCall(GenBsfCallString, IgnoreError= True, PythonScript= True)
        else:
            os.system(GenBsfCallString)

        if OS == 'Windows':
            if Error == 1:
                PreBuildFail('Failed to generate ' + FspBsfPath)

        logger.info('BSF file was generated successfully!')

    if os.path.isfile(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspUpd.h'), stat.S_IWRITE)
    if os.path.isfile(OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h'), stat.S_IREAD)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h'),
                    OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspUpd.h')
                    )

    if os.path.isfile(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FsptUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FsptUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FsptUpd.h'),
                    OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FsptUpd.h')
                    )
    if os.path.isfile(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspmUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspmUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspmUpd.h'),
                    OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspmUpd.h')
                    )
    if os.path.isfile(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspsUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspsUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspsUpd.h'),
                    OsPath(MasterFspBuildSettings['PlatformPkg'] + '/Include/FspsUpd.h')
                    )




    logger.info('End of PreBuild...')

    ################################ BUILD #############################################################################
    logger.info('BUILD')
    if os.path.exists('BroxtonFspPkg/Include/FspUpd.h'):
        os.chmod('BroxtonFspPkg/Include/FspUpd.h', stat.S_IREAD)
    if os.path.exists('BroxtonFspPkg/Include/FsptUpd.h'):
        os.chmod('BroxtonFspPkg/Include/FsptUpd.h', stat.S_IREAD)
    if os.path.exists('BroxtonFspPkg/Include/FspmUpd.h'):
        os.chmod('BroxtonFspPkg/Include/FspmUpd.h', stat.S_IREAD)
    if os.path.exists('BroxtonFspPkg/Include/FspsUpd.h'):
        os.chmod('BroxtonFspPkg/Include/FspsUpd.h', stat.S_IREAD)

    if OS == 'Windows':
        BuildCall = RootDirectory.GenAbsPath('BaseTools/Bin/Win32/build.exe')
    else:
        BuildCall = 'build'

    BuildCall = 'build' + \
                ' ' + MasterFspBuildSettings['BuildArgs']

    if OS == 'Windows':
        CommonHelperFunctions.CliLikeCall(BuildCall)
    else:
        BuildCall = BuildCall + ' -DGCC'
        ErrorLevel = os.system(BuildCall)

    if ErrorLevel != 0 :
        sys.exit(1)

    ################################ POST BUILD ########################################################################

    logger.info('Start of PostBuild...')

    logger.info('Patch FD Image')

    # !!!! PATCH FD IMAGE !!!!!!
    PatchFvPyPath = RootDirectory.GenAbsPath('IntelFsp2Pkg\Tools\PatchFv.py')

    # !!!! PATCH FSP-T IMAGE !!!!!!
    logger.info('Patch FSP-T Image')
    PatchFvBaseString = MasterFspBuildSettings['PythonPath'] + \
                        ' ' + PatchFvPyPath + \
                        ' ' + OsPath(MasterFspBuildSettings['BuildFvPath']) + \
                        ' ' + 'FSP-T:BROXTON' + ' '

    PatchFvImage = PatchFvBaseString + \
     '"0x0000,            _BASE_FSP-T_,                                                    @Temporary Base" ' \
     '"<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                             @FSP-T Size" ' \
     '"<[0x0000]>+0x00B0, [0x0000],                                                        @FSP-T Base" ' \
     '"<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                     @FSP-T Image Attribute" ' \
     '"<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x1000 | %x | %x,           @FSP-T Component Attribute" ' \
     '"<[0x0000]>+0x00B8, 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x1C - <[0x0000]>,          @FSP-T CFG Offset" ' \
     '"<[0x0000]>+0x00BC, [70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x14] & 0xFFFFFF - 0x001C, @FSP-T CFG Size" ' \
     '"<[0x0000]>+0x00C4, FspSecCoreT:_TempRamInitApi - [0x0000],                           @TempRamInit API" ' \
     '"0x0000,            0x00000000,                                                      @Restore the value" ' \
     '"FspSecCoreT:FspInfoHeaderRelativeOff, FspSecCoreT:_AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-T Header Offset"' %(BuildType, ReleaseType)

    os.system(PatchFvImage)

    # !!!! PATCH FSP-M IMAGE !!!!!!
    logger.info('Patch FSP-M Image')
    PatchFvBaseString = MasterFspBuildSettings['PythonPath'] + \
                        ' ' + PatchFvPyPath + \
                        ' ' + OsPath(MasterFspBuildSettings['BuildFvPath']) + \
                        ' ' + 'FSP-M:BROXTON' + ' '

    PatchFvImage = PatchFvBaseString + \
     '"0x0000,            _BASE_FSP-M_,                                                    @Temporary Base" ' \
     '"<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                             @FSP-M Size" ' \
     '"<[0x0000]>+0x00B0, [0x0000],                                                        @FSP-M Base" ' \
     '"<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                     @FSP-M Image Attribute" ' \
     '"<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x2000 | %x | %x,           @FSP-M Component Attribute" ' \
     '"<[0x0000]>+0x00B8, D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x1C - <[0x0000]>,          @FSP-M CFG Offset" ' \
     '"<[0x0000]>+0x00BC, [D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x14] & 0xFFFFFF - 0x001C, @FSP-M CFG Size" ' \
     '"<[0x0000]>+0x00D0, FspSecCoreM:_FspMemoryInitApi - [0x0000],                         @MemoryInitApi API" ' \
     '"<[0x0000]>+0x00D4, FspSecCoreM:_TempRamExitApi - [0x0000],                           @TempRamExit API" ' \
     '"FspSecCoreM:_FspPeiCoreEntryOff, PeiCore:__ModuleEntryPoint - [0x0000],               @PeiCore Entry" ' \
     '"0x0000,            0x00000000,                                                      @Restore the value" ' \
     '"FspSecCoreM:_FspInfoHeaderRelativeOff, FspSecCoreM:_AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-M Header Offset"' %(BuildType, ReleaseType)

    os.system(PatchFvImage)


    # !!!! PATCH FSP-S IMAGE !!!!!!
    logger.info('Patch FSP-S Image')
    PatchFvBaseString = MasterFspBuildSettings['PythonPath'] + \
                        ' ' + PatchFvPyPath + \
                        ' ' + OsPath(MasterFspBuildSettings['BuildFvPath']) + \
                        ' ' + 'FSP-S:BROXTON' + ' '

    PatchFvImage = PatchFvBaseString + \
     '"0x0000,            _BASE_FSP-S_,                                                    @Temporary Base" ' \
     '"<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                             @FSP-S Size" ' \
     '"<[0x0000]>+0x00B0, [0x0000],                                                        @FSP-S Base" ' \
     '"<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                     @FSP-S Image Attribute " ' \
     '"<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x3000 | %x | %x,           @FSP-S Component Attribute" ' \
     '"<[0x0000]>+0x00B8, E3CD9B18-998C-4F76-B65E-98B154E5446F:0x1C - <[0x0000]>,          @FSP-S CFG Offset" ' \
     '"<[0x0000]>+0x00BC, [E3CD9B18-998C-4F76-B65E-98B154E5446F:0x14] & 0xFFFFFF - 0x001C, @FSP-S CFG Size" ' \
     '"<[0x0000]>+0x00D8, FspSecCoreS:_FspSiliconInitApi - [0x0000],                        @SiliconInit API" ' \
     '"<[0x0000]>+0x00CC, FspSecCoreS:_NotifyPhaseApi - [0x0000],                           @NotifyPhase API" ' \
     '"0x0000,            0x00000000,                                                      @Restore the value" ' \
     '"FspSecCoreS:_FspInfoHeaderRelativeOff, FspSecCoreS:_AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-S Header Offset" ' %(BuildType, ReleaseType)


    os.system(PatchFvImage)

    logger.info('PostBuild is DONE')

    ######################## PACKING UP ###############################################################################
    logger.info('Copy Fsp images to /BuildFsp')
    BuildFspDirectory = RootDirectory.GenAbsPath('BuildFsp')
    CommonHelperFunctions.MakeDir(BuildFspDirectory)

    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/BROXTON.fd'),
                    OsPath(BuildFspDirectory + '/BROXTON_FSP.fd')
                   )
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FsptUpd.h'),
                    OsPath(BuildFspDirectory + '/FsptUpd.h')
                   )
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspmUpd.h'),
                    OsPath(BuildFspDirectory + '/FspmUpd.h')
                   )
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspsUpd.h'),
                    OsPath(BuildFspDirectory + '/FspsUpd.h')
                   )
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h'),
                    OsPath(BuildFspDirectory + '/FspUpd.h')
                   )


    if os.path.isfile(OsPath(MasterFspBuildSettings['BinPkg'] + '/Fsp.fd')):
        os.chmod(OsPath(MasterFspBuildSettings['BinPkg'] + '/Fsp.fd'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/BROXTON.fd'),
                    OsPath(MasterFspBuildSettings['BinPkg'] + '/Fsp.fd')
                    )
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BinPkg'] + '/Fsp.bsf'),
                    OsPath(BuildFspDirectory + '/BROXTON_FSP.bsf')
                   )

    if os.path.isfile(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspUpd.h'),
                    OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspUpd.h')
                    )

    if os.path.isfile(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FsptUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FsptUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FsptUpd.h'),
                    OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FsptUpd.h')
                    )

    if os.path.isfile(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspmUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspmUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspmUpd.h'),
                    OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspmUpd.h')
                    )

    if os.path.isfile(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspsUpd.h')):
        os.chmod(OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspsUpd.h'), stat.S_IWRITE)
    shutil.copyfile(
                    OsPath(MasterFspBuildSettings['BuildFvPath'] + '/FspsUpd.h'),
                    OsPath(MasterFspBuildSettings['BinPkg'] + '/Include/FspsUpd.h')
                    )
    sys.exit(0)



# CLI Support
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument(
                        '-PlatformType',
                        choices= ['BXT'],
                        default='BXT',
                        metavar='[BXT]',
                        action='store',
                        help="The type of FSP platform to build"
                       )
    parser.add_argument(
                        '-BuildTarget',
                        choices=['Debug', 'Release', 'TestRelease'],
                        default='Debug',
                        metavar='[Debug, Release, TestRelease]',
                        action='store',
                        help='Set the type of build'
                        )
    parser.add_argument(
                        '-vs',
                        choices=['08', '10', '12', '13'],
                        default='13',
                        action='store',
                        help='Select Visual Studio version 08: 2008, 10: 2010, 12: 2012, 13: 2013'
                        )
    parser.add_argument(
                        '-Clean',
                        default=False,
                        action='store_true',
                        help="Clean the directory (including /Build and /BuildFsp) before Building"
                        )
    parser.add_argument(
                        '-Log',
                        default= 'Ifwi.log',
                        help='The Log file to use'
                        )
    Args = parser.parse_args()

    if Args.vs:
        Args.vs = 'VS20' + Args.vs

    Run(
        PlatformType= Args.PlatformType,
        BuildTarget= Args.BuildTarget,
        Compiler= Args.vs,
        CleanBuild= Args.Clean
        )
