/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Builder original — código 100% autoral
 * Assinatura digital: SHA-256 do arquivo.
 */
#pragma once
#include <string>
#include <vector>

struct ProjetoConfig {
    std::string nome;
    std::vector<std::string> fontes;
    std::vector<std::string> recursos;
    std::string pasta_saida;
};

bool lerConfig(const std::string& caminho, ProjetoConfig& config);
bool compilarFontes(const ProjetoConfig& config);
bool copiarRecursos(const ProjetoConfig& config);
bool compactarSaida(const ProjetoConfig& config); 