#!/bin/bash
# Propriedade Intelectual: Adilson Oliveira 2025
# Build automático do projeto AdilsonGUI (Linux X11)
set -e

echo "=== AdilsonGUI Linux Build ==="
echo "Propriedade Intelectual: Adilson Oliveira 2025"
echo "Sistema: Linux X11 + Cairo"
echo "================================"

# Verificar dependências
echo "Verificando dependências..."
if ! pkg-config --exists x11; then
    echo "Erro: libx11-dev não encontrada. Instale com: sudo apt install libx11-dev"
    exit 1
fi

if ! pkg-config --exists cairo; then
    echo "Erro: libcairo2-dev não encontrada. Instale com: sudo apt install libcairo2-dev"
    exit 1
fi

if ! pkg-config --exists openssl; then
    echo "Erro: libssl-dev não encontrada. Instale com: sudo apt install libssl-dev"
    exit 1
fi

echo "Dependências OK!"

# Criar diretório build
mkdir -p build

# Compilar módulos principais
echo "Compilando módulos..."
g++ -std=c++17 -Wall -Iinclude -c src/adilsongui_linux.cpp -o build/adilsongui_linux.o \
    $(pkg-config --cflags x11 cairo)

g++ -std=c++17 -Wall -Iinclude -c src/builder.cpp -o build/builder.o \
    $(pkg-config --cflags openssl)

g++ -std=c++17 -Wall -Iinclude -c src/myjson.cpp -o build/myjson.o

g++ -std=c++17 -Wall -Iinclude -c src/utils.cpp -o build/utils.o \
    $(pkg-config --cflags openssl)

# Compilar exemplo Linux
echo "Compilando exemplo Linux..."
g++ -std=c++17 -Wall -Iinclude exemplo/exemplo_linux.cpp \
    build/adilsongui_linux.o build/builder.o build/myjson.o build/utils.o \
    -o build/exemplo_linux \
    $(pkg-config --libs x11 cairo openssl)

# Compilar builder
echo "Compilando builder..."
g++ -std=c++17 -Wall -Iinclude src/builder.cpp src/myjson.cpp src/utils.cpp \
    -o build/builder \
    $(pkg-config --libs openssl)

# Tornar executável
chmod +x build/exemplo_linux
chmod +x build/builder

echo "================================"
echo "Build concluído com sucesso!"
echo "Executáveis criados:"
echo "  - build/exemplo_linux (GUI X11)"
echo "  - build/builder (sistema de build)"
echo ""
echo "Para executar:"
echo "  ./build/exemplo_linux"
echo ""
echo "Para ver dependências:"
echo "  ldd build/exemplo_linux"
echo "================================" 