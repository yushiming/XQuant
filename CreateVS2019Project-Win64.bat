
:: 创建编译工程目录及执行cmake编译

mkdir vs2019-build-win64
cd vs2019-build-win64
cmake ../ -G "Visual Studio 16 2019" -A x64

:: 暂停命令行界面
pause