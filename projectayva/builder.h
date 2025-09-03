#pragma once
#include <string>
#include <vector>

// Estrutura para armazenar informações do projeto
struct ProjetoConfig {
    std::string nome;
    std::vector<std::string> fontes;
    std::vector<std::string> recursos;
    std::string pasta_saida;
};

// Funções principais do builder
bool lerConfig(const std::string& caminho, ProjetoConfig& config);
bool compilarFontes(const ProjetoConfig& config);
bool copiarRecursos(const ProjetoConfig& config);
bool compactarSaida(const ProjetoConfig& config); 