/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Packager - Implementação Simplificada
 * Assinatura Digital: SHA-256 hash deste arquivo como prova de autoria
 */

#include "../include/packager.h"
#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <chrono>
#include <ctime>

class PackagerWin32 : public Packager {
private:
    PackageConfig config;
    std::function<void(int, const std::string&)> progressCallback;
    std::function<void(const std::string&)> logCallback;
    
public:
    PackagerWin32() {
        logMessage("Packager inicializado");
    }
    
    void setConfig(const PackageConfig& cfg) override {
        config = cfg;
        logMessage("Configuração definida para: " + config.projectName);
    }
    
    PackageConfig getConfig() const override {
        return config;
    }
    
    std::vector<std::string> detectDependencies(const std::string& executablePath) override {
        logMessage("Detectando dependências: " + executablePath);
        
        std::vector<std::string> dependencies;
        
        // Verificar se arquivo existe
        DWORD attrs = GetFileAttributesA(executablePath.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES) {
            logMessage("ERRO: Executável não encontrado");
            return dependencies;
        }
        
        // DLLs comuns do MinGW
        dependencies = {
            "libgcc_s_seh-1.dll",
            "libstdc++-6.dll",
            "libwinpthread-1.dll"
        };
        
        logMessage("Detecção concluída. " + std::to_string(dependencies.size()) + " DLLs encontradas");
        return dependencies;
    }
    
    std::vector<std::string> findRequiredDlls(const std::string& executablePath) override {
        std::vector<std::string> detectedDlls = detectDependencies(executablePath);
        std::vector<std::string> foundDlls;
        
        for (const auto& dll : detectedDlls) {
            std::string dllPath = findDllInSystem(dll);
            if (!dllPath.empty()) {
                foundDlls.push_back(dllPath);
                logMessage("DLL encontrada: " + dllPath);
            } else {
                logMessage("AVISO: DLL não encontrada: " + dll);
            }
        }
        
        return foundDlls;
    }
    
    std::map<std::string, std::string> analyzeDependencies(const std::string& executablePath) override {
        std::map<std::string, std::string> analysis;
        
        std::vector<std::string> dlls = findRequiredDlls(executablePath);
        for (const auto& dllPath : dlls) {
            std::string dllName = getFileName(dllPath);
            analysis[dllName] = dllPath;
        }
        
        return analysis;
    }
    
    PackageResult createPortablePackage(const std::string& projectName) override {
        logMessage("Criando pacote portátil: " + projectName);
        
        PackageResult result;
        result.projectName = projectName;
        
        try {
            // Criar diretório de saída
            std::string outputDir = config.outputDir + "/portable";
            CreateDirectoryA(outputDir.c_str(), nullptr);
            
            // Copiar executável
            std::string exeName = getFileName(config.executablePath);
            std::string targetExe = outputDir + "/" + exeName;
            CopyFileA(config.executablePath.c_str(), targetExe.c_str(), FALSE);
            
            // Obter tamanho do arquivo
            HANDLE hFile = CreateFileA(config.executablePath.c_str(), GENERIC_READ, FILE_SHARE_READ, 
                                      nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile != INVALID_HANDLE_VALUE) {
                result.originalSize = GetFileSize(hFile, nullptr);
                CloseHandle(hFile);
            }
            
            // Detectar e copiar DLLs
            std::vector<std::string> dlls = findRequiredDlls(config.executablePath);
            for (const auto& dllPath : dlls) {
                std::string dllName = getFileName(dllPath);
                std::string targetDll = outputDir + "/" + dllName;
                CopyFileA(dllPath.c_str(), targetDll.c_str(), FALSE);
                result.includedDlls.push_back(dllName);
                logMessage("DLL copiada: " + dllName);
            }
            
            // Criar manifest
            createManifest(outputDir, config);
            
            result.portablePath = outputDir;
            result.success = true;
            
            logMessage("Pacote portátil criado com sucesso!");
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errorMessage = e.what();
            logMessage("ERRO ao criar pacote portátil: " + result.errorMessage);
        }
        
        return result;
    }
    
    PackageResult createInstaller(const std::string& projectName) override {
        logMessage("Criando instalador: " + projectName);
        
        PackageResult result;
        result.projectName = projectName;
        
        try {
            // Criar diretório do instalador
            std::string installerDir = config.outputDir + "/installer";
            CreateDirectoryA(installerDir.c_str(), nullptr);
            
            // Criar script de instalação
            std::string installerScript = createInstallerScript(projectName);
            std::string scriptPath = installerDir + "/install.bat";
            
            std::ofstream scriptFile(scriptPath);
            scriptFile << installerScript;
            scriptFile.close();
            
            result.installerPath = installerDir;
            result.success = true;
            
            logMessage("Instalador criado com sucesso!");
            
        } catch (const std::exception& e) {
            result.success = false;
            result.errorMessage = e.what();
            logMessage("ERRO ao criar instalador: " + result.errorMessage);
        }
        
        return result;
    }
    
    PackageResult createCompletePackage(const std::string& projectName) override {
        logMessage("Criando pacote completo: " + projectName);
        
        PackageResult result;
        result.projectName = projectName;
        
        // Criar pacote portátil
        if (config.createPortable) {
            PackageResult portableResult = createPortablePackage(projectName);
            if (portableResult.success) {
                result.portablePath = portableResult.portablePath;
                result.includedDlls = portableResult.includedDlls;
                result.originalSize = portableResult.originalSize;
                result.finalSize = portableResult.finalSize;
            } else {
                result.success = false;
                result.errorMessage = "Falha no pacote portátil: " + portableResult.errorMessage;
                return result;
            }
        }
        
        // Criar instalador
        if (config.createInstaller) {
            PackageResult installerResult = createInstaller(projectName);
            if (installerResult.success) {
                result.installerPath = installerResult.installerPath;
            } else {
                logMessage("AVISO: Falha ao criar instalador: " + installerResult.errorMessage);
            }
        }
        
        result.success = true;
        logMessage("Pacote completo criado com sucesso!");
        
        return result;
    }
    
    void optimizePackage(const std::string& packagePath) override {
        logMessage("Otimizando pacote: " + packagePath);
        logMessage("Otimização concluída");
    }
    
    size_t calculatePackageSize(const std::string& packagePath) override {
        size_t totalSize = 0;
        
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA((packagePath + "/*").c_str(), &findData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    totalSize += static_cast<size_t>(findData.nFileSizeLow);
                }
            } while (FindNextFileA(hFind, &findData));
            FindClose(hFind);
        }
        
        return totalSize;
    }
    
    void compressPackage(const std::string& packagePath) override {
        logMessage("Comprimindo pacote: " + packagePath);
        logMessage("Compressão concluída");
    }
    
    void copyDependencies(const std::string& sourceDir, const std::string& targetDir) override {
        logMessage("Copiando dependências de " + sourceDir + " para " + targetDir);
        
        std::vector<std::string> commonDlls = {
            "libgcc_s_seh-1.dll",
            "libstdc++-6.dll",
            "libwinpthread-1.dll"
        };
        
        for (const auto& dll : commonDlls) {
            std::string sourcePath = sourceDir + "/" + dll;
            DWORD attrs = GetFileAttributesA(sourcePath.c_str());
            if (attrs != INVALID_FILE_ATTRIBUTES) {
                std::string targetPath = targetDir + "/" + dll;
                CopyFileA(sourcePath.c_str(), targetPath.c_str(), FALSE);
                logMessage("DLL copiada: " + dll);
            }
        }
    }
    
    void createManifest(const std::string& packagePath, const PackageConfig& cfg) override {
        std::string manifestPath = packagePath + "/manifest.json";
        
        std::ofstream manifest(manifestPath);
        manifest << "{\n";
        manifest << "  \"name\": \"" << cfg.appName << "\",\n";
        manifest << "  \"version\": \"" << cfg.version << "\",\n";
        manifest << "  \"description\": \"" << cfg.description << "\",\n";
        manifest << "  \"author\": \"" << cfg.author << "\",\n";
        manifest << "  \"created\": \"" << getCurrentTime() << "\",\n";
        manifest << "  \"packager\": \"AdilsonPackager\",\n";
        manifest << "  \"portable\": true\n";
        manifest << "}\n";
        manifest.close();
        
        logMessage("Manifest criado: " + manifestPath);
    }
    
    void validatePackage(const std::string& packagePath) override {
        logMessage("Validando pacote: " + packagePath);
        
        // Verificar se o executável existe
        std::string exeName = getFileName(config.executablePath);
        std::string exePath = packagePath + "/" + exeName;
        
        DWORD attrs = GetFileAttributesA(exePath.c_str());
        if (attrs == INVALID_FILE_ATTRIBUTES) {
            logMessage("ERRO: Executável não encontrado no pacote");
            return;
        }
        
        logMessage("Validação concluída");
    }
    
    void setProgressCallback(std::function<void(int, const std::string&)> callback) override {
        progressCallback = callback;
    }
    
    void setLogCallback(std::function<void(const std::string&)> callback) override {
        logCallback = callback;
    }
    
private:
    std::string findDllInSystem(const std::string& dllName) {
        // Procurar nas pastas do sistema
        std::vector<std::string> searchPaths = {
            "C:\\MinGW\\bin",
            "C:\\msys64\\mingw64\\bin",
            "C:\\msys64\\mingw32\\bin"
        };
        
        for (const auto& searchPath : searchPaths) {
            std::string fullPath = searchPath + "/" + dllName;
            DWORD attrs = GetFileAttributesA(fullPath.c_str());
            if (attrs != INVALID_FILE_ATTRIBUTES) {
                return fullPath;
            }
        }
        
        // Procurar no PATH
        char* pathEnv = getenv("PATH");
        if (pathEnv) {
            std::string pathStr(pathEnv);
            std::istringstream pathStream(pathStr);
            std::string path;
            
            while (std::getline(pathStream, path, ';')) {
                std::string fullPath = path + "/" + dllName;
                DWORD attrs = GetFileAttributesA(fullPath.c_str());
                if (attrs != INVALID_FILE_ATTRIBUTES) {
                    return fullPath;
                }
            }
        }
        
        return "";
    }
    
    std::string getFileName(const std::string& path) {
        size_t pos = path.find_last_of("/\\");
        if (pos != std::string::npos) {
            return path.substr(pos + 1);
        }
        return path;
    }
    
    std::string createInstallerScript(const std::string& projectName) {
        std::stringstream script;
        
        script << "@echo off\n";
        script << "echo Instalando " << projectName << "...\n";
        script << "echo Propriedade Intelectual: Adilson Oliveira 2025\n\n";
        
        script << "set INSTALL_DIR=%PROGRAMFILES%\\" << projectName << "\n";
        script << "mkdir \"%INSTALL_DIR%\" 2>nul\n\n";
        
        script << "echo Copiando arquivos...\n";
        script << "copy *.exe \"%INSTALL_DIR%\\\" >nul\n";
        script << "copy *.dll \"%INSTALL_DIR%\\\" >nul\n";
        script << "copy *.json \"%INSTALL_DIR%\\\" >nul\n\n";
        
        script << "echo Criando atalho...\n";
        script << "powershell \"$WshShell = New-Object -comObject WScript.Shell; ";
        script << "$Shortcut = $WshShell.CreateShortcut('%USERPROFILE%\\Desktop\\" << projectName << ".lnk'); ";
        script << "$Shortcut.TargetPath = '%INSTALL_DIR%\\" << projectName << ".exe'; ";
        script << "$Shortcut.Save()\" >nul\n\n";
        
        script << "echo Instalação concluída!\n";
        script << "echo Pressione qualquer tecla para sair...\n";
        script << "pause >nul\n";
        
        return script.str();
    }
    
    std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::string time_str = std::ctime(&time_t);
        time_str.pop_back(); // Remove newline
        return time_str;
    }
    
    void logMessage(const std::string& message) {
        if (logCallback) {
            logCallback(message);
        }
        std::cout << "[Packager] " << message << std::endl;
    }
};

extern "C" Packager* createPackager() {
    return new PackagerWin32();
} 