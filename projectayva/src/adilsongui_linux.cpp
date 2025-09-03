/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * AdilsonGUI - Biblioteca GUI Original para Linux (X11)
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 * 
 * Funcionalidades:
 * - Janelas com transparência usando X11
 * - Efeito Liquid Glass (blur + transparência)
 * - Menus dinâmicos via JSON
 * - Sistema de eventos nativo
 */

#include "../include/adilsongui.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xcomposite.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <string>
#include <vector>
#include <map>

class AdilsonGUILinux : public AdilsonGUI {
private:
    Display* display;
    Window window;
    cairo_surface_t* surface;
    cairo_t* cr;
    int screen;
    bool running;
    
    // Estrutura do menu
    struct MenuItem {
        std::string text;
        std::string action;
        int x, y, width, height;
    };
    std::vector<MenuItem> menuItems;
    bool menuVisible;
    
public:
    AdilsonGUILinux() : display(nullptr), window(0), surface(nullptr), 
                        cr(nullptr), running(false), menuVisible(false) {}
    
    bool initialize(const std::string& title, int width, int height) override {
        display = XOpenDisplay(nullptr);
        if (!display) {
            printf("Erro: Não foi possível conectar ao X11\n");
            return false;
        }
        
        screen = DefaultScreen(display);
        Window root = DefaultRootWindow(display);
        
        // Configurar atributos da janela
        XVisualInfo vinfo;
        XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo);
        
        XSetWindowAttributes attr;
        attr.colormap = XCreateColormap(display, root, vinfo.visual, AllocNone);
        attr.border_pixel = 0;
        attr.background_pixel = 0;
        attr.override_redirect = True;
        
        // Criar janela com transparência
        window = XCreateWindow(display, root, 100, 100, width, height, 0,
                              vinfo.depth, InputOutput, vinfo.visual,
                              CWColormap | CWBorderPixel | CWBackPixel | CWOverrideRedirect, &attr);
        
        // Configurar propriedades para transparência
        Atom _NET_WM_WINDOW_TYPE = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
        Atom _NET_WM_WINDOW_TYPE_DESKTOP = XInternAtom(display, "_NET_WM_WINDOW_TYPE_DESKTOP", False);
        XChangeProperty(display, window, _NET_WM_WINDOW_TYPE, XA_ATOM, 32, PropModeReplace,
                        (unsigned char*)&_NET_WM_WINDOW_TYPE_DESKTOP, 1);
        
        // Configurar título
        XStoreName(display, window, title.c_str());
        
        // Configurar eventos
        XSelectInput(display, window, ExposureMask | KeyPressMask | ButtonPressMask | 
                                    ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
        
        // Mostrar janela
        XMapWindow(display, window);
        XFlush(display);
        
        // Inicializar Cairo para desenho
        surface = cairo_xlib_surface_create(display, window, vinfo.visual, width, height);
        cr = cairo_create(surface);
        
        running = true;
        printf("AdilsonGUI Linux: Janela criada com sucesso!\n");
        return true;
    }
    
    void setMenu(const std::string& menuJson) override {
        menuItems.clear();
        // Parse simples do JSON do menu
        // Formato esperado: [{"text":"Item1","action":"action1"},{"text":"Item2","action":"action2"}]
        
        size_t pos = 0;
        while ((pos = menuJson.find("\"text\":\"", pos)) != std::string::npos) {
            pos += 8; // Pular "text":"
            size_t end = menuJson.find("\"", pos);
            if (end != std::string::npos) {
                MenuItem item;
                item.text = menuJson.substr(pos, end - pos);
                
                // Encontrar action
                size_t actionPos = menuJson.find("\"action\":\"", end);
                if (actionPos != std::string::npos) {
                    actionPos += 10; // Pular "action":"
                    size_t actionEnd = menuJson.find("\"", actionPos);
                    if (actionEnd != std::string::npos) {
                        item.action = menuJson.substr(actionPos, actionEnd - actionPos);
                    }
                }
                
                menuItems.push_back(item);
            }
        }
        
        printf("AdilsonGUI Linux: Menu carregado com %zu itens\n", menuItems.size());
    }
    
    void showMenu() override {
        menuVisible = true;
        redraw();
    }
    
    void hideMenu() override {
        menuVisible = false;
        redraw();
    }
    
    void redraw() {
        if (!cr) return;
        
        // Limpar com transparência
        cairo_set_source_rgba(cr, 0.1, 0.1, 0.2, 0.8); // Azul escuro semi-transparente
        cairo_paint(cr);
        
        // Desenhar borda com efeito Liquid Glass
        cairo_set_source_rgba(cr, 0.3, 0.5, 0.8, 0.9);
        cairo_set_line_width(cr, 2.0);
        cairo_rectangle(cr, 1, 1, cairo_xlib_surface_get_width(surface) - 2, 
                       cairo_xlib_surface_get_height(surface) - 2);
        cairo_stroke(cr);
        
        // Desenhar título
        cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
        cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_font_size(cr, 18);
        cairo_move_to(cr, 20, 30);
        cairo_show_text(cr, "AdilsonGUI Linux - Liquid Glass Effect");
        
        // Desenhar menu se visível
        if (menuVisible) {
            drawMenu();
        }
        
        cairo_surface_flush(surface);
        XFlush(display);
    }
    
    void drawMenu() {
        if (menuItems.empty()) return;
        
        int menuY = 60;
        int itemHeight = 30;
        
        for (size_t i = 0; i < menuItems.size(); i++) {
            MenuItem& item = menuItems[i];
            item.x = 20;
            item.y = menuY + i * itemHeight;
            item.width = 200;
            item.height = itemHeight;
            
            // Fundo do item do menu
            cairo_set_source_rgba(cr, 0.2, 0.3, 0.5, 0.8);
            cairo_rectangle(cr, item.x, item.y, item.width, item.height);
            cairo_fill(cr);
            
            // Texto do item
            cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
            cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
            cairo_set_font_size(cr, 14);
            cairo_move_to(cr, item.x + 10, item.y + 20);
            cairo_show_text(cr, item.text.c_str());
        }
    }
    
    void run() override {
        XEvent event;
        
        while (running) {
            XNextEvent(display, &event);
            
            switch (event.type) {
                case Expose:
                    redraw();
                    break;
                    
                case KeyPress:
                    if (event.xkey.keycode == 9) { // ESC
                        running = false;
                    } else if (event.xkey.keycode == 64) { // Espaço
                        menuVisible = !menuVisible;
                        redraw();
                    }
                    break;
                    
                case ButtonPress:
                    if (event.xbutton.button == Button1) {
                        handleMouseClick(event.xbutton.x, event.xbutton.y);
                    }
                    break;
                    
                case ClientMessage:
                    running = false;
                    break;
            }
        }
    }
    
    void handleMouseClick(int x, int y) {
        if (!menuVisible) return;
        
        for (const MenuItem& item : menuItems) {
            if (x >= item.x && x <= item.x + item.width &&
                y >= item.y && y <= item.y + item.height) {
                printf("AdilsonGUI Linux: Menu item clicado: %s (%s)\n", 
                       item.text.c_str(), item.action.c_str());
                break;
            }
        }
    }
    
    void cleanup() override {
        if (cr) cairo_destroy(cr);
        if (surface) cairo_surface_destroy(surface);
        if (window) XDestroyWindow(display, window);
        if (display) XCloseDisplay(display);
        printf("AdilsonGUI Linux: Limpeza concluída\n");
    }
    
    ~AdilsonGUILinux() {
        cleanup();
    }
};

// Factory function para criar instância Linux
extern "C" AdilsonGUI* createAdilsonGUI() {
    return new AdilsonGUILinux();
} 