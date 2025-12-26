@echo off

set dev_dir=%CD%\..

:qtftp
echo extracting qtftp sources to %dev_dir%\src...

rmdir /S /Q %dev_dir%\src\qtftp*
7z.exe ^
	x ^
	-y ^
	-o%dev_dir%\src ^
	%dev_dir%\ext\qt5ftp_source\qtftp*.zip
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo patching qtftp sources...

pushd %dev_dir%\src\qtftp*
    git apply -v -p1 %dev_dir%\src\patch\win32-qftp.pro.patch
    if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
popd

echo done
echo.
