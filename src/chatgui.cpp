/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * ChatGUI - Implementação Profissional da Janela de Chat
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 *
 * Funcionalidades:
 * - Janela padrão com barra de título, minimizar, maximizar, fechar
 * - Layout moderno, balões de mensagem, padding, fontes bonitas
 * - Botão enviar estilizado
 * - Ícone na janela
 * - Transparência opcional
 */

#include "../include/chatgui.h"
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <chrono>
#include <shellapi.h>

#pragma comment(lib, "comctl32.lib")

#define CHAT_ICON_ID 101

struct Message {
    std::string sender;
    std::string text;
    bool isOwn;
    std::chrono::system_clock::time_point timestamp;
    Message(const std::string& s, const std::string& t, bool own = false)
        : sender(s), text(t), isOwn(own), timestamp(std::chrono::system_clock::now()) {}
};

class ChatGUIWin32 : public ChatGUI {
private:
    HWND hwnd;
    HWND hwndEdit;
    HWND hwndSend;
    HWND hwndChat;
    HFONT hFont;
    HFONT hFontBold;
    HICON hIcon;
    std::vector<Message> messages;
    std::string currentUser;
    std::string currentTheme;
    std::function<void(const std::string&)> sendCallback;
    bool running;
    int scrollY;
    int maxScrollY;
    struct ThemeColors {
        COLORREF background;
        COLORREF text;
        COLORREF ownMessage;
        COLORREF otherMessage;
        COLORREF inputBackground;
        COLORREF buttonBackground;
        COLORREF buttonText;
        COLORREF border;
    } colors;
public:
    ChatGUIWin32() : hwnd(nullptr), hwndEdit(nullptr), hwndSend(nullptr), hwndChat(nullptr),
        hFont(nullptr), hFontBold(nullptr), hIcon(nullptr), running(false), scrollY(0), maxScrollY(0) {
        setTheme("dark");
    }
    bool initialize(const std::string& title, int width, int height) override {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "AdilsonChatGUI";
        wc.hbrBackground = CreateSolidBrush(colors.background);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        RegisterClassEx(&wc);
        hwnd = CreateWindowEx(
            0, // sem WS_EX_LAYERED para barra de título
            "AdilsonChatGUI",
            title.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            nullptr, nullptr, GetModuleHandle(nullptr), this
        );
        if (!hwnd) {
            std::cerr << "Erro ao criar janela de chat" << std::endl;
            return false;
        }
        // Ícone customizado (opcional)
        hIcon = LoadIcon(nullptr, IDI_INFORMATION);
        if (hIcon) {
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }
        createControls();
        hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
        hFontBold = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
        running = true;
        addMessage("Sistema", "Bem-vindo ao AdilsonChat! Digite sua mensagem abaixo.", false);
        std::cout << "ChatGUI: Janela criada com sucesso!" << std::endl;
        return true;
    }
    void addMessage(const std::string& sender, const std::string& message, bool isOwn = false) override {
        messages.emplace_back(sender, message, isOwn);
        maxScrollY = std::max(0, (int)messages.size() * 70 - 400);
        scrollY = maxScrollY;
        InvalidateRect(hwndChat, nullptr, TRUE);
        UpdateWindow(hwndChat);
        std::cout << "ChatGUI: Mensagem adicionada - " << sender << ": " << message << std::endl;
    }
    void setSendCallback(std::function<void(const std::string&)> callback) override {
        sendCallback = callback;
    }
    void setTheme(const std::string& theme) override {
        currentTheme = theme;
        if (theme == "dark") {
            colors.background = RGB(36, 37, 46);
            colors.text = RGB(255, 255, 255);
            colors.ownMessage = RGB(0, 120, 215);
            colors.otherMessage = RGB(60, 60, 70);
            colors.inputBackground = RGB(45, 45, 55);
            colors.buttonBackground = RGB(0, 120, 215);
            colors.buttonText = RGB(255, 255, 255);
            colors.border = RGB(80, 80, 100);
        } else {
            colors.background = RGB(240, 240, 245);
            colors.text = RGB(30, 30, 30);
            colors.ownMessage = RGB(0, 120, 215);
            colors.otherMessage = RGB(220, 220, 225);
            colors.inputBackground = RGB(255, 255, 255);
            colors.buttonBackground = RGB(0, 120, 215);
            colors.buttonText = RGB(255, 255, 255);
            colors.border = RGB(180, 180, 200);
        }
        if (hwnd) {
            InvalidateRect(hwnd, nullptr, TRUE);
        }
    }
    void setCurrentUser(const std::string& username) override {
        currentUser = username;
    }
    void run() override {
        MSG msg;
        while (running && GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    void cleanup() override {
        if (hFont) DeleteObject(hFont);
        if (hFontBold) DeleteObject(hFontBold);
        if (hwnd) DestroyWindow(hwnd);
        std::cout << "ChatGUI: Limpeza concluída" << std::endl;
    }
private:
    void createControls() {
        hwndChat = CreateWindowEx(
            0, "STATIC", "",
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
            10, 10, 380, 400,
            hwnd, (HMENU)1003, GetModuleHandle(nullptr), nullptr
        );
        hwndEdit = CreateWindowEx(
            0, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
            10, 420, 270, 30,
            hwnd, (HMENU)1001, GetModuleHandle(nullptr), nullptr
        );
        hwndSend = CreateWindowEx(
            0, "BUTTON", "Enviar",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            290, 420, 100, 30,
            hwnd, (HMENU)1002, GetModuleHandle(nullptr), nullptr
        );
        SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hwndSend, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndChat, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    void handleSendMessage() {
        char buffer[1024];
        GetWindowTextA(hwndEdit, buffer, 1024);
        if (strlen(buffer) > 0) {
            std::string text(buffer);
            addMessage(currentUser.empty() ? "Você" : currentUser, text, true);
            SetWindowTextA(hwndEdit, "");
            if (sendCallback) {
                sendCallback(text);
            }
        }
    }
    void drawChat(HDC hdc, RECT rect) {
        HBRUSH hBrushBg = CreateSolidBrush(colors.background);
        FillRect(hdc, &rect, hBrushBg);
        DeleteObject(hBrushBg);
        int y = 20 - scrollY;
        for (const auto& msg : messages) {
            if (y + 60 > 0 && y < rect.bottom) {
                drawMessage(hdc, msg, 20, y, rect.right - 40);
            }
            y += 70;
        }
    }
    void drawMessage(HDC hdc, const Message& msg, int x, int y, int maxWidth) {
        COLORREF msgColor = msg.isOwn ? colors.ownMessage : colors.otherMessage;
        HBRUSH hBrushMsg = CreateSolidBrush(msgColor);
        RECT textRect = {x, y, x + maxWidth, y + 60};
        DrawTextA(hdc, msg.text.c_str(), -1, &textRect, DT_CALCRECT | DT_WORDBREAK);
        int msgWidth = textRect.right - textRect.left + 30;
        int msgHeight = textRect.bottom - textRect.top + 20;
        if (msg.isOwn) {
            x = maxWidth - msgWidth + 40;
        }
        RECT msgRect = {x, y, x + msgWidth, y + msgHeight};
        FillRect(hdc, &msgRect, hBrushMsg);
        HPEN hPen = CreatePen(PS_SOLID, 1, colors.border);
        HGDIOBJ oldPen = SelectObject(hdc, hPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        RoundRect(hdc, msgRect.left, msgRect.top, msgRect.right, msgRect.bottom, 18, 18);
        SelectObject(hdc, oldPen);
        DeleteObject(hPen);
        SetTextColor(hdc, colors.text);
        SetBkMode(hdc, TRANSPARENT);
        SelectObject(hdc, hFontBold);
        RECT nameRect = {x + 10, y + 8, x + msgWidth - 10, y + 28};
        DrawTextA(hdc, msg.sender.c_str(), -1, &nameRect, DT_LEFT);
        SelectObject(hdc, hFont);
        RECT textRect2 = {x + 10, y + 32, x + msgWidth - 10, y + msgHeight - 10};
        DrawTextA(hdc, msg.text.c_str(), -1, &textRect2, DT_LEFT | DT_WORDBREAK);
        DeleteObject(hBrushMsg);
    }
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        ChatGUIWin32* chat = nullptr;
        if (uMsg == WM_CREATE) {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            chat = (ChatGUIWin32*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)chat);
        } else {
            chat = (ChatGUIWin32*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        if (!chat) return DefWindowProc(hwnd, uMsg, wParam, lParam);
        switch (uMsg) {
        case WM_COMMAND:
            if (LOWORD(wParam) == 1002) {
                chat->handleSendMessage();
            }
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_RETURN) {
                chat->handleSendMessage();
                return 0;
            } else if (wParam == VK_ESCAPE) {
                chat->running = false;
                PostQuitMessage(0);
                return 0;
            }
            break;
        case WM_MOUSEWHEEL:
            chat->scrollY -= GET_WHEEL_DELTA_WPARAM(wParam) / 120 * 30;
            chat->scrollY = std::max(0, std::min(chat->scrollY, chat->maxScrollY));
            InvalidateRect(chat->hwndChat, nullptr, TRUE);
            return 0;
        case WM_DRAWITEM:
            if (wParam == 1003) {
                DRAWITEMSTRUCT* dis = (DRAWITEMSTRUCT*)lParam;
                chat->drawChat(dis->hDC, dis->rcItem);
                return TRUE;
            }
            break;
        case WM_CLOSE:
            chat->running = false;
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            chat->running = false;
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};
extern "C" ChatGUI* createChatGUI() {
    return new ChatGUIWin32();
} 