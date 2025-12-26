@echo off

set dev_dir=%CD%\..

:qtserialport
echo extracting qtserialport sources to %dev_dir%\src...

7z.exe ^
	x ^
	-y ^
	-o%dev_dir%\src ^
	%dev_dir%\ext\qt5serialport_source\qtserialport*.zip
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.
