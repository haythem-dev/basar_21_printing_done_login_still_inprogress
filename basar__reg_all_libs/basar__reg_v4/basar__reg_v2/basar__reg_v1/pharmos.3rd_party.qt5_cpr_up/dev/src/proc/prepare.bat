@echo off

set proc_dir=%CD%\proc

cmd /c %proc_dir%\prepare-qtbase.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\prepare-qttools.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\prepare-qttranslations.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\prepare-qtserialport.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM cmd /c %proc_dir%\prepare-qtftp.bat
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\prepare-qtactive.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
