##
# 
# @package Clean
# Cleans temporary files related to the EDK2 Build and Stitch process.
# 
# Example useage:
#   Clean.py  -->              A + B
#   Clean.py  -->  /intm       A    ** This is the default usage in the build script.
#   Clean.py  -->  /intm /bin  A + B + C
#   Where:
#     A:  Intermediate Build Files
#     B:  Build Folder
#     C:  .rom and .bin files
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

import argparse
import os
import glob
import shutil
import logging
import sys

BuildScriptDirectory = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'BuildScripts')
sys.path.append(BuildScriptDirectory)
import CommonHelperFunctions
from CommonHelperFunctions import OsPath

logger = logging.getLogger(__name__)

##
# 
#     Runs the clean tool.
# 
#     @param RootFolder A CurrentDirectory object instance to the workspace root
#     @param IntermediateOnly Will only clean files that are intermediate between Builds and Stitches
#     @param BinaryDelete Delete .zip and .bin binary files from the /Stitch folder
#     @return
#     
def Run(RootFolder, IntermediateOnly=True, BinaryDelete=False):
    logger.info("Cleaning Platform Folders")

    # Delete all DefineAtBuildMacros.dsc and reset vector intermediate build files
    PlatformFilesToDelete = [
                               'DefineAtBuildMacros.dsc',
                               OsPath('Override/UefiCpuPkg/ResetVector/Vtf*/Bin/*.raw'),
                               OsPath('Override/Core/UefiCpuPkg/ResetVector/Vtf*/Bin/*.raw')
                            ]

    ListOfPlatforms = os.listdir(RootFolder.GenAbsPath('Platform'))
    for PlatformFolder in ListOfPlatforms:
        logger.debug(" checking " + PlatformFolder)
        PlatformFolderPath = os.path.join(RootFolder.GenAbsPath('Platform'), PlatformFolder)
        CommonHelperFunctions.CleanFolderOfFiles(FolderPath=PlatformFolderPath, ListOfFiles= PlatformFilesToDelete)

    # Remove all intermediate build files stored in the CSE directories
    logger.info("Cleaning Stitch/CSE Folders...")
    CseDirectory = RootFolder.GenAbsPath('Stitch/CSE')
    ListOfCseVersions = os.listdir(CseDirectory)
    CseFilesToDelete = [
                        '*.map',
                        '*.xml',
                        '*.log',
                        'SmipSubPartition*.bin',
                        '*emmc*.bin'
                       ]

    for EachCseVersion in ListOfCseVersions:
        logger.debug(" checking " + EachCseVersion)
        FitPath = os.path.join(CseDirectory, EachCseVersion, '*', 'FIT')
        CommonHelperFunctions.CleanFolderOfFiles(FitPath, CseFilesToDelete)

        InputPath = os.path.join(FitPath, 'input')
        InputPaths = glob.glob(InputPath)
        for EachInputPath in InputPaths:
            shutil.rmtree(EachInputPath, ignore_errors=True)


    logger.info('Cleaning Directories...')
    DirectoriesToRemove = [
                           RootFolder.GenAbsPath('Build'),
                           RootFolder.GenAbsPath('BuildFsp'),
                           RootFolder.GenAbsPath('Conf/.cache'),
                           RootFolder.GenAbsPath('Stitch/Fota/GenCapsule/tmp/')
                          ]

    # If only delete intermediate build files, then do not clean the build directories
    # !!!! NOTE !!!! Does this include BuildFsp? What about other directories?
    if IntermediateOnly:
        DirectoriesToRemove.remove(RootFolder.GenAbsPath('Build'))
        DirectoriesToRemove.remove(RootFolder.GenAbsPath('BuildFsp'))

    for EachDirectory in DirectoriesToRemove:
        logger.debug("Removing " + EachDirectory)
        # shutil.rmtree will remove an entire directory tree. It will throw up an error if pointed
        # to a directory that does not exist, so we turn off errors. !!! NOTE !!! It will not remove read-only files
        shutil.rmtree(EachDirectory, ignore_errors=True)

    logger.info("Cleaning remaining files...")

    RemainingFilesToRemove = ['*.map',
                              '*.ROM',               # !!! NOTE !!! cln.bat deletes this, so does not fall under -bin?
                              'FlashMap.h',
                              OsPath('Stitch/*.log'),
                              OsPath('Stitch/MD5_For_IAFW.txt'),
                              OsPath('Stitch/SMIP/ftool/*.log'),
                              OsPath('Silicon/BroxtonSiPkg/Include/SmipGenerated.h'),
                              OsPath('Stitch/Fota/*.fv'),
                              OsPath('Stitch/Fota/GenCapsule/*.txt')
                              ]

    CommonHelperFunctions.CleanFolderOfFiles(os.getcwd(), RemainingFilesToRemove)

    if BinaryDelete:
        StitchFolder = RootFolder.GenAbsPath('Stitch')
        BinaryFilesToDelete = ['*.bin', '*.zip']
        CommonHelperFunctions.CleanFolderOfFiles(StitchFolder, BinaryFilesToDelete)

    logger.info('All done...')

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-intm", help="Delete Intermediate Build Files Only", action='store_true')
    parser.add_argument("-bin", help="Delete .zip and .bin binary files from the /Stitch folder", action='store_true')
    Args = parser.parse_args()
    RootDirectory = CommonHelperFunctions.CurrentDirectory(os.path.dirname(os.path.realpath(__file__)))
    Run(RootDirectory, Args.intm, Args.bin)


