/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Exemplo Linux - Demonstração do AdilsonGUI com X11
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 * 
 * Este exemplo demonstra:
 * - Criação de janela com transparência no Linux
 * - Efeito Liquid Glass usando X11 + Cairo
 * - Menu dinâmico via JSON
 * - Sistema de eventos nativo
 */

#include "../include/adilsongui.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== AdilsonGUI Linux - Exemplo Original ===" << std::endl;
    std::cout << "Propriedade Intelectual: Adilson Oliveira 2025" << std::endl;
    std::cout << "Sistema: Linux X11 com Cairo" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Criar instância do GUI
    AdilsonGUI* gui = createAdilsonGUI();
    
    if (!gui) {
        std::cerr << "Erro: Não foi possível criar a instância do GUI" << std::endl;
        return 1;
    }
    
    // Inicializar janela
    if (!gui->initialize("AdilsonGUI Linux - Liquid Glass", 800, 600)) {
        std::cerr << "Erro: Falha ao inicializar a janela" << std::endl;
        delete gui;
        return 1;
    }
    
    // Configurar menu via JSON
    std::string menuJson = R"([
        {"text": "Arquivo", "action": "menu_file"},
        {"text": "Editar", "action": "menu_edit"},
        {"text": "Visualizar", "action": "menu_view"},
        {"text": "Ferramentas", "action": "menu_tools"},
        {"text": "Ajuda", "action": "menu_help"}
    ])";
    
    gui->setMenu(menuJson);
    
    std::cout << "Janela criada com sucesso!" << std::endl;
    std::cout << "Controles:" << std::endl;
    std::cout << "  - ESPAÇO: Mostrar/Ocultar menu" << std::endl;
    std::cout << "  - ESC: Sair" << std::endl;
    std::cout << "  - Clique: Interagir com menu" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    // Mostrar menu inicialmente
    gui->showMenu();
    
    // Executar loop principal
    gui->run();
    
    // Limpeza
    gui->cleanup();
    delete gui;
    
    std::cout << "Aplicação encerrada com sucesso!" << std::endl;
    return 0;
} 