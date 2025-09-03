/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Código original e protegido
 */
#include "builder.h"
#include "utils.h"
#include "myjson.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h> // Requer OpenSSL para hash

// Função para calcular SHA-256 de um arquivo
std::string calcularHashSHA256(const std::string& arquivo) {
    std::ifstream file(arquivo, std::ios::binary);
    if (!file) return "(hash indisponível)";
    std::ostringstream oss;
    oss << file.rdbuf();
    std::string conteudo = oss.str();
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)conteudo.c_str(), conteudo.size(), hash);
    std::ostringstream hex;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        hex << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return hex.str();
}

// Implementação da leitura do config (JSON) usando MyJson
bool lerConfig(const std::string& caminho, ProjetoConfig& config) {
    std::string conteudo;
    if (!lerArquivoTexto(caminho, conteudo)) {
        std::cerr << "[ERRO] Não foi possível ler o arquivo de configuração: " << caminho << std::endl;
        return false;
    }
    try {
        auto root = MyJson::parse(conteudo);
        if (!root->isObjeto()) {
            std::cerr << "[ERRO] O arquivo de configuração não é um objeto JSON válido." << std::endl;
            return false;
        }
        const auto& obj = root->getObjeto();
        // nome
        if (obj.count("nome")) config.nome = obj.at("nome")->getString();
        // fontes
        if (obj.count("fontes")) {
            const auto& arr = obj.at("fontes")->getArray();
            config.fontes.clear();
            for (const auto& v : arr) config.fontes.push_back(v->getString());
        }
        // recursos
        if (obj.count("recursos")) {
            const auto& arr = obj.at("recursos")->getArray();
            config.recursos.clear();
            for (const auto& v : arr) config.recursos.push_back(v->getString());
        }
        // pasta_saida
        if (obj.count("pasta_saida")) config.pasta_saida = obj.at("pasta_saida")->getString();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[ERRO] Falha ao parsear o arquivo de configuração: " << e.what() << std::endl;
        return false;
    }
}

// Compilar arquivos fonte
bool compilarFontes(const ProjetoConfig& config) {
    std::cout << "[INFO] Compilando fontes...\n";
    criarDiretorioSeNaoExiste(config.pasta_saida);
    std::string comando = "g++";
    for (const auto& fonte : config.fontes) {
        comando += " " + fonte;
    }
    comando += " -o " + config.pasta_saida + "/" + config.nome;
    std::cout << "[CMD] " << comando << std::endl;
    if (!executarComando(comando)) {
        std::cerr << "[ERRO] Falha ao compilar os arquivos fonte." << std::endl;
        return false;
    }
    std::cout << "[OK] Compilação concluída." << std::endl;
    return true;
}

// Copiar recursos
bool copiarRecursos(const ProjetoConfig& config) {
    std::cout << "[INFO] Copiando recursos...\n";
    for (const auto& recurso : config.recursos) {
        std::string destino = config.pasta_saida + "/" + recurso;
        if (!copiarArquivo(recurso, destino)) {
            std::cerr << "[ERRO] Falha ao copiar: " << recurso << std::endl;
            return false;
        }
        std::cout << "[OK] Copiado: " << recurso << std::endl;
    }
    return true;
}

// Compactar saída em ZIP
bool compactarSaida(const ProjetoConfig& config) {
    std::cout << "[INFO] Compactando saída em ZIP...\n";
    std::string zipfile = config.nome + ".zip";
    std::string comando = "zip -r " + zipfile + " " + config.pasta_saida;
    std::cout << "[CMD] " << comando << std::endl;
    if (!executarComando(comando)) {
        std::cerr << "[ERRO] Falha ao compactar a saída." << std::endl;
        return false;
    }
    std::cout << "[OK] Saída compactada em: " << zipfile << std::endl;
    return true;
}

// Função principal
int main() {
    std::cout << "\n==============================\n";
    std::cout << "Propriedade Intelectual: Adilson Oliveira 2025\n";
    std::cout << "Assinatura digital (SHA-256 builder.cpp): " << calcularHashSHA256("builder.cpp") << "\n";
    std::cout << "==============================\n\n";

    ProjetoConfig config;
    if (!lerConfig("config.json", config)) {
        std::cerr << "[FATAL] Erro ao ler config.json" << std::endl;
        return 1;
    }
    if (!compilarFontes(config)) {
        std::cerr << "[FATAL] Erro ao compilar fontes" << std::endl;
        return 1;
    }
    if (!copiarRecursos(config)) {
        std::cerr << "[FATAL] Erro ao copiar recursos" << std::endl;
        return 1;
    }
    if (!compactarSaida(config)) {
        std::cerr << "[FATAL] Erro ao compactar saída" << std::endl;
        return 1;
    }
    std::cout << "\n[SUCCESS] Build e empacotamento concluídos com sucesso!\n";
    return 0;
} 