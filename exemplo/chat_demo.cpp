/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Chat Demo - Demonstração da Janela de Chat Moderna
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 * 
 * Este exemplo abre uma janela de chat funcional com:
 * - Interface moderna com transparência
 * - Campo de entrada para mensagens
 * - Botão enviar
 * - Scroll automático
 * - Temas personalizáveis
 */

#include "../include/chatgui.h"
#include <iostream>
#include <chrono>
#include <random>

int main() {
    std::cout << "=== AdilsonChat - Janela de Chat Moderna ===" << std::endl;
    std::cout << "Propriedade Intelectual: Adilson Oliveira 2025" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Criar instância do chat
    ChatGUI* chat = createChatGUI();
    
    if (!chat) {
        std::cerr << "Erro: Não foi possível criar a janela de chat" << std::endl;
        return 1;
    }
    
    // Configurar usuário
    chat->setCurrentUser("Adilson");
    
    // Configurar tema escuro
    chat->setTheme("dark");
    
    // Configurar callback para mensagens enviadas
    chat->setSendCallback([](const std::string& message) {
        std::cout << "Mensagem enviada: " << message << std::endl;
    });
    
    // Inicializar janela
    if (!chat->initialize("AdilsonChat - Chat Moderno", 420, 500)) {
        std::cerr << "Erro: Falha ao inicializar a janela de chat" << std::endl;
        delete chat;
        return 1;
    }
    
    std::cout << "Janela de chat aberta!" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  - Digite sua mensagem no campo de texto" << std::endl;
    std::cout << "  - Pressione ENTER ou clique em 'Enviar'" << std::endl;
    std::cout << "  - Use a roda do mouse para fazer scroll" << std::endl;
    std::cout << "  - Pressione ESC para sair" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Adicionar algumas mensagens de exemplo
    chat->addMessage("Sistema", "Chat iniciado com sucesso!", false);
    chat->addMessage("Adilson", "Olá! Esta é uma demonstração do AdilsonChat!", true);
    chat->addMessage("Bot", "Bem-vindo ao chat! Como posso ajudar?", false);
    chat->addMessage("Adilson", "Testando a interface moderna com Liquid Glass!", true);
    chat->addMessage("Bot", "A interface está funcionando perfeitamente!", false);
    
    // Adicionar mensagens de exemplo do bot
    chat->addMessage("Bot", "Que legal essa interface!", false);
    chat->addMessage("Bot", "O efeito Liquid Glass está incrível!", false);
    chat->addMessage("Bot", "C++ pode fazer interfaces modernas sim!", false);
    chat->addMessage("Bot", "Adilson Oliveira 2025 - Propriedade Intelectual!", false);
    
    // Executar chat
    chat->run();
    
    // Limpeza
    chat->cleanup();
    delete chat;
    
    std::cout << "Chat encerrado!" << std::endl;
    return 0;
} 