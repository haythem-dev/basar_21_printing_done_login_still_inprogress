@echo off

set proc_dir=%CD%\proc

call %proc_dir%\build-common.inc.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

pushd %CD%\qtserialport*
    set serialport_src_dir=%CD%
popd
set serialport_build_dir=%CD%\build\qtserialport

rem if exist %install_dir%\bin\Qt5SerialPort.dll goto qtserialport_end
:qtserialport
mkdir %serialport_build_dir% >NUL 2>&1
pushd %serialport_build_dir%

echo configuring qtserialport

%QMAKE% %serialport_src_dir%\qtserialport.pro
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo building qtserialport

nmake
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo installing qtserialport

nmake install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

popd
:qtserialport_end
