@echo off

set proc_dir=%CD%\proc

call %proc_dir%\build-common.inc.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

pushd %CD%\qttools*
    set tools_src_dir=%CD%
popd
set tools_build_dir=%CD%\build\qttools

rem if exist %install_dir%\bin\designer.exe goto qttools_end
:qttools
mkdir %tools_build_dir% >NUL 2>&1
pushd %tools_build_dir%

echo configuring qttools

%QMAKE% %tools_src_dir%\qttools.pro
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo building qttools

nmake
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo installing qttools

nmake install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

popd
:qttools_end
