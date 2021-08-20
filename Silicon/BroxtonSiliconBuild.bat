@ECHO OFF
REM @file
REM
REM @copyright
REM Copyright (c) 2016 Intel Corporation. All rights reserved
REM This software and associated documentation (if any) is furnished
REM under a license and may only be used or copied in accordance
REM with the terms of the license. Except as permitted by the
REM license, no part of this software or documentation may be
REM reproduced, stored in a retrieval system, or transmitted in any
REM form or by any means without the express written consent of
REM Intel Corporation.
REM This file contains an 'Intel Peripheral Driver' and is uniquely
REM identified as "Intel Reference Module" and is licensed for Intel
REM CPUs and chipsets under the terms of your license agreement with
REM Intel or your vendor. This file may be modified by the user, subject
REM to additional terms of the license agreement.
REM
REM @par Specification Reference:

if /i "%~1"=="" goto Usage
if /i "%~1"=="/?" goto Usage
::Build Flags
if /i "%~1"=="APLK" (
    set Build_Flags=APLK
)
if /i "%~1"=="BXTM" (
    set Build_Flags=BXTM
)

@call edksetup.bat
@ECHO OFF

@if not defined TOOL_CHAIN_TAG (
  if defined VS120COMNTOOLS (
    echo.
    echo Prebuild:  Set the VS2013 environment.
    echo.
    if not defined VSINSTALLDIR call "%VS120COMNTOOLS%\vsvars32.bat"
    if /I "%VS120COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 12.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2013
    ) else (
      set TOOL_CHAIN_TAG=VS2013x86
    )
  )
)

@if not defined TOOL_CHAIN_TAG (
  if defined VS110COMNTOOLS (
    echo.
    echo Prebuild:  Set the VS2012 environment.
    echo.
    if not defined VSINSTALLDIR call "%VS110COMNTOOLS%\vsvars32.bat"
    if /I "%VS110COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 11.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2012
    ) else (
      set TOOL_CHAIN_TAG=VS2012x86
    )
  )
)

@if not defined TOOL_CHAIN_TAG (
  if defined VS100COMNTOOLS (
    echo.
    echo Prebuild:  Set the VS2010 environment.
    echo.
    if not defined VSINSTALLDIR call "%VS100COMNTOOLS%\vsvars32.bat"
    if /I "%VS100COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 10.0\Common7\Tools\" (
      set TOOL_CHAIN_TAG=VS2010
    ) else (
      set TOOL_CHAIN_TAG=VS2010x86
    )
  )
)

@if not defined TOOL_CHAIN_TAG (
  if defined VS90COMNTOOLS (
    echo.
    echo Prebuild:  Set the VS2008 environment.
    echo.
  if not defined VSINSTALLDIR call "%VS90COMNTOOLS%\vsvars32.bat"
  if /I "%VS90COMNTOOLS%" == "C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools\" (
    set TOOL_CHAIN_TAG=VS2008
  ) else (
    set TOOL_CHAIN_TAG=VS2008x86
  )
  )
)



set TARGET=RELEASE

@echo.
@echo Set build environment.
@echo.
@if not exist Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG% (
  mkdir Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%
)

if /i "%Build_Flags%" == "APLK" (
@echo ACTIVE_PLATFORM = Silicon/BroxtonSiPkg/BroxtonSiPkgBxtP.dsc >> Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
)
if /i "%Build_Flags%" == "BXTM" (
@echo ACTIVE_PLATFORM = Silicon/BroxtonSiPkg/BroxtonSiPkgBxtM.dsc >> Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
)
@echo TARGET          = %TARGET%                                  >> Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo TARGET_ARCH     = IA32 X64                                  >> Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo TOOL_CHAIN_TAG  = %TOOL_CHAIN_TAG%                          >> Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@echo BUILD_RULE_CONF = Conf/build_rule.txt                       >> Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt
@move /Y Build\BroxtonSiPkg\%TARGET%_%TOOL_CHAIN_TAG%\target.txt Conf

move /Y Conf\target.txt.tmp Conf\target.txt >nul
echo *_VS2008x86_*_ASL_PATH = %WORKSPACE%\BpCommonPkg\Tools\Iasl\iasl.exe >> Conf\tools_def.txt
echo *_VS2010x86_*_ASL_PATH = %WORKSPACE%\BpCommonPkg\Tools\Iasl\iasl.exe >> Conf\tools_def.txt
echo *_VS2012x86_*_ASL_PATH = %WORKSPACE%\BpCommonPkg\Tools\Iasl\iasl.exe >> Conf\tools_def.txt
echo *_VS2013x86_*_ASL_PATH = %WORKSPACE%\BpCommonPkg\Tools\Iasl\iasl.exe >> Conf\tools_def.txt

goto BuildRcStart

:Usage
echo Script to build APLK/BXTM RcPkg.
echo.
echo Usage: BroxtonSiliconBuild.bat [options]
echo.
echo.    /?     Display this help text
echo.
echo     APLK - Netbook/Desktop
echo     BXTM - Phablet build for BXT-0
echo     Build Targets:    Release
echo.
echo     Sample:
echo     BroxtonSiliconBuild.bat APLK
echo.
set exitCode=1

:Exit
exit /b %exitCode%

:BuildRcStart
build
