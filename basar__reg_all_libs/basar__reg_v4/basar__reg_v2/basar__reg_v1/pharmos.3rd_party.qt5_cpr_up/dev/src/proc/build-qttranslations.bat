@echo off

set proc_dir=%CD%\proc

call %proc_dir%\build-common.inc.bat
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

pushd %CD%\qttranslations*
    set translations_src_dir=%CD%
popd
set translations_build_dir=%CD%\build\qttranslations

rem if exist %install_dir%\translations\qt_de.qm goto qttranslations_end
:qttranslations
mkdir %translations_build_dir% >NUL 2>&1
pushd %translations_build_dir%

echo configuring qttranslations

%QMAKE% %translations_src_dir%\qttranslations.pro
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo building qttranslations

nmake
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

echo installing qttranslations

nmake install
if %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%

echo done
echo.

popd
:qttranslations_end
