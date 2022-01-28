set workDir=%~dp0
set buildWinDir=%workDir%buildwin\
call %buildWinDir%configure.bat not_copy_depends

if not exist msvs2017_cmake (
    mkdir msvs2017_cmake
)

cd msvs2017_cmake
cmake ..\ -G "Visual Studio 15 2017" -A Win32 -DTarget=Windows -DArch=Win32 -DQT_DIR=F:\qt\Qt5.12.10\5.12.10\msvc2017
cd ..\
pause