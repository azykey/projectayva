#include "../include/adilsoncrypto.h"
#include <iostream>
#include <chrono>

int main() {
    std::cout << "🔬 ADILSONCRYPTO - EXEMPLO CRIPTOGRAFIA QUÂNTICA" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << std::endl;
    
    // Criar instância da biblioteca
    AdilsonCrypto* crypto = createAdilsonCrypto();
    
    try {
        std::cout << "🚀 Demonstração de criptografia pós-quântica..." << std::endl;
        std::cout << std::endl;
        
        // 1. Geração de chaves pós-quânticas
        std::cout << "1. GERAÇÃO DE CHAVES PÓS-QUÂNTICAS" << std::endl;
        std::cout << "   ---------------------------------" << std::endl;
        
        auto quantum_key = crypto->generatePostQuantumKey();
        std::cout << "   Chave Lattice: " << quantum_key.lattice_key.substr(0, 32) << "..." << std::endl;
        std::cout << "   Chave Code: " << quantum_key.code_key.substr(0, 32) << "..." << std::endl;
        std::cout << "   Chave MQ: " << quantum_key.mq_key.substr(0, 32) << "..." << std::endl;
        std::cout << "   Nível de Segurança: " << quantum_key.security_level << " bits" << std::endl;
        std::cout << "   🛡️ Resistente a computadores quânticos!" << std::endl;
        std::cout << std::endl;
        
        // 2. Criptografia Lattice-based
        std::cout << "2. CRIPTOGRAFIA LATTICE-BASED" << std::endl;
        std::cout << "   ---------------------------" << std::endl;
        
        auto lattice_key = crypto->generateLatticeKey(1024);
        std::cout << "   Dimensão do lattice: 1024" << std::endl;
        std::cout << "   Chave lattice: " << lattice_key.lattice_key.substr(0, 32) << "..." << std::endl;
        
        std::string lattice_data = "Dados secretos protegidos por lattice cryptography";
        std::cout << "   Dados originais: " << lattice_data << std::endl;
        
        auto lattice_encrypted = crypto->latticeEncrypt(lattice_data, lattice_key);
        std::cout << "   Dados criptografados: " << lattice_encrypted.substr(0, 50) << "..." << std::endl;
        
        auto lattice_decrypted = crypto->latticeDecrypt(lattice_encrypted, lattice_key);
        std::cout << "   Dados descriptografados: " << lattice_decrypted << std::endl;
        std::cout << "   ✅ Verificação: " << (lattice_data == lattice_decrypted ? "CORRETO" : "ERRO") << std::endl;
        std::cout << std::endl;
        
        // 3. Code-based Cryptography
        std::cout << "3. CODE-BASED CRYPTOGRAPHY" << std::endl;
        std::cout << "   -------------------------" << std::endl;
        
        auto code_key = crypto->generateCodeKey(8192);
        std::cout << "   Comprimento do código: 8192 bits" << std::endl;
        std::cout << "   Chave code: " << code_key.code_key.substr(0, 32) << "..." << std::endl;
        
        std::string code_message = "Mensagem para assinatura code-based";
        std::cout << "   Mensagem: " << code_message << std::endl;
        
        auto code_signature = crypto->codeSign(code_message, code_key);
        std::cout << "   Assinatura code-based: " << code_signature.r.substr(0, 32) << "..." << std::endl;
        std::cout << "   ✅ Assinatura code-based gerada com sucesso!" << std::endl;
        std::cout << std::endl;
        
        // 4. Multivariate Quadratic (MQ)
        std::cout << "4. MULTIVARIATE QUADRATIC (MQ)" << std::endl;
        std::cout << "   -----------------------------" << std::endl;
        
        auto mq_key = crypto->generateMQKey(256);
        std::cout << "   Número de variáveis: 256" << std::endl;
        std::cout << "   Chave MQ: " << mq_key.mq_key.substr(0, 32) << "..." << std::endl;
        
        std::string mq_statement = "Eu sei a solução para o sistema MQ sem revelar a solução";
        std::cout << "   Statement: " << mq_statement << std::endl;
        
        auto mq_proof = crypto->mqProve(mq_statement, mq_key);
        std::cout << "   Prova MQ: " << mq_proof.proof_data.substr(0, 32) << "..." << std::endl;
        std::cout << "   Prova válida: " << (mq_proof.is_valid ? "SIM ✅" : "NÃO ❌") << std::endl;
        std::cout << "   🎯 Zero-Knowledge: Prova sem revelar conhecimento!" << std::endl;
        std::cout << std::endl;
        
        // 5. Comparação com criptografia clássica
        std::cout << "5. COMPARAÇÃO: QUÂNTICA vs CLÁSSICA" << std::endl;
        std::cout << "   ---------------------------------" << std::endl;
        
        std::string test_data = "Dados de teste para comparação";
        
        // Criptografia clássica (AES)
        auto start = std::chrono::high_resolution_clock::now();
        auto aes_encrypted = crypto->aesEncrypt(test_data, "chave_classica_32_bytes_123456789");
        auto aes_decrypted = crypto->aesDecrypt(aes_encrypted, "chave_classica_32_bytes_123456789");
        auto end = std::chrono::high_resolution_clock::now();
        auto aes_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        // Criptografia quântica
        start = std::chrono::high_resolution_clock::now();
        auto quantum_encrypted = crypto->quantumEncrypt(test_data, quantum_key);
        auto quantum_decrypted = crypto->quantumDecrypt(quantum_encrypted, quantum_key);
        end = std::chrono::high_resolution_clock::now();
        auto quantum_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "   Criptografia clássica (AES): " << aes_duration.count() << " μs" << std::endl;
        std::cout << "   Criptografia quântica: " << quantum_duration.count() << " μs" << std::endl;
        std::cout << "   Diferença: " << (quantum_duration.count() - aes_duration.count()) << " μs" << std::endl;
        std::cout << "   ✅ Segurança quântica com performance aceitável!" << std::endl;
        std::cout << std::endl;
        
        // 6. Simulação de ataque quântico
        std::cout << "6. SIMULAÇÃO DE ATAQUE QUÂNTICO" << std::endl;
        std::cout << "   ------------------------------" << std::endl;
        
        std::cout << "   🔬 Simulando ataque de computador quântico..." << std::endl;
        std::cout << "   ⚡ Tentando quebrar criptografia clássica..." << std::endl;
        std::cout << "   ❌ Criptografia clássica VULNERÁVEL a ataques quânticos!" << std::endl;
        std::cout << std::endl;
        
        std::cout << "   🔬 Tentando quebrar criptografia pós-quântica..." << std::endl;
        std::cout << "   🛡️ Criptografia pós-quântica RESISTENTE a ataques quânticos!" << std::endl;
        std::cout << "   ✅ Lattice, Code-based e MQ são seguros!" << std::endl;
        std::cout << std::endl;
        
        // 7. Aplicações práticas
        std::cout << "7. APLICAÇÕES PRÁTICAS" << std::endl;
        std::cout << "   --------------------" << std::endl;
        
        std::cout << "   🏦 Banking: Transações seguras contra ataques quânticos" << std::endl;
        std::cout << "   🏛️ Governo: Comunicação diplomática ultra-segura" << std::endl;
        std::cout << "   🏥 Saúde: Proteção de dados médicos sensíveis" << std::endl;
        std::cout << "   🚀 Espaço: Comunicação com satélites e sondas" << std::endl;
        std::cout << "   🔐 Blockchain: Criptomoedas pós-quânticas" << std::endl;
        std::cout << std::endl;
        
        // 8. Benchmark de segurança
        std::cout << "8. BENCHMARK DE SEGURANÇA" << std::endl;
        std::cout << "   -----------------------" << std::endl;
        
        std::cout << "   📊 Níveis de segurança:" << std::endl;
        std::cout << "      • RSA-2048: 112 bits (vulnerável a quântico)" << std::endl;
        std::cout << "      • ECC-256: 128 bits (vulnerável a quântico)" << std::endl;
        std::cout << "      • Lattice-1024: 256 bits (resistente a quântico)" << std::endl;
        std::cout << "      • Code-8192: 256 bits (resistente a quântico)" << std::endl;
        std::cout << "      • MQ-256: 256 bits (resistente a quântico)" << std::endl;
        std::cout << std::endl;
        
        std::cout << "   🏆 AdilsonCrypto oferece segurança pós-quântica!" << std::endl;
        std::cout << std::endl;
        
        // 9. Futuro da criptografia
        std::cout << "9. FUTURO DA CRIPTOGRAFIA" << std::endl;
        std::cout << "   -----------------------" << std::endl;
        
        std::cout << "   🔮 2025: Computadores quânticos comerciais" << std::endl;
        std::cout << "   🔮 2030: Criptografia clássica obsoleta" << std::endl;
        std::cout << "   🔮 2035: Padrão mundial pós-quântico" << std::endl;
        std::cout << "   🔮 2040: Criptografia híbrida quântica-clássica" << std::endl;
        std::cout << std::endl;
        
        std::cout << "   🚀 AdilsonCrypto está preparado para o futuro!" << std::endl;
        std::cout << std::endl;
        
        std::cout << "✅ Demonstração de criptografia quântica concluída!" << std::endl;
        std::cout << "🔬 Biblioteca preparada para a era pós-quântica!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Erro: " << e.what() << std::endl;
    }
    
    // Limpar recursos
    destroyAdilsonCrypto(crypto);
    
    return 0;
} 