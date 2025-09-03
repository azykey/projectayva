/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Biblioteca gráfica original (AdilsonGUI)
 * Código 100% autoral, sem dependências externas.
 * Assinatura digital: SHA-256 do arquivo.
 */
#include "../include/adilsongui.h"
#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>

namespace adilsongui {

struct Window::Impl {
    HWND hwnd;
    std::string titulo;
    int largura, altura;
    std::map<int, std::string> menuItens;
    HMENU hMenu = nullptr;
};

// Função de janela Win32
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

Window::Window(int largura, int altura, const std::string& titulo) {
    pImpl = new Impl;
    pImpl->largura = largura;
    pImpl->altura = altura;
    pImpl->titulo = titulo;

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"AdilsonGUIJanela";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    RegisterClass(&wc);

    pImpl->hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_APPWINDOW,
        L"AdilsonGUIJanela",
        std::wstring(titulo.begin(), titulo.end()).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        largura, altura,
        nullptr, nullptr, hInstance, nullptr);

    if (!pImpl->hwnd) throw std::runtime_error("Falha ao criar janela");

    // Efeito Liquid Glass (transparência + blur básico Win32)
    SetLayeredWindowAttributes(pImpl->hwnd, 0, (255 * 80) / 100, LWA_ALPHA);
    // Futuro: implementar blur real (DWM ou software)
}

Window::~Window() {
    if (pImpl->hwnd) DestroyWindow(pImpl->hwnd);
    delete pImpl;
}

void Window::show() {
    ShowWindow(pImpl->hwnd, SW_SHOW);
    UpdateWindow(pImpl->hwnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::setMenu(const std::string& menuJson) {
    // Exemplo: menuJson = "[\"Arquivo\", \"Editar\", \"Sair\"]"
    // (parse simples, sem dependências externas)
    std::vector<std::string> itens;
    size_t i = 0;
    while ((i = menuJson.find('"', i)) != std::string::npos) {
        size_t j = menuJson.find('"', i+1);
        if (j == std::string::npos) break;
        itens.push_back(menuJson.substr(i+1, j-i-1));
        i = j+1;
    }
    if (pImpl->hMenu) DestroyMenu(pImpl->hMenu);
    pImpl->hMenu = CreateMenu();
    int id = 1000;
    for (const auto& item : itens) {
        AppendMenuA(pImpl->hMenu, MF_STRING, id, item.c_str());
        pImpl->menuItens[id] = item;
        ++id;
    }
    SetMenu(pImpl->hwnd, pImpl->hMenu);
}

void init() {}
void shutdown() {}

} // namespace adilsongui 