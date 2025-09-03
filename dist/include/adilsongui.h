/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Biblioteca gráfica original (AdilsonGUI)
 * Código 100% autoral, sem dependências externas.
 * Assinatura digital: SHA-256 do arquivo.
 */
#pragma once
#include <string>

namespace adilsongui {

// Forward declaration
class Window;

// Classe base para janela
class Window {
public:
    Window(int largura, int altura, const std::string& titulo);
    virtual ~Window();
    void show();
    void setMenu(const std::string& menuJson); // menu em JSON
    // Futuro: adicionar métodos para efeitos visuais, blur, etc
protected:
    // Implementação interna (oculta)
    struct Impl;
    Impl* pImpl;
};

// Inicialização global da biblioteca (se necessário)
void init();
void shutdown();

} // namespace adilsongui 