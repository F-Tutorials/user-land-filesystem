rm -rf build lib
mkdir build
mkdir lib

type NUL > build/ddrivercyg
type NUL > build/ddrivercyg_log

@REM Generate Path
set CUR_PATH=%~dp0
set CUR_PATH=\"%CUR_PATH:\=/%\"
powershell -Command "(Get-Content ddrivercyg.c -encoding utf8) -replace '\$CURPATH', '%CUR_PATH%' | Out-File -encoding utf8 ddrivercyg.c"

@REM Compile
gcc -c ddrivercyg.c 
ar -cr libcygddriver.a ddrivercyg.o

@REM Clean
rm ddrivercyg.o
mv libcygddriver.a lib