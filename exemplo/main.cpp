/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Exemplo de uso da biblioteca gráfica original (AdilsonGUI)
 * Código 100% autoral
 */
#include "../include/adilsongui.h"
#include <iostream>

int main() {
    std::cout << "Exemplo de app com AdilsonGUI — Propriedade Intelectual: Adilson Oliveira 2025\n";
    adilsongui::init();
    adilsongui::Window janela(600, 400, "Minha Janela Liquid Glass");
    janela.setMenu("[\"Arquivo\", \"Editar\", \"Sair\"]");
    janela.show();
    adilsongui::shutdown();
    return 0;
} 