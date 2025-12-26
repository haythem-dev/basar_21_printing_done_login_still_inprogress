@echo off

set proc_dir=%CD%\proc

call %proc_dir%\build-common.inc.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

pushd %CD%\qtactiveqt*
    set qtactive_src_dir=%CD%
popd
set qtactive_build_dir=%CD%\build\qtactive

rem if exist %install_dir%\bin\designer.exe goto qttools_end
:qtactive
mkdir %qtactive_build_dir% >NUL 2>&1
pushd %qtactive_build_dir%

echo configuring qtactive

%QMAKE% %qtactive_src_dir%\qtactiveqt.pro
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo building qtactive

nmake
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo installing qtactive

nmake install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

popd
:qttools_end