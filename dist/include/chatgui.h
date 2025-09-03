/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * ChatGUI - Interface de Chat Moderna com Liquid Glass
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 */

#pragma once

#include <string>
#include <vector>
#include <functional>

class ChatGUI {
public:
    virtual ~ChatGUI() = default;
    
    // Inicializar janela de chat
    virtual bool initialize(const std::string& title, int width, int height) = 0;
    
    // Adicionar mensagem ao chat
    virtual void addMessage(const std::string& sender, const std::string& message, bool isOwn = false) = 0;
    
    // Configurar callback para quando enviar mensagem
    virtual void setSendCallback(std::function<void(const std::string&)> callback) = 0;
    
    // Executar loop principal
    virtual void run() = 0;
    
    // Limpeza
    virtual void cleanup() = 0;
    
    // Configurar tema
    virtual void setTheme(const std::string& theme) = 0;
    
    // Configurar usuário atual
    virtual void setCurrentUser(const std::string& username) = 0;
};

// Factory function
extern "C" ChatGUI* createChatGUI(); 