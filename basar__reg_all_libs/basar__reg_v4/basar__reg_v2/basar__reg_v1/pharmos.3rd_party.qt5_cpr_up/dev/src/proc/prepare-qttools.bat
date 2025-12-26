@echo off

set dev_dir=%CD%\..

:qttools
echo extracting qttools sources to %dev_dir%\src...

7z.exe ^
	x ^
	-y ^
	-o%dev_dir%\src ^
	%dev_dir%\ext\qt5tools_source\qttools*.zip
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.
