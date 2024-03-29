@REM ## @file
@REM
@REM #@copyright
@REM #  Copyright (c) 2008 - 2016 Intel Corporation All rights reserved
@REM #  This software and associated documentation (if any) is furnished
@REM #  under a license and may only be used or copied in accordance
@REM #  with the terms of the license. Except as permitted by such
@REM #  license, no part of this software or documentation may be
@REM #  reproduced, stored in a retrieval system, or transmitted in any
@REM #  form or by any means without the express written consent of
@REM #  Intel Corporation.
@REM #  This file contains an 'Sample Driver' and is
@REM #  licensed for Intel CPUs and chipsets under the terms of your
@REM #  license agreement with Intel or your vendor.  This file may
@REM #  be modified by the user, subject to additional terms of the
@REM #  license agreement
@REM ##
@echo off
set FSP_VER=2
set FSP_BASENAME=Broxton
set FSP_PKG_NAME=BroxtonFspPkg
set FSP_BIN_PKG_NAME=BroxtonFspBinPkg
@if /I "%1"=="/h" goto Usage
@if /I "%1"=="/?" goto Usage


@echo off
set MISC_FLAGS=-DFSP_FLAG %2
set FSP_BD_COMMON=-p %FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc %MISC_FLAGS% -a IA32 -n 4 -t %TOOL_CHAIN_TAG% -Y PCD -Y LIBRARY

if /I "%1"=="/clean" goto Clean
if /I "%1"=="/r" goto ReleaseBuild
if /I "%1"=="/tr" goto ReleaseTypeTest
if /I "%1"=="/d" goto DebugBuild

if /I "%1"=="" (
  goto DebugBuild
) else (
  echo.
  echo  ERROR: "%1" is not valid parameter.
  goto Usage
)

:Clean
echo Removing Build and Conf directories ...
if exist Build rmdir Build /s /q
if exist Conf  rmdir Conf  /s /q
if exist *.log  del *.log /q /f
set WORKSPACE=
set EDK_TOOLS_PATH=
goto End

:ReleaseTypeTest
set  BD_TARGET=RELEASE
set  BD_MACRO=%MISC_FLAGS%
set  BD_ARGS=%FSP_BD_COMMON% -b RELEASE %BD_MACRO% -y ReportRelease.log
set  FSP_BUILD_TYPE=0x0001
set  FSP_RELEASE_TYPE=0x0000
goto Build

:ReleaseBuild
set  BD_TARGET=RELEASE
set  BD_MACRO=%MISC_FLAGS%
set  BD_ARGS=%FSP_BD_COMMON% -b RELEASE %BD_MACRO% -y ReportRelease.log
set  FSP_BUILD_TYPE=0x0001
set  FSP_RELEASE_TYPE=0x0002
goto Build

:DebugBuild
set  BD_TARGET=DEBUG
set  BD_MACRO=%MISC_FLAGS%
set  BD_ARGS=%FSP_BD_COMMON% -b DEBUG  %BD_MACRO% -y ReportDebug.log
set  FSP_BUILD_TYPE=0x0000
set  FSP_RELEASE_TYPE=0x0000
goto Build


:Build

echo PREBUILD
build  -m %FSP_PKG_NAME%\FspHeader\FspHeader.inf %BD_ARGS% -DCFG_PREBUILD
if ERRORLEVEL 1 exit /b 1
call :PreBuild  CALL_RET
if "%CALL_RET%"=="1" exit /b 1
echo BUILD
build  %BD_ARGS%
if ERRORLEVEL 1 exit /b 1
echo POSTBUILD
call :PostBuild
goto End

:Usage
echo.
echo  Usage: "%0 [/h | /? | /r | /tr | /d | /clean]"
echo.
goto End

:PreBuild
echo Start of PreBuild ...

set %~1=1
echo Copy required files

copy /y %FSP_PKG_NAME%\Include\SmipGenerated.h Silicon\BroxtonSiPkg\Include\SmipGenerated.h
copy /y %FSP_PKG_NAME%\Override\BaseTools\Bin\Win32\nasm.exe BaseTools\Bin\Win32\nasm.exe

@REM @todo update BPDG.exe tool to use FSP_C_UPD_GUID
set TOOL_MACRO=%BD_MACRO% -DFSP_VER=%FSP_VER%
set FSP_T_UPD_GUID=34686CA3-34F9-4901-B82A-BA630F0714C6
set FSP_M_UPD_GUID=39A250DB-E465-4DD1-A2AC-E2BD3C0E2385
set FSP_S_UPD_GUID=CAE3605B-5B34-4C85-B3D7-27D54273C40F
python %WORKSPACE%/IntelFsp2Pkg/Tools/GenCfgOpt.py UPDTXT ^
     %FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc ^
     Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     %TOOL_MACRO%
if "%ERRORLEVEL%"=="256" (
  REM  DSC is not changed, no need to recreate MAP and BIN file
) else (
  if ERRORLEVEL 1 goto:PreBuildFail
  echo UPD TXT file was generated successfully !

  echo Generate UPD Header File ...
  for %%A in (%FSP_T_UPD_GUID% %FSP_M_UPD_GUID% %FSP_S_UPD_GUID%) do (
    echo %%A
    del /q /f Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%%A.bin ^
            Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%%A.map 2>nul
    BaseTools\Bin\Win32\BPDG.exe ^
       Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%%%A.txt ^
       -o Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%%A.bin ^
       -m Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\%%A.map
    if ERRORLEVEL 1 goto:PreBuildFail
  )
)

python %WORKSPACE%/IntelFsp2Pkg/Tools/GenCfgOpt.py HEADER ^
         %FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc ^
         Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
         %FSP_PKG_NAME%\Include\BootLoaderPlatformData.h ^
         %TOOL_MACRO%
if "%ERRORLEVEL%"=="256" (
    REM  No need to recreate header file
) else (
    if ERRORLEVEL 1 goto:PreBuildFail
    echo Upd header file was generated successfully !

    echo Generate BSF File ...
    if exist %FSP_BIN_PKG_NAME%\Fsp.bsf attrib -r %FSP_BIN_PKG_NAME%\Fsp.bsf
    python %WORKSPACE%/IntelFsp2Pkg/Tools/GenCfgOpt.py GENBSF ^
         %FSP_PKG_NAME%\%FSP_PKG_NAME%.dsc ^
         Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
         %FSP_BIN_PKG_NAME%\Fsp.bsf ^
         %TOOL_MACRO%

    if ERRORLEVEL 1 goto:PreBuildFail
    echo BSF file was generated successfully !

    if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h" (
      attrib -r %FSP_PKG_NAME%\Include\FspUpd.h
      copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h %FSP_PKG_NAME%\Include\FspUpd.h
      )
    if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h" (
      attrib -r %FSP_PKG_NAME%\Include\FsptUpd.h
      copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h %FSP_PKG_NAME%\Include\FsptUpd.h
      )
    if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h" (
      attrib -r %FSP_PKG_NAME%\Include\FspmUpd.h
      copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h %FSP_PKG_NAME%\Include\FspmUpd.h
      )
    if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h" (
      attrib -r %FSP_PKG_NAME%\Include\FspsUpd.h
      copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h %FSP_PKG_NAME%\Include\FspsUpd.h
      )

)

:PreBuildRet
set %~1=0
echo End of PreBuild ...
echo.
goto:EOF

:PreBuildFail
del /q /f Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h
del /q /f Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h
del /q /f Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h
del /q /f Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h

echo.
(goto) 2>nul & endlocal & exit /b 1
goto:EOF

:PostBuild
echo Start of PostBuild ...

echo Patch FSP-T Image ...
python %WORKSPACE%\IntelFsp2Pkg\Tools\PatchFv.py ^
     Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     FSP-T:%FSP_BASENAME%  ^
     "0x0000,            _BASE_FSP-T_,                                                                                       @Temporary Base" ^
     "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-T Size" ^
     "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-T Base" ^
     "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-T Image Attribute" ^
     "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x1000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-T Component Attribute" ^
     "<[0x0000]>+0x00B8, 70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x1C - <[0x0000]>,                                             @FSP-T CFG Offset" ^
     "<[0x0000]>+0x00BC, [70BCF6A5-FFB1-47D8-B1AE-EFE5508E23EA:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-T CFG Size" ^
     "<[0x0000]>+0x00C4, FspSecCoreT:_TempRamInitApi - [0x0000],                                                             @TempRamInit API" ^
     "0x0000,            0x00000000,                                                                                         @Restore the value" ^
     "FspSecCoreT:_FspInfoHeaderRelativeOff, FspSecCoreT:_AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-T Header Offset"
if ERRORLEVEL 1 goto:PreBuildFail

echo Patch FSP-M Image ...
python %WORKSPACE%\IntelFsp2Pkg\Tools\PatchFv.py ^
     Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     FSP-M:%FSP_BASENAME%  ^
     "0x0000,            _BASE_FSP-M_,                                                                                       @Temporary Base" ^
     "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-M Size" ^
     "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-M Base" ^
     "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-M Image Attribute" ^
     "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x2000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-M Component Attribute" ^
     "<[0x0000]>+0x00B8, D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x1C - <[0x0000]>,                                             @FSP-M CFG Offset" ^
     "<[0x0000]>+0x00BC, [D5B86AEA-6AF7-40D4-8014-982301BC3D89:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-M CFG Size" ^
     "<[0x0000]>+0x00D0, FspSecCoreM:_FspMemoryInitApi - [0x0000],                                                           @MemoryInitApi API" ^
     "<[0x0000]>+0x00D4, FspSecCoreM:_TempRamExitApi - [0x0000],                                                             @TempRamExit API" ^
     "FspSecCoreM:_FspPeiCoreEntryOff, PeiCore:__ModuleEntryPoint - [0x0000],                                                @PeiCore Entry" ^
     "0x0000,            0x00000000,                                                                                         @Restore the value" ^
     "FspSecCoreM:_FspInfoHeaderRelativeOff, FspSecCoreM:_AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-M Header Offset"
if ERRORLEVEL 1 goto:PreBuildFail

echo Patch FSP-S Image ...
python %WORKSPACE%\IntelFsp2Pkg\Tools\PatchFv.py ^
     Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV ^
     FSP-S:%FSP_BASENAME%  ^
     "0x0000,            _BASE_FSP-S_,                                                                                       @Temporary Base" ^
     "<[0x0000]>+0x00AC, [<[0x0000]>+0x0020],                                                                                @FSP-S Size" ^
     "<[0x0000]>+0x00B0, [0x0000],                                                                                           @FSP-S Base" ^
     "<[0x0000]>+0x00B4, ([<[0x0000]>+0x00B4] & 0xFFFFFFFF) | 0x0001,                                                        @FSP-S Image Attribute" ^
     "<[0x0000]>+0x00B6, ([<[0x0000]>+0x00B6] & 0xFFFF0FFC) | 0x3000 | %FSP_BUILD_TYPE% | %FSP_RELEASE_TYPE%,                @FSP-S Component Attribute" ^
     "<[0x0000]>+0x00B8, E3CD9B18-998C-4F76-B65E-98B154E5446F:0x1C - <[0x0000]>,                                             @FSP-S CFG Offset" ^
     "<[0x0000]>+0x00BC, [E3CD9B18-998C-4F76-B65E-98B154E5446F:0x14] & 0xFFFFFF - 0x001C,                                    @FSP-S CFG Size" ^
     "<[0x0000]>+0x00D8, FspSecCoreS:_FspSiliconInitApi - [0x0000],                                                          @SiliconInit API" ^
     "<[0x0000]>+0x00CC, FspSecCoreS:_NotifyPhaseApi - [0x0000],                                                             @NotifyPhase API" ^
     "0x0000,            0x00000000,                                                                                         @Restore the value" ^
     "FspSecCoreS:_FspInfoHeaderRelativeOff, FspSecCoreS:_AsmGetFspInfoHeader - {912740BE-2284-4734-B971-84B027353F0C:0x1C}, @FSP-S Header Offset"
if ERRORLEVEL 1 goto:PreBuildFail

echo Copy Fsp images and FVs to \BuildFsp

if not exist %WORKSPACE%\BuildFsp @mkdir %WORKSPACE%\BuildFsp
copy /y /b Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\BROXTON.fd %WORKSPACE%\BuildFsp\BROXTON_FSP.fd
copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h %WORKSPACE%\BuildFsp\FspUpd.h
copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h %WORKSPACE%\BuildFsp\FsptUpd.h
copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h %WORKSPACE%\BuildFsp\FspmUpd.h
copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h %WORKSPACE%\BuildFsp\FspsUpd.h
if exist %WORKSPACE%\%FSP_BIN_PKG_NAME%\Fsp.bsf attrib -r %WORKSPACE%\%FSP_BIN_PKG_NAME%\Fsp.bsf
copy /y %FSP_BIN_PKG_NAME%\Fsp.bsf %WORKSPACE%\BuildFsp\BROXTON_FSP.bsf

echo Copy FSP image to %FSP_BIN_PKG_NAME%
if exist %WORKSPACE%\%FSP_BIN_PKG_NAME%\Fsp.fd attrib -r %WORKSPACE%\%FSP_BIN_PKG_NAME%\Fsp.fd
copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\BROXTON.fd %WORKSPACE%\%FSP_BIN_PKG_NAME%\Fsp.fd

if exist %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FspUpd.h attrib -r %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FspUpd.h
if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h" (
  copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspUpd.h %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FsptUpd.h attrib -r %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FsptUpd.h
if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h" (
  copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FsptUpd.h %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FspmUpd.h attrib -r %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FspmUpd.h
if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h" (
  copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspmUpd.h %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\
  )
if exist %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FspsUpd.h attrib -r %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\FspsUpd.h
if exist "Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h" (
  copy /y Build\%FSP_PKG_NAME%\%BD_TARGET%_%TOOL_CHAIN_TAG%\FV\FspsUpd.h %WORKSPACE%\%FSP_BIN_PKG_NAME%\Include\
  )

echo off

echo Patch is DONE

goto:EOF

:End
echo.
