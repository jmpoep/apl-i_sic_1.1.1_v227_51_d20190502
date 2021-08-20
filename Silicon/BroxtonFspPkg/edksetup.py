##
# 
# @package edksetup
# Sets the enviroment variables necessary for the EDK2 build to work.
# Copies fresh templates of the EDK2 build core configuration files:
# target.txt, tools_def.txt, build_rule.txt.
# Updates the system's PATH variable with the directories of the EDK2 common tools.
# 
# THIS FILE MUST EXIST IN THE ROOT DIRECTORY OF THE DESIRED WORKSPACE.
# It uses it's own file path to construct the paths stored in environment variable
# so that the scripts can be called from any directory. This is chosen because
# getting the current working directory would cause errors if the script
# was invoked in a different directory, decreasing automation friendliness.
# This way, automated scripts do not have to change their working directory
# to build.
# CAVEAT: MOVING THE LOCATION OF THIS FILE WILL BREAK THE BUILD.
#         Also, attempting to use this workspace's build scripts on another
#         workspace will also not work.
# 
# 
# Copyright (c) 2006 - 2015, Intel Corporation. All rights reserved.<BR>
# This program and the accompanying materials
# are licensed and made available under the terms and conditions of the BSD License
# which accompanies this distribution.  The full text of the license may be found at
# http://opensource.org/licenses/bsd-license.php
# 
# THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
# WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
# 

import os
import sys
import shutil
import platform
import argparse
EdkWorkspace = os.path.dirname(os.path.realpath(__file__))
sys.path.append(os.path.join(EdkWorkspace, 'BuildScripts'))
import CommonHelperFunctions

# This file contains the essential parts of edksetup.bat and BaseTools/toolsetup.bat
# Omitted all support for edksetup arguments (--nt32, etc) and a bunch of other stuff

# Omitted visual studio and gcc checks, will probably add gcc later
def Run(CodeCoveragePath=False, Reconfig= True):
    OS = platform.system()

    WorkSpace = CommonHelperFunctions.CurrentDirectory(os.path.dirname(os.path.realpath(__file__)))
    os.environ['EFI_SOURCE'] = WorkSpace()
    os.environ['EDK_SOURCE'] = WorkSpace()

    if not os.environ.get('WORKSPACE'):
        #-----toolsetup.bat code start-----
        # Set the PATH and EDK_TOOLS_PATH environment variables. Kernel looks at each folder starting from the first one
        # so therefore pushing paths to the start of the PATH enviroment variable ensures they will be used.
        # "Additionally, the provided EDK II tool set must be present in a directory that is in the system environment
        # variable: PATH."
        # - (EDK II Build Spec. 1.24A, Ch 4.2 Build Process Overview, Pg 64)
        os.environ['EDK_TOOLS_PATH'] = WorkSpace() + r'/BaseTools'
        OsSep = os.pathsep
        if OS == 'Linux':
          OldEnvironPath = os.environ['PATH']
          os.environ['PATH'] = WorkSpace.GenAbsPath('BaseTools/BinWrappers/PosixLike') + OsSep + \
                               WorkSpace.GenAbsPath('BaseTools/Source') + OsSep + \
                               WorkSpace.GenAbsPath('BaseTools/Source/Python') + OsSep + \
                               WorkSpace.GenAbsPath('BaseTools/Source/Python/Trim') + OsSep + \
                               OldEnvironPath
          os.environ['WORKSPACE'] = WorkSpace()
        else:
          OldEnvironPath = os.environ['PATH']
          os.environ['PATH'] = WorkSpace.GenAbsPath('BaseTools\Bin') + OsSep + \
                               WorkSpace.GenAbsPath(r'BaseTools\Bin\Win32') + OsSep + \
                               OldEnvironPath
          os.environ['WORKSPACE_TOOLS_PATH'] = WorkSpace.GenAbsPath('BaseTools')
          os.environ['WORKSPACE'] = WorkSpace()

          if isinstance(CodeCoveragePath, str):
              print('Enabling Code Coverage toolset...')
              if os.path.isdir(CodeCoveragePath):
                  CodeCoverageFolder = CommonHelperFunctions.CurrentDirectory(CodeCoveragePath)
                  OldEnvironPath = os.environ['PATH']
                  os.environ['PATH'] = CodeCoverageFolder.GenAbsPath('Build\Win32') + OsSep + \
                                       CodeCoverageFolder.GenAbsPath('Tools\CodeCoverageTool\Bin') + OsSep + \
                                       CodeCoverageFolder.GenAbsPath('tools\CodeCoverageTool\ItsCovAnalysisTool') + OsSep + \
                                       OldEnvironPath
              else:
                  raise CommonHelperFunctions.InvalidSetupError('Cannot find Code Coverage Package at ' + CodeCoveragePath)

    if Reconfig:
        if not os.path.exists('Conf'):
	  	os.makedirs('Conf')# Copy target, tools_def, and build_rule templates into Conf/
        try:
          RootDirectory = CommonHelperFunctions.CurrentDirectory(os.environ['WORKSPACE'])
          shutil.copyfile(
                RootDirectory.GenAbsPath('BaseTools/Conf/target.template'),
                RootDirectory.GenAbsPath('Conf/target.txt')
               )
                
          shutil.copyfile(
                RootDirectory.GenAbsPath('Platform/DeviceCommonPkg/Override/BaseTools/Conf/tools_def.template'),
                RootDirectory.GenAbsPath('Conf/tools_def.txt')
               )
          shutil.copyfile('BaseTools/Conf/build_rule.template', 'Conf/build_rule.txt')
          #-----toolsetup.bat code end-----
        except IOError, ErrorString:
          sys.exit(ErrorString)

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
                        '-CodeCoverage',
                        help='Use the Code Coverage build tools instead of the default.'
                        )
    parser.add_argument(
                        '-Reconfig',
                        action= 'store_true',
                        help='Copy fresh EDK Build configuration files into the /Conf directory.'
                       )
    args = parser.parse_args()
    Run(args.CodeCoverage)
