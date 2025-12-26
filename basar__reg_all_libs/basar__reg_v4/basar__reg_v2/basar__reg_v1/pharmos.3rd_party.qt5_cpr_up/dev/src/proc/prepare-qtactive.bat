@echo off

set dev_dir=%CD%\..

:qttranslations
echo extracting qtactive sources to %dev_dir%\src...

7z.exe ^
	x ^
	-y ^
	-o%dev_dir%\src ^
	%dev_dir%\ext\qt5activeqt_source\qtactiveqt*.zip
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.