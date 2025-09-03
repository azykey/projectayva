#include "../include/adilsoncrypto.h"
#include <iostream>

int main() {
    std::cout << "🔐 ADILSONCRYPTO - EXEMPLO BÁSICO" << std::endl;
    std::cout << "=================================" << std::endl;
    std::cout << std::endl;
    
    // Criar instância da biblioteca
    AdilsonCrypto* crypto = createAdilsonCrypto();
    
    try {
        // 1. Gerar par de chaves
        std::cout << "1. Gerando par de chaves..." << std::endl;
        auto keypair = crypto->generateKeyPair();
        
        std::cout << "   Chave Privada: " << keypair.private_key.substr(0, 32) << "..." << std::endl;
        std::cout << "   Chave Pública: " << keypair.public_key.substr(0, 64) << "..." << std::endl;
        std::cout << "   Endereço: " << keypair.address << std::endl;
        std::cout << std::endl;
        
        // 2. Assinar uma mensagem
        std::cout << "2. Assinando mensagem..." << std::endl;
        std::string message = "Olá, AdilsonCrypto!";
        auto signature = crypto->sign(message, keypair.private_key);
        
        std::cout << "   Mensagem: " << message << std::endl;
        std::cout << "   Assinatura R: " << signature.r.substr(0, 32) << "..." << std::endl;
        std::cout << "   Assinatura S: " << signature.s.substr(0, 32) << "..." << std::endl;
        std::cout << std::endl;
        
        // 3. Verificar assinatura
        std::cout << "3. Verificando assinatura..." << std::endl;
        bool valid = crypto->verify(message, signature, keypair.public_key);
        std::cout << "   Assinatura válida: " << (valid ? "SIM ✅" : "NÃO ❌") << std::endl;
        std::cout << std::endl;
        
        // 4. Funções de hash
        std::cout << "4. Funções de hash..." << std::endl;
        std::string data = "Dados para hash";
        std::cout << "   Dados: " << data << std::endl;
        std::cout << "   SHA256: " << crypto->sha256(data) << std::endl;
        std::cout << "   SHA512: " << crypto->sha512(data).substr(0, 64) << "..." << std::endl;
        std::cout << "   RIPEMD160: " << crypto->ripemd160(data) << std::endl;
        std::cout << std::endl;
        
        // 5. Criptografia simétrica
        std::cout << "5. Criptografia simétrica..." << std::endl;
        std::string secret = "Mensagem secreta";
        std::string key = "chave_secreta_32_bytes_123456789";
        
        std::cout << "   Mensagem original: " << secret << std::endl;
        auto encrypted = crypto->aesEncrypt(secret, key);
        std::cout << "   Criptografado: " << encrypted.substr(0, 50) << "..." << std::endl;
        
        auto decrypted = crypto->aesDecrypt(encrypted, key);
        std::cout << "   Descriptografado: " << decrypted << std::endl;
        std::cout << std::endl;
        
        // 6. Validação
        std::cout << "6. Validação..." << std::endl;
        bool key_valid = crypto->validateKeyPair(keypair);
        bool sig_valid = crypto->validateSignature(signature);
        bool addr_valid = crypto->validateAddress(keypair.address);
        
        std::cout << "   Par de chaves válido: " << (key_valid ? "✅" : "❌") << std::endl;
        std::cout << "   Assinatura válida: " << (sig_valid ? "✅" : "❌") << std::endl;
        std::cout << "   Endereço válido: " << (addr_valid ? "✅" : "❌") << std::endl;
        std::cout << std::endl;
        
        std::cout << "✅ Exemplo básico concluído com sucesso!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << "❌ Erro: " << e.what() << std::endl;
    }
    
    // Limpar recursos
    destroyAdilsonCrypto(crypto);
    
    return 0;
} 