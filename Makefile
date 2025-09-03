# Propriedade Intelectual: Adilson Oliveira 2025
# Makefile Universal - AdilsonGUI (Windows + Linux)
# Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria

# Detectar sistema operacional
ifeq ($(OS),Windows_NT)
    # Windows
    CC = g++
    CFLAGS = -std=c++17 -Wall -Iinclude
    LIBS_WIN = -lgdi32
    LIBS_SSL = -lssl -lcrypto
    EXE_EXT = .exe
    OBJ_EXT = .o
    RM = del /Q
    MKDIR = if not exist
    RMDIR = rmdir /S /Q
    GUI_SRC = src/adilsongui.cpp
    EXAMPLE_SRC = exemplo/main.cpp
    EXAMPLE_EXE = build/exemplo$(EXE_EXT)
    BUILDER_EXE = build/builder$(EXE_EXT)
else
    # Linux
    CC = g++
    CFLAGS = -std=c++17 -Wall -Iinclude $(shell pkg-config --cflags x11 cairo openssl 2>/dev/null)
    LIBS_LINUX = $(shell pkg-config --libs x11 cairo 2>/dev/null)
    LIBS_SSL = $(shell pkg-config --libs openssl 2>/dev/null)
    EXE_EXT = 
    OBJ_EXT = .o
    RM = rm -f
    MKDIR = mkdir -p
    RMDIR = rm -rf
    GUI_SRC = src/adilsongui_linux.cpp
    EXAMPLE_SRC = exemplo/exemplo_linux.cpp
    EXAMPLE_EXE = build/exemplo_linux
    BUILDER_EXE = build/builder
endif

# Objetos
OBJS = build/adilsongui$(OBJ_EXT) build/builder$(OBJ_EXT) build/myjson$(OBJ_EXT) build/utils$(OBJ_EXT)

# Alvos principais
all: $(EXAMPLE_EXE) $(BUILDER_EXE)
	@echo "=== Build Concluído ==="
	@echo "Propriedade Intelectual: Adilson Oliveira 2025"
	@echo "Executáveis criados:"
	@echo "  - $(EXAMPLE_EXE)"
	@echo "  - $(BUILDER_EXE)"

# Criar diretório build
build:
	$(MKDIR) build

# Compilar objetos
build/adilsongui$(OBJ_EXT): $(GUI_SRC) build
	$(CC) $(CFLAGS) -c $(GUI_SRC) -o $@

build/builder$(OBJ_EXT): src/builder.cpp build
	$(CC) $(CFLAGS) -c src/builder.cpp -o $@

build/myjson$(OBJ_EXT): src/myjson.cpp build
	$(CC) $(CFLAGS) -c src/myjson.cpp -o $@

build/utils$(OBJ_EXT): src/utils.cpp build
	$(CC) $(CFLAGS) -c src/utils.cpp -o $@

# Compilar exemplo
$(EXAMPLE_EXE): $(OBJS) $(EXAMPLE_SRC)
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) $(OBJS) -o $@ $(LIBS_WIN)
else
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) $(OBJS) -o $@ $(LIBS_LINUX) $(LIBS_SSL)
	chmod +x $@
endif

# Compilar builder
$(BUILDER_EXE): src/builder.cpp src/myjson.cpp src/utils.cpp build
ifeq ($(OS),Windows_NT)
	$(CC) $(CFLAGS) src/builder.cpp src/myjson.cpp src/utils.cpp -o $@ $(LIBS_SSL)
else
	$(CC) $(CFLAGS) src/builder.cpp src/myjson.cpp src/utils.cpp -o $@ $(LIBS_SSL)
	chmod +x $@
endif

# Limpar
clean:
	$(RMDIR) build
	@echo "Limpeza concluída!"

# Executar exemplo
run: $(EXAMPLE_EXE)
	@echo "Executando exemplo..."
	./$(EXAMPLE_EXE)

# Verificar dependências (Linux)
deps:
ifeq ($(OS),Windows_NT)
	@echo "Windows detectado - use 'dumpbin /DEPENDENTS build/exemplo.exe' para ver DLLs"
else
	@echo "Verificando dependências Linux..."
	@ldd $(EXAMPLE_EXE) 2>/dev/null || echo "Executável não encontrado. Execute 'make' primeiro."
endif

# Ajuda
help:
	@echo "=== AdilsonGUI Makefile ==="
	@echo "Propriedade Intelectual: Adilson Oliveira 2025"
	@echo ""
	@echo "Comandos disponíveis:"
	@echo "  make        - Compilar tudo"
	@echo "  make clean  - Limpar arquivos de build"
	@echo "  make run    - Executar exemplo"
	@echo "  make deps   - Verificar dependências"
	@echo "  make help   - Mostrar esta ajuda"
	@echo ""
	@echo "Sistema detectado: $(if $(OS),Windows,Linux)"

.PHONY: all clean run deps help build 