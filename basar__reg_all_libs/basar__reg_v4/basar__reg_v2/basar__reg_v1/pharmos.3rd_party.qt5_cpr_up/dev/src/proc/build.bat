@echo off

set proc_dir=%CD%\proc

cmd /c %proc_dir%\build-qtbase.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\build-qttools.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\build-qttranslations.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\build-qtserialport.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

REM cmd /c %proc_dir%\build-qtftp.bat %*
REM if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

cmd /c %proc_dir%\build-qtactive.bat %*
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%
