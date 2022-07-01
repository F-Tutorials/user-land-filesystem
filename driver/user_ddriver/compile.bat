rm -rf build lib
mkdir build
mkdir lib

type NUL > build/ddriver
type NUL > build/ddriver_log

@REM Generate Path
set CUR_PATH=%~dp0
set CUR_PATH=\"%CUR_PATH:\=/%\"
powershell -Command "(Get-Content ddriver.c -encoding utf8) -replace '\$CURPATH', '%CUR_PATH%' | Out-File -encoding utf8 ddriver.c"

@REM Compile
gcc -c ddriver.c 
ar -cr libddriver.a ddriver.o

@REM Clean
rm ddriver.o
mv libddriver.a lib