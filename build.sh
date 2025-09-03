#!/bin/bash
# Propriedade Intelectual: Adilson Oliveira 2025
# Build automático do projeto AdilsonGUI (Linux)
set -e
mkdir -p build
g++ -std=c++17 -Wall -Iinclude -c src/adilsongui.cpp -o build/adilsongui.o
g++ -std=c++17 -Wall -Iinclude -c src/builder.cpp -o build/builder.o
g++ -std=c++17 -Wall -Iinclude -c src/myjson.cpp -o build/myjson.o
g++ -std=c++17 -Wall -Iinclude -c src/utils.cpp -o build/utils.o
g++ -std=c++17 -Wall -Iinclude exemplo/main.cpp build/adilsongui.o build/builder.o build/myjson.o build/utils.o -o build/exemplo -lX11
g++ -std=c++17 -Wall -Iinclude src/builder.cpp src/myjson.cpp src/utils.cpp -o build/builder -lssl -lcrypto
echo "Build concluído! Veja os executáveis em build/" 