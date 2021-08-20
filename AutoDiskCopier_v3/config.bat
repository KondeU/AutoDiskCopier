:: Manually configure the local environment.

PATH=D:\CMake\cmake-3.16.0-win64-x64\bin;%PATH%
PATH=D:\CMake\cmake-3.19.8-win64-x64\bin;%PATH%
echo Env PATH: %PATH%

:: The version of CMake finally used:
cmake --version

:: The default build uses VS2015, if we have a higher version of VS,
:: use the higher version of MSBuild to build, then we need to add
:: the path of MSBuild.exe to the environment PATH.
:: And we need to install vc140 compatible compile and build tools.
PATH=D:\VS\2017\Enterprise\MSBuild\15.0\Bin;%PATH%
