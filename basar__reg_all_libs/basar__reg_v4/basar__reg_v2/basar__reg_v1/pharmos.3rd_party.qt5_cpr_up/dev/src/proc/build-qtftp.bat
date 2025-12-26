@echo off

set proc_dir=%CD%\proc

call %proc_dir%\build-common.inc.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

pushd %CD%\qtftp*
    set ftp_src_dir=%CD%
popd
set ftp_build_dir=%CD%\build\qtftp

:qtftp
mkdir %ftp_build_dir% >NUL 2>&1
pushd %ftp_build_dir%

echo configuring qtftp

%QMAKE% %ftp_src_dir%\qtftp.pro
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo building qtftp

nmake
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo installing qtftp

nmake install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

rem header files seem not to be installed automatically?
xcopy /Q /Y /D %ftp_src_dir%\src\qftp\*.h   %install_dir%\include\QtFtp\

rem copy convenience header files
xcopy /Q /Y /E /S /D %src_dir%\include\QtFtp     %install_dir%\include\QtFtp\
xcopy /Q /Y /E /S /D %src_dir%\include\QtNetwork %install_dir%\include\QtNetwork\

echo done
echo.

popd
:qtftp_end
