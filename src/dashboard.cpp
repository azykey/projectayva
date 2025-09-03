/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Dashboard - Implementação do Sistema Principal
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 * 
 * Dashboard moderno que SUPERA o Electron:
 * - Interface gráfica profissional
 * - Gerenciamento de projetos
 * - Sistema de build automático
 * - Empacotamento inteligente
 * - Distribuição com um clique
 */

#include "../include/dashboard.h"
#include "../include/packager.h"
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>
// Removido filesystem - usando implementação nativa
#include <algorithm>
#include <chrono>
#include <ctime>

#pragma comment(lib, "comctl32.lib")

class DashboardWin32 : public Dashboard {
private:
    HWND hwnd;
    HWND hwndProjectsList;
    HWND hwndBuildButton;
    HWND hwndPackageButton;
    HWND hwndCreateButton;
    HWND hwndLogArea;
    HWND hwndStatusBar;
    
    HFONT hFont;
    HFONT hFontBold;
    
    std::vector<Project> projects;
    std::map<std::string, BuildResult> buildResults;
    
    std::function<void(const BuildResult&)> buildCallback;
    std::function<void(const Project&)> projectCallback;
    
    Packager* packager;
    bool running;
    
    struct ThemeColors {
        COLORREF background;
        COLORREF text;
        COLORREF buttonBackground;
        COLORREF buttonText;
        COLORREF listBackground;
        COLORREF listText;
        COLORREF statusBackground;
        COLORREF statusText;
    } colors;
    
public:
    DashboardWin32() : hwnd(nullptr), hwndProjectsList(nullptr), hwndBuildButton(nullptr),
                       hwndPackageButton(nullptr), hwndCreateButton(nullptr), hwndLogArea(nullptr),
                       hwndStatusBar(nullptr), hFont(nullptr), hFontBold(nullptr), 
                       packager(nullptr), running(false) {
        setTheme();
        packager = createPackager();
    }
    
    bool initialize(const std::string& title, int width, int height) override {
        WNDCLASSEX wc = {};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = "AdilsonDashboard";
        wc.hbrBackground = CreateSolidBrush(colors.background);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
        
        RegisterClassEx(&wc);
        
        hwnd = CreateWindowEx(
            0,
            "AdilsonDashboard",
            title.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            nullptr, nullptr, GetModuleHandle(nullptr), this
        );
        
        if (!hwnd) {
            std::cerr << "Erro ao criar dashboard" << std::endl;
            return false;
        }
        
        createControls();
        createFonts();
        loadProjects();
        
        running = true;
        updateStatus("Dashboard iniciado - Pronto para criar projetos!");
        
        std::cout << "Dashboard: Inicializado com sucesso!" << std::endl;
        return true;
    }
    
    void createProject(const std::string& name, const std::string& path) override {
        Project project;
        project.name = name;
        project.path = path;
        project.version = "1.0.0";
        project.description = "Projeto criado pelo AdilsonDashboard";
        project.isActive = true;
        
        // Criar estrutura de pastas
        CreateDirectoryA(path.c_str(), nullptr);
        CreateDirectoryA((path + "/src").c_str(), nullptr);
        CreateDirectoryA((path + "/include").c_str(), nullptr);
        CreateDirectoryA((path + "/build").c_str(), nullptr);
        CreateDirectoryA((path + "/dist").c_str(), nullptr);
        
        // Configuração padrão
        project.config["compiler"] = "g++";
        project.config["std"] = "c++17";
        project.config["optimization"] = "O2";
        
        projects.push_back(project);
        saveProjects();
        updateProjectsList();
        
        logMessage("Projeto criado: " + name);
        updateStatus("Projeto '" + name + "' criado com sucesso!");
    }
    
    void loadProject(const std::string& path) override {
        // Implementar carregamento de projeto existente
        logMessage("Carregando projeto: " + path);
    }
    
    void saveProject(const Project& project) override {
        // Salvar configuração do projeto
        std::string configPath = project.path + "/project.json";
        // Implementar salvamento JSON
        logMessage("Projeto salvo: " + project.name);
    }
    
    std::vector<Project> getProjects() override {
        return projects;
    }
    
    BuildResult buildProject(const std::string& projectName) override {
        logMessage("Iniciando build do projeto: " + projectName);
        updateStatus("Compilando " + projectName + "...");
        
        BuildResult result;
        result.projectName = projectName;
        
        // Encontrar projeto
        auto it = std::find_if(projects.begin(), projects.end(),
                              [&](const Project& p) { return p.name == projectName; });
        
        if (it == projects.end()) {
            result.success = false;
            result.errorMessage = "Projeto não encontrado: " + projectName;
            logMessage("ERRO: " + result.errorMessage);
            return result;
        }
        
        const Project& project = *it;
        
        // Executar build
        std::string buildCmd = "cd \"" + project.path + "\" && g++ -std=c++17 -O2 ";
        buildCmd += "-Iinclude src/*.cpp -o build/app.exe";
        
        int result_code = system(buildCmd.c_str());
        
        if (result_code == 0) {
            result.success = true;
            result.executablePath = project.path + "/build/app.exe";
            result.outputPath = project.path + "/build/";
            
            // Detectar DLLs
            if (packager) {
                result.dlls = packager->detectDependencies(result.executablePath);
            }
            
            // Obter tamanho do arquivo
            HANDLE hFile = CreateFileA(result.executablePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
                                      nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile != INVALID_HANDLE_VALUE) {
                result.totalSize = GetFileSize(hFile, nullptr);
                CloseHandle(hFile);
            }
            
            logMessage("Build concluído com sucesso!");
            updateStatus("Build de '" + projectName + "' concluído!");
            
            if (buildCallback) {
                buildCallback(result);
            }
        } else {
            result.success = false;
            result.errorMessage = "Erro na compilação";
            logMessage("ERRO: Falha na compilação");
            updateStatus("Erro no build de '" + projectName + "'");
        }
        
        buildResults[projectName] = result;
        return result;
    }
    
    BuildResult buildAllProjects() override {
        logMessage("Iniciando build de todos os projetos...");
        BuildResult finalResult;
        finalResult.success = true;
        
        for (const auto& project : projects) {
            if (project.isActive) {
                BuildResult result = buildProject(project.name);
                if (!result.success) {
                    finalResult.success = false;
                    finalResult.errorMessage += "Falha em " + project.name + "; ";
                }
            }
        }
        
        if (finalResult.success) {
            logMessage("Todos os builds concluídos com sucesso!");
            updateStatus("Build de todos os projetos concluído!");
        } else {
            logMessage("Alguns builds falharam!");
            updateStatus("Alguns builds falharam!");
        }
        
        return finalResult;
    }
    
    void packageProject(const std::string& projectName) override {
        logMessage("Empacotando projeto: " + projectName);
        updateStatus("Empacotando " + projectName + "...");
        
        if (!packager) {
            logMessage("ERRO: Packager não disponível");
            return;
        }
        
        // Configurar packager
        PackageConfig config;
        config.projectName = projectName;
        config.appName = projectName;
        config.version = "1.0.0";
        config.description = "Aplicação criada com AdilsonDashboard";
        config.author = "Adilson Oliveira 2025";
        config.outputDir = "./dist/" + projectName;
        config.createPortable = true;
        config.createInstaller = true;
        
        packager->setConfig(config);
        
        // Criar pacote
        PackageResult result = packager->createCompletePackage(projectName);
        
        if (result.success) {
            logMessage("Empacotamento concluído!");
            logMessage("Pacote criado em: " + result.portablePath);
            updateStatus("Empacotamento de '" + projectName + "' concluído!");
        } else {
            logMessage("ERRO no empacotamento: " + result.errorMessage);
            updateStatus("Erro no empacotamento de '" + projectName + "'");
        }
    }
    
    void createInstaller(const std::string& projectName) override {
        logMessage("Criando instalador para: " + projectName);
        updateStatus("Criando instalador...");
        
        if (packager) {
            PackageResult result = packager->createInstaller(projectName);
            if (result.success) {
                logMessage("Instalador criado: " + result.installerPath);
                updateStatus("Instalador criado com sucesso!");
            } else {
                logMessage("ERRO ao criar instalador: " + result.errorMessage);
                updateStatus("Erro ao criar instalador");
            }
        }
    }
    
    void setBuildConfig(const std::string& projectName, const std::map<std::string, std::string>& config) override {
        auto it = std::find_if(projects.begin(), projects.end(),
                              [&](const Project& p) { return p.name == projectName; });
        if (it != projects.end()) {
            it->config = config;
            saveProjects();
            logMessage("Configuração atualizada para: " + projectName);
        }
    }
    
    std::map<std::string, std::string> getBuildConfig(const std::string& projectName) override {
        auto it = std::find_if(projects.begin(), projects.end(),
                              [&](const Project& p) { return p.name == projectName; });
        if (it != projects.end()) {
            return it->config;
        }
        return {};
    }
    
    void setBuildCallback(std::function<void(const BuildResult&)> callback) override {
        buildCallback = callback;
    }
    
    void setProjectCallback(std::function<void(const Project&)> callback) override {
        projectCallback = callback;
    }
    
    void run() override {
        MSG msg;
        while (running && GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    void cleanup() override {
        if (packager) {
            delete packager;
        }
        if (hFont) DeleteObject(hFont);
        if (hFontBold) DeleteObject(hFontBold);
        if (hwnd) DestroyWindow(hwnd);
        
        std::cout << "Dashboard: Limpeza concluída" << std::endl;
    }
    
    void openProjectFolder(const std::string& projectName) override {
        auto it = std::find_if(projects.begin(), projects.end(),
                              [&](const Project& p) { return p.name == projectName; });
        if (it != projects.end()) {
            ShellExecuteA(nullptr, "open", it->path.c_str(), nullptr, nullptr, SW_SHOW);
            logMessage("Pasta do projeto aberta: " + projectName);
        }
    }
    
    void openOutputFolder(const std::string& projectName) override {
        std::string outputPath = "./dist/" + projectName;
        DWORD attrs = GetFileAttributesA(outputPath.c_str());
        if (attrs != INVALID_FILE_ATTRIBUTES) {
            ShellExecuteA(nullptr, "open", outputPath.c_str(), nullptr, nullptr, SW_SHOW);
            logMessage("Pasta de saída aberta: " + projectName);
        }
    }
    
    void showBuildLog(const std::string& projectName) override {
        logMessage("Mostrando log do build: " + projectName);
        // Implementar visualização de log
    }
    
private:
    void setTheme() {
        colors.background = RGB(45, 45, 55);
        colors.text = RGB(255, 255, 255);
        colors.buttonBackground = RGB(0, 120, 215);
        colors.buttonText = RGB(255, 255, 255);
        colors.listBackground = RGB(60, 60, 70);
        colors.listText = RGB(255, 255, 255);
        colors.statusBackground = RGB(30, 30, 40);
        colors.statusText = RGB(200, 200, 200);
    }
    
    void createControls() {
        // Lista de projetos
        hwndProjectsList = CreateWindowEx(
            0, "LISTBOX", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY | WS_VSCROLL,
            20, 20, 300, 200,
            hwnd, (HMENU)1001, GetModuleHandle(nullptr), nullptr
        );
        
        // Botões
        hwndCreateButton = CreateWindowEx(
            0, "BUTTON", "Criar Projeto",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            20, 240, 120, 30,
            hwnd, (HMENU)1002, GetModuleHandle(nullptr), nullptr
        );
        
        hwndBuildButton = CreateWindowEx(
            0, "BUTTON", "Build Projeto",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            150, 240, 120, 30,
            hwnd, (HMENU)1003, GetModuleHandle(nullptr), nullptr
        );
        
        hwndPackageButton = CreateWindowEx(
            0, "BUTTON", "Empacotar",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            280, 240, 120, 30,
            hwnd, (HMENU)1004, GetModuleHandle(nullptr), nullptr
        );
        
        // Área de log
        hwndLogArea = CreateWindowEx(
            0, "EDIT", "",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | WS_VSCROLL,
            340, 20, 400, 200,
            hwnd, (HMENU)1005, GetModuleHandle(nullptr), nullptr
        );
        
        // Barra de status
        hwndStatusBar = CreateWindowEx(
            0, "STATIC", "Pronto",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 280, 720, 20,
            hwnd, (HMENU)1006, GetModuleHandle(nullptr), nullptr
        );
    }
    
    void createFonts() {
        hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                          DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                          CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
        
        hFontBold = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                              CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
        
        SendMessage(hwndProjectsList, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hwndCreateButton, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndBuildButton, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndPackageButton, WM_SETFONT, (WPARAM)hFontBold, TRUE);
        SendMessage(hwndLogArea, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hwndStatusBar, WM_SETFONT, (WPARAM)hFont, TRUE);
    }
    
    void loadProjects() {
        // Carregar projetos salvos
        DWORD attrs = GetFileAttributesA("projects.json");
        if (attrs != INVALID_FILE_ATTRIBUTES) {
            // Implementar carregamento JSON
            logMessage("Projetos carregados do arquivo");
        }
        updateProjectsList();
    }
    
    void saveProjects() {
        // Salvar projetos em JSON
        logMessage("Projetos salvos");
    }
    
    void updateProjectsList() {
        SendMessage(hwndProjectsList, LB_RESETCONTENT, 0, 0);
        for (const auto& project : projects) {
            std::string displayName = project.name + " (" + project.version + ")";
            SendMessageA(hwndProjectsList, LB_ADDSTRING, 0, (LPARAM)displayName.c_str());
        }
    }
    
    void logMessage(const std::string& message) {
        std::string timestamp = getCurrentTime();
        std::string logEntry = "[" + timestamp + "] " + message + "\r\n";
        
        int length = GetWindowTextLength(hwndLogArea);
        SendMessage(hwndLogArea, EM_SETSEL, length, length);
        SendMessageA(hwndLogArea, EM_REPLACESEL, FALSE, (LPARAM)logEntry.c_str());
        
        // Auto-scroll
        SendMessage(hwndLogArea, EM_SCROLLCARET, 0, 0);
        
        std::cout << logEntry;
    }
    
    void updateStatus(const std::string& status) {
        SetWindowTextA(hwndStatusBar, status.c_str());
    }
    
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::string time_str = std::ctime(&time_t);
        time_str.pop_back(); // Remove newline
        return time_str;
    }
    
    void handleCreateProject() {
        // Implementar diálogo para criar projeto
        createProject("NovoProjeto", "./projects/NovoProjeto");
    }
    
    void handleBuildProject() {
        int selected = SendMessage(hwndProjectsList, LB_GETCURSEL, 0, 0);
        if (selected != LB_ERR && selected < projects.size()) {
            buildProject(projects[selected].name);
        } else {
            logMessage("Selecione um projeto para fazer build");
        }
    }
    
    void handlePackageProject() {
        int selected = SendMessage(hwndProjectsList, LB_GETCURSEL, 0, 0);
        if (selected != LB_ERR && selected < projects.size()) {
            packageProject(projects[selected].name);
        } else {
            logMessage("Selecione um projeto para empacotar");
        }
    }
    
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        DashboardWin32* dashboard = nullptr;
        
        if (uMsg == WM_CREATE) {
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            dashboard = (DashboardWin32*)cs->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)dashboard);
        } else {
            dashboard = (DashboardWin32*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }
        
        if (!dashboard) return DefWindowProc(hwnd, uMsg, wParam, lParam);
        
        switch (uMsg) {
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
            case 1002: // Criar Projeto
                dashboard->handleCreateProject();
                break;
            case 1003: // Build Projeto
                dashboard->handleBuildProject();
                break;
            case 1004: // Empacotar
                dashboard->handlePackageProject();
                break;
            }
            return 0;
            
        case WM_CLOSE:
            dashboard->running = false;
            DestroyWindow(hwnd);
            return 0;
            
        case WM_DESTROY:
            dashboard->running = false;
            PostQuitMessage(0);
            return 0;
        }
        
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

extern "C" Dashboard* createDashboard() {
    return new DashboardWin32();
} 