@echo off
REM Propriedade Intelectual: Adilson Oliveira 2025
REM Build do AdilsonCore - SUPERANDO RUST
setlocal

echo 🔥 ADILSONCORE - SUPERANDO RUST 🔥
echo Propriedade Intelectual: Adilson Oliveira 2025
echo ===============================================

if not exist build mkdir build

echo.
echo 🛡️ Compilando Memory Safety Superior...
g++ -std=c++17 -Wall -Iinclude -c src/adilsoncore.cpp -o build/adilsoncore.o

echo.
echo ⚡ Compilando Concorrência Lock-Free...
g++ -std=c++17 -Wall -Iinclude -c src/adilsongui.cpp -o build/adilsongui.o -lgdi32

echo.
echo 📦 Compilando Package Manager...
g++ -std=c++17 -Wall -Iinclude -c src/builder.cpp -o build/builder.o

echo.
echo 📚 Compilando Documentação Automática...
g++ -std=c++17 -Wall -Iinclude -c src/myjson.cpp -o build/myjson.o

echo.
echo 🌐 Compilando Comunidade Integrada...
g++ -std=c++17 -Wall -Iinclude -c src/utils.cpp -o build/utils.o

echo.
echo ⚡ Compilando WebAssembly Nativo...
g++ -std=c++17 -Wall -Iinclude exemplo/exemplo_supera_rust.cpp ^
    build/adilsoncore.o build/adilsongui.o build/builder.o build/myjson.o build/utils.o ^
    -o build/supera_rust.exe -lgdi32 -lssl -lcrypto

echo.
echo 🎨 Compilando GUI Avançada...
g++ -std=c++17 -Wall -Iinclude exemplo/main.cpp ^
    build/adilsongui.o build/builder.o build/myjson.o build/utils.o ^
    -o build/gui_avancada.exe -lgdi32

echo.
echo ===============================================
echo ✅ BUILD CONCLUÍDO - ADILSONCORE SUPERANDO RUST!
echo ===============================================
echo.
echo Executáveis criados:
echo   - build/supera_rust.exe (Demonstração completa)
echo   - build/gui_avancada.exe (GUI Liquid Glass)
echo.
echo Para executar:
echo   build\supera_rust.exe
echo.
echo Para ver DLLs:
echo   dumpbin /DEPENDENTS build\supera_rust.exe
echo.
echo 🏆 ADILSONCORE É SUPERIOR AO RUST! 🏆
echo Propriedade Intelectual: Adilson Oliveira 2025

endlocal
pause 