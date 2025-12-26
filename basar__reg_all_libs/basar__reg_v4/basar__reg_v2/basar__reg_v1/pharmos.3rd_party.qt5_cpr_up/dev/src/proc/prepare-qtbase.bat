@echo off

set dev_dir=%CD%\..

:qtbase
echo extracting qtbase sources to %dev_dir%\src...

7z.exe ^
	x ^
	-y ^
	-o%dev_dir%\src ^
	%dev_dir%\ext\qt5base_source\qtbase*.zip
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo patching qtbase sources...

pushd %dev_dir%\src\qtbase*
popd

echo done
echo.
