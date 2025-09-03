@echo off
REM Propriedade Intelectual: Adilson Oliveira 2025
REM Build da Janela de Chat Moderna
setlocal

echo 🎨 ADILSONCHAT - JANELA DE CHAT MODERNA 🎨
echo Propriedade Intelectual: Adilson Oliveira 2025
echo =============================================

if not exist build mkdir build

echo.
echo 🛡️ Compilando ChatGUI...
g++ -std=c++17 -Wall -Iinclude -c src/chatgui.cpp -o build/chatgui.o

echo.
echo ⚡ Compilando Chat Demo...
g++ -std=c++17 -Wall -Iinclude exemplo/chat_demo.cpp build/chatgui.o -o build/chat_demo.exe -lgdi32 -lcomctl32

echo.
echo =============================================
echo ✅ CHAT COMPILADO COM SUCESSO!
echo =============================================
echo.
echo Executável criado: build/chat_demo.exe
echo.
echo 🚀 ABRINDO A JANELA DE CHAT...
echo.
echo Controles:
echo   - Digite sua mensagem no campo de texto
echo   - Pressione ENTER ou clique em 'Enviar'
echo   - Use a roda do mouse para fazer scroll
echo   - Pressione ESC para sair
echo.
echo 🎨 Interface moderna com:
echo   - Transparência e efeito Liquid Glass
echo   - Tema escuro moderno
echo   - Mensagens com scroll automático
echo   - Bot automático respondendo
echo.
echo =============================================

REM Executar o chat automaticamente
build\chat_demo.exe

echo.
echo Chat encerrado!
echo Propriedade Intelectual: Adilson Oliveira 2025

endlocal
pause 