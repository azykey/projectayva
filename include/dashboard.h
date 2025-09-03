/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Dashboard - Interface Principal do Sistema de Build
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 * 
 * Funcionalidades que SUPERAM o Electron:
 * - Dashboard moderno para gerenciar projetos
 * - Sistema de build automático
 * - Empacotamento inteligente
 * - Distribuição com um clique
 * - Performance nativa superior
 */

#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>

struct Project {
    std::string name;
    std::string path;
    std::string version;
    std::string description;
    std::vector<std::string> files;
    std::map<std::string, std::string> config;
    bool isActive;
    
    Project() : isActive(false) {}
};

struct BuildResult {
    std::string projectName;
    std::string outputPath;
    std::string executablePath;
    std::vector<std::string> dlls;
    size_t totalSize;
    bool success;
    std::string errorMessage;
    
    BuildResult() : totalSize(0), success(false) {}
};

class Dashboard {
public:
    virtual ~Dashboard() = default;
    
    // Inicializar dashboard
    virtual bool initialize(const std::string& title, int width, int height) = 0;
    
    // Gerenciar projetos
    virtual void createProject(const std::string& name, const std::string& path) = 0;
    virtual void loadProject(const std::string& path) = 0;
    virtual void saveProject(const Project& project) = 0;
    virtual std::vector<Project> getProjects() = 0;
    
    // Sistema de build
    virtual BuildResult buildProject(const std::string& projectName) = 0;
    virtual BuildResult buildAllProjects() = 0;
    virtual void packageProject(const std::string& projectName) = 0;
    virtual void createInstaller(const std::string& projectName) = 0;
    
    // Configurações
    virtual void setBuildConfig(const std::string& projectName, const std::map<std::string, std::string>& config) = 0;
    virtual std::map<std::string, std::string> getBuildConfig(const std::string& projectName) = 0;
    
    // Callbacks
    virtual void setBuildCallback(std::function<void(const BuildResult&)> callback) = 0;
    virtual void setProjectCallback(std::function<void(const Project&)> callback) = 0;
    
    // Executar dashboard
    virtual void run() = 0;
    virtual void cleanup() = 0;
    
    // Utilitários
    virtual void openProjectFolder(const std::string& projectName) = 0;
    virtual void openOutputFolder(const std::string& projectName) = 0;
    virtual void showBuildLog(const std::string& projectName) = 0;
};

// Factory function
extern "C" Dashboard* createDashboard(); 