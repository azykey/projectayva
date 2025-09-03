/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Packager - Sistema de Empacotamento Inteligente
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 * 
 * Funcionalidades que SUPERAM o Electron:
 * - Detecção automática de DLLs
 * - Empacotamento inteligente
 * - Criação de distribuição portátil
 * - Geração de instalador
 * - Otimização de tamanho
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

struct PackageConfig {
    std::string projectName;
    std::string executablePath;
    std::string outputDir;
    std::string installerName;
    std::string appName;
    std::string version;
    std::string description;
    std::string author;
    std::string iconPath;
    bool createInstaller;
    bool createPortable;
    bool optimizeSize;
    std::vector<std::string> includeFiles;
    std::vector<std::string> excludeFiles;
    
    PackageConfig() : createInstaller(true), createPortable(true), optimizeSize(true) {}
};

struct PackageResult {
    std::string projectName;
    std::string portablePath;
    std::string installerPath;
    std::vector<std::string> includedDlls;
    size_t originalSize;
    size_t finalSize;
    bool success;
    std::string errorMessage;
    
    PackageResult() : originalSize(0), finalSize(0), success(false) {}
};

class Packager {
public:
    virtual ~Packager() = default;
    
    // Configuração
    virtual void setConfig(const PackageConfig& config) = 0;
    virtual PackageConfig getConfig() const = 0;
    
    // Detecção de dependências
    virtual std::vector<std::string> detectDependencies(const std::string& executablePath) = 0;
    virtual std::vector<std::string> findRequiredDlls(const std::string& executablePath) = 0;
    virtual std::map<std::string, std::string> analyzeDependencies(const std::string& executablePath) = 0;
    
    // Empacotamento
    virtual PackageResult createPortablePackage(const std::string& projectName) = 0;
    virtual PackageResult createInstaller(const std::string& projectName) = 0;
    virtual PackageResult createCompletePackage(const std::string& projectName) = 0;
    
    // Otimização
    virtual void optimizePackage(const std::string& packagePath) = 0;
    virtual size_t calculatePackageSize(const std::string& packagePath) = 0;
    virtual void compressPackage(const std::string& packagePath) = 0;
    
    // Utilitários
    virtual void copyDependencies(const std::string& sourceDir, const std::string& targetDir) = 0;
    virtual void createManifest(const std::string& packagePath, const PackageConfig& config) = 0;
    virtual void validatePackage(const std::string& packagePath) = 0;
    
    // Callbacks
    virtual void setProgressCallback(std::function<void(int, const std::string&)> callback) = 0;
    virtual void setLogCallback(std::function<void(const std::string&)> callback) = 0;
};

// Factory function
extern "C" Packager* createPackager(); 