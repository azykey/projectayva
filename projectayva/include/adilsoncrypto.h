#ifndef ADILSONCRYPTO_H
#define ADILSONCRYPTO_H

#include <string>
#include <vector>
#include <memory>
#include <functional>

// Estruturas de dados avançadas
struct KeyPair {
    std::string private_key;
    std::string public_key;
    std::string address;
};

struct Signature {
    std::string r;
    std::string s;
    std::string v;
    std::string proof;
};

struct QuantumKey {
    std::string lattice_key;
    std::string code_key;
    std::string mq_key;
    int security_level;
};

struct ZKProof {
    std::string proof_data;
    std::string public_inputs;
    std::string verification_key;
    bool is_valid;
};

struct MultiSignature {
    std::vector<std::string> signatures;
    std::vector<std::string> public_keys;
    int threshold;
    bool is_complete;
};

struct HomomorphicData {
    std::string encrypted_value;
    std::string public_key;
    std::string parameters;
};

// Interfaces para diferentes tipos de criptografia
class IEllipticCurve {
public:
    virtual ~IEllipticCurve() = default;
    virtual std::string getName() const = 0;
    virtual KeyPair generateKeyPair() = 0;
    virtual Signature sign(const std::string& message, const std::string& private_key) = 0;
    virtual bool verify(const std::string& message, const Signature& signature, const std::string& public_key) = 0;
    virtual std::string getAddress(const std::string& public_key) = 0;
};

class IQuantumCrypto {
public:
    virtual ~IQuantumCrypto() = default;
    virtual QuantumKey generateLatticeKey(int dimension) = 0;
    virtual std::string latticeEncrypt(const std::string& data, const QuantumKey& key) = 0;
    virtual std::string latticeDecrypt(const std::string& encrypted, const QuantumKey& key) = 0;
    virtual QuantumKey generateCodeKey(int code_length) = 0;
    virtual Signature codeSign(const std::string& message, const QuantumKey& key) = 0;
    virtual QuantumKey generateMQKey(int variables) = 0;
    virtual ZKProof mqProve(const std::string& statement, const QuantumKey& key) = 0;
};

class IZeroKnowledge {
public:
    virtual ~IZeroKnowledge() = default;
    virtual std::unique_ptr<IZeroKnowledge> createZKSNARK() = 0;
    virtual ZKProof prove(const std::string& statement, const std::string& witness) = 0;
    virtual bool verify(const std::string& statement, const ZKProof& proof) = 0;
    virtual std::unique_ptr<IZeroKnowledge> createZKSTARK() = 0;
    virtual ZKProof proveSTARK(const std::string& computation) = 0;
    virtual bool verifySTARK(const std::string& computation, const ZKProof& proof) = 0;
    virtual std::unique_ptr<IZeroKnowledge> createBulletproof() = 0;
    virtual ZKProof proveRange(int value, const std::string& commitment) = 0;
    virtual bool verifyRange(const std::string& commitment, const ZKProof& proof) = 0;
};

class IMultiSignature {
public:
    virtual ~IMultiSignature() = default;
    virtual std::unique_ptr<IMultiSignature> createThresholdSignature(int total, int threshold) = 0;
    virtual std::vector<std::string> generateShares(const std::string& private_key) = 0;
    virtual Signature sign(const std::string& message, const std::vector<std::string>& shares) = 0;
    virtual std::unique_ptr<IMultiSignature> createRingSignature(int participants) = 0;
    virtual Signature signRing(const std::string& message, const std::string& private_key, const std::vector<std::string>& public_keys) = 0;
    virtual bool verifyRing(const std::string& message, const Signature& signature, const std::vector<std::string>& public_keys) = 0;
    virtual std::unique_ptr<IMultiSignature> createMPC(int parties) = 0;
    virtual std::string compute(const std::function<std::string(const std::vector<std::string>&)>& function, const std::vector<std::string>& inputs) = 0;
};

class IHomomorphicEncryption {
public:
    virtual ~IHomomorphicEncryption() = default;
    virtual std::unique_ptr<IHomomorphicEncryption> createHomomorphicEncryption() = 0;
    virtual HomomorphicData encrypt(int value) = 0;
    virtual HomomorphicData add(const HomomorphicData& a, const HomomorphicData& b) = 0;
    virtual HomomorphicData multiply(const HomomorphicData& a, const HomomorphicData& b) = 0;
    virtual int decrypt(const HomomorphicData& encrypted) = 0;
};

class IHardwareAccelerator {
public:
    virtual ~IHardwareAccelerator() = default;
    virtual std::unique_ptr<IHardwareAccelerator> createGPUAccelerator() = 0;
    virtual void setDevice(const std::string& device_name) = 0;
    virtual std::vector<Signature> signBatch(const std::vector<std::string>& messages, int batch_size) = 0;
    virtual std::unique_ptr<IHardwareAccelerator> createFPGAAccelerator() = 0;
    virtual void loadBitstream(const std::string& bitstream_path) = 0;
    virtual std::string sha256(const std::string& data) = 0;
    virtual std::unique_ptr<IHardwareAccelerator> createASICSimulator() = 0;
    virtual void setArchitecture(const std::string& architecture) = 0;
    virtual std::string mineBlock(const std::string& header, const std::string& target) = 0;
};

class IBlockchainInterface {
public:
    virtual ~IBlockchainInterface() = default;
    virtual std::unique_ptr<IBlockchainInterface> createBitcoinInterface() = 0;
    virtual KeyPair generateKey() = 0;
    virtual std::string getAddress(const KeyPair& keypair) = 0;
    virtual Signature sign(const std::string& transaction, const KeyPair& keypair) = 0;
    virtual std::unique_ptr<IBlockchainInterface> createEthereumInterface() = 0;
    virtual std::unique_ptr<IBlockchainInterface> createSolanaInterface() = 0;
};

// Classe principal AdilsonCrypto
class AdilsonCrypto {
private:
    std::unique_ptr<IEllipticCurve> current_curve;
    std::unique_ptr<IQuantumCrypto> quantum_crypto;
    std::unique_ptr<IZeroKnowledge> zk_proofs;
    std::unique_ptr<IMultiSignature> multi_sig;
    std::unique_ptr<IHomomorphicEncryption> homomorphic;
    std::unique_ptr<IHardwareAccelerator> hardware;
    std::unique_ptr<IBlockchainInterface> blockchain;

public:
    AdilsonCrypto();
    ~AdilsonCrypto();

    // Métodos básicos
    KeyPair generateKeyPair();
    Signature sign(const std::string& message, const std::string& private_key);
    bool verify(const std::string& message, const Signature& signature, const std::string& public_key);
    std::string getAddress(const std::string& public_key);

    // Curvas elípticas
    std::unique_ptr<IEllipticCurve> createCurve(const std::string& curve_name);
    std::unique_ptr<IEllipticCurve> createCustomCurve(const std::string& p, const std::string& a, const std::string& b);
    void setCurrentCurve(std::unique_ptr<IEllipticCurve> curve);

    // Criptografia quântica
    QuantumKey generatePostQuantumKey();
    std::string quantumEncrypt(const std::string& data, const QuantumKey& key);
    std::string quantumDecrypt(const std::string& encrypted, const QuantumKey& key);
    QuantumKey generateLatticeKey(int dimension);
    std::string latticeEncrypt(const std::string& data, const QuantumKey& key);
    std::string latticeDecrypt(const std::string& encrypted, const QuantumKey& key);
    QuantumKey generateCodeKey(int code_length);
    Signature codeSign(const std::string& message, const QuantumKey& key);
    QuantumKey generateMQKey(int variables);
    ZKProof mqProve(const std::string& statement, const QuantumKey& key);

    // Zero-Knowledge Proofs
    std::unique_ptr<IZeroKnowledge> createZKSNARK();
    std::unique_ptr<IZeroKnowledge> createZKSTARK();
    std::unique_ptr<IZeroKnowledge> createBulletproof();

    // Multi-Signature
    std::unique_ptr<IMultiSignature> createThresholdSignature(int total, int threshold);
    std::unique_ptr<IMultiSignature> createRingSignature(int participants);
    std::unique_ptr<IMultiSignature> createMPC(int parties);

    // Homomorphic Encryption
    std::unique_ptr<IHomomorphicEncryption> createHomomorphicEncryption();

    // Hardware Acceleration
    std::unique_ptr<IHardwareAccelerator> createGPUAccelerator();
    std::unique_ptr<IHardwareAccelerator> createFPGAAccelerator();
    std::unique_ptr<IHardwareAccelerator> createASICSimulator();

    // Blockchain Integration
    std::unique_ptr<IBlockchainInterface> createBitcoinInterface();
    std::unique_ptr<IBlockchainInterface> createEthereumInterface();
    std::unique_ptr<IBlockchainInterface> createSolanaInterface();

    // Utilitários
    std::string sha256(const std::string& data);
    std::string sha512(const std::string& data);
    std::string ripemd160(const std::string& data);
    std::string keccak256(const std::string& data);
    std::string randomBytes(int length);
    std::string base58Encode(const std::string& data);
    std::string base58Decode(const std::string& encoded);
    std::string hexEncode(const std::string& data);
    std::string hexDecode(const std::string& hex);

    // Benchmark e performance
    void runBenchmark();
    void setOptimizationLevel(int level);
    void enableHardwareAcceleration(bool enable);
    void setThreadCount(int threads);

    // Configuração
    void setSecurityLevel(int bits);
    void setCurveType(const std::string& type);
    void setHashAlgorithm(const std::string& algorithm);
    void setRandomSource(const std::string& source);

    // Logging e debug
    void enableLogging(bool enable);
    void setLogLevel(const std::string& level);
    void log(const std::string& message);
    void clearLogs();
    std::vector<std::string> getLogs();

    // Validação e testes
    bool validateKeyPair(const KeyPair& keypair);
    bool validateSignature(const Signature& signature);
    bool validateAddress(const std::string& address);
    void runSelfTest();
    bool isHealthy();

    // Serialização
    std::string serializeKeyPair(const KeyPair& keypair);
    KeyPair deserializeKeyPair(const std::string& serialized);
    std::string serializeSignature(const Signature& signature);
    Signature deserializeSignature(const std::string& serialized);

    // Criptografia simétrica
    std::string aesEncrypt(const std::string& data, const std::string& key);
    std::string aesDecrypt(const std::string& encrypted, const std::string& key);
    std::string chacha20Encrypt(const std::string& data, const std::string& key, const std::string& nonce);
    std::string chacha20Decrypt(const std::string& encrypted, const std::string& key, const std::string& nonce);

    // Funções de derivação
    std::string pbkdf2(const std::string& password, const std::string& salt, int iterations, int key_length);
    std::string scrypt(const std::string& password, const std::string& salt, int n, int r, int p, int key_length);
    std::string argon2(const std::string& password, const std::string& salt, int iterations, int memory, int parallelism, int key_length);

    // Funções de compromisso
    std::string pedersenCommit(const std::string& value, const std::string& blinding);
    bool pedersenVerify(const std::string& commitment, const std::string& value, const std::string& blinding);
    std::string bulletproofCommit(const std::string& value, const std::string& blinding);
    bool bulletproofVerify(const std::string& commitment, const std::string& value, const std::string& blinding);
};

// Funções de criação
extern "C" {
    AdilsonCrypto* createAdilsonCrypto();
    void destroyAdilsonCrypto(AdilsonCrypto* crypto);
}

// Constantes
const int SECURITY_LEVEL_128 = 128;
const int SECURITY_LEVEL_256 = 256;
const int SECURITY_LEVEL_384 = 384;
const int SECURITY_LEVEL_512 = 512;

const std::string CURVE_SECP256K1 = "secp256k1";
const std::string CURVE_SECP384R1 = "secp384r1";
const std::string CURVE_SECP521R1 = "secp521r1";
const std::string CURVE_BRAINPOOLP512T1 = "brainpoolP512t1";

const std::string HASH_SHA256 = "sha256";
const std::string HASH_SHA512 = "sha512";
const std::string HASH_RIPEMD160 = "ripemd160";
const std::string HASH_KECCAK256 = "keccak256";

#endif // ADILSONCRYPTO_H 