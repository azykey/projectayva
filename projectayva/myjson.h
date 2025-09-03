/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Código original e protegido
 * Este parser JSON é exclusivo, feito do zero, e assinado digitalmente.
 */
#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>

// Enum para tipos de valor JSON
enum class MyJsonTipo {
    Nulo, Booleano, Numero, String, Objeto, Array
};

// Forward declaration
class MyJson;

// Definição de Objeto e Array
using MyJsonObjeto = std::map<std::string, std::shared_ptr<MyJson>>;
using MyJsonArray = std::vector<std::shared_ptr<MyJson>>;

// Classe principal de valor JSON
class MyJson {
public:
    MyJsonTipo tipo;
    std::variant<std::nullptr_t, bool, double, std::string, MyJsonObjeto, MyJsonArray> valor;

    MyJson();
    MyJson(bool b);
    MyJson(double n);
    MyJson(const std::string& s);
    MyJson(const MyJsonObjeto& o);
    MyJson(const MyJsonArray& a);

    // Métodos utilitários
    bool isNulo() const;
    bool isBooleano() const;
    bool isNumero() const;
    bool isString() const;
    bool isObjeto() const;
    bool isArray() const;

    // Getters seguros
    bool getBooleano() const;
    double getNumero() const;
    const std::string& getString() const;
    const MyJsonObjeto& getObjeto() const;
    const MyJsonArray& getArray() const;

    // Serialização
    std::string toString(int indent = 0) const;

    // Parsing
    static std::shared_ptr<MyJson> parse(const std::string& texto);

    // Futuro: suporte a comentários, unicode, validação, etc.
}; 