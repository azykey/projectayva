@echo off
REM Propriedade Intelectual: Adilson Oliveira 2025
REM Build automático do projeto AdilsonGUI (Windows)
setlocal
if not exist build mkdir build
g++ -std=c++17 -Wall -Iinclude -c src\adilsongui.cpp -o build\adilsongui.o
g++ -std=c++17 -Wall -Iinclude -c src\builder.cpp -o build\builder.o
g++ -std=c++17 -Wall -Iinclude -c src\myjson.cpp -o build\myjson.o
g++ -std=c++17 -Wall -Iinclude -c src\utils.cpp -o build\utils.o
g++ -std=c++17 -Wall -Iinclude exemplo\main.cpp build\adilsongui.o build\builder.o build\myjson.o build\utils.o -o build\exemplo.exe -lgdi32
g++ -std=c++17 -Wall -Iinclude src\builder.cpp src\myjson.cpp src\utils.cpp -o build\builder.exe -lssl -lcrypto
echo Build concluído! Veja os executáveis em build\
endlocal
pause 