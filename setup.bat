del setup.exe
echo Compiling...
g++ -std=c++14 -Wall -Wextra Setup.cpp Matrix.cpp LinearProgramming.cpp -o setup
echo Finished Compiling!
setup.exe
pause