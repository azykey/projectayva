#pragma once
#include <string>
#include <vector>
 
bool lerArquivoTexto(const std::string& caminho, std::string& conteudo);
bool executarComando(const std::string& comando);
bool criarDiretorioSeNaoExiste(const std::string& caminho);
bool copiarArquivo(const std::string& origem, const std::string& destino);
bool listarArquivos(const std::string& pasta, std::vector<std::string>& arquivos, const std::string& extensao = ""); 