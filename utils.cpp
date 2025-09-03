#include "utils.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <cstdlib>

namespace fs = std::filesystem;

bool lerArquivoTexto(const std::string& caminho, std::string& conteudo) {
    std::ifstream arquivo(caminho);
    if (!arquivo.is_open()) return false;
    conteudo.assign((std::istreambuf_iterator<char>(arquivo)), std::istreambuf_iterator<char>());
    return true;
}

bool executarComando(const std::string& comando) {
    int resultado = std::system(comando.c_str());
    return resultado == 0;
}

bool criarDiretorioSeNaoExiste(const std::string& caminho) {
    if (!fs::exists(caminho)) {
        return fs::create_directories(caminho);
    }
    return true;
}

bool copiarArquivo(const std::string& origem, const std::string& destino) {
    try {
        fs::copy(origem, destino, fs::copy_options::overwrite_existing);
        return true;
    } catch (...) {
        return false;
    }
}

bool listarArquivos(const std::string& pasta, std::vector<std::string>& arquivos, const std::string& extensao) {
    try {
        for (const auto& entry : fs::directory_iterator(pasta)) {
            if (entry.is_regular_file()) {
                if (extensao.empty() || entry.path().extension() == extensao) {
                    arquivos.push_back(entry.path().string());
                }
            }
        }
        return true;
    } catch (...) {
        return false;
    }
} 