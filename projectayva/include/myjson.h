/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Parser JSON original — código 100% autoral
 * Assinatura digital: SHA-256 do arquivo.
 */
#pragma once
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <memory>

enum class MyJsonTipo {
    Nulo, Booleano, Numero, String, Objeto, Array
};

class MyJson {
public:
    MyJsonTipo tipo;
    std::variant<std::nullptr_t, bool, double, std::string, std::map<std::string, std::shared_ptr<MyJson>>, std::vector<std::shared_ptr<MyJson>>> valor;

    MyJson();
    MyJson(bool b);
    MyJson(double n);
    MyJson(const std::string& s);
    MyJson(const std::map<std::string, std::shared_ptr<MyJson>>& o);
    MyJson(const std::vector<std::shared_ptr<MyJson>>& a);

    bool isNulo() const;
    bool isBooleano() const;
    bool isNumero() const;
    bool isString() const;
    bool isObjeto() const;
    bool isArray() const;

    bool getBooleano() const;
    double getNumero() const;
    const std::string& getString() const;
    const std::map<std::string, std::shared_ptr<MyJson>>& getObjeto() const;
    const std::vector<std::shared_ptr<MyJson>>& getArray() const;

    std::string toString(int indent = 0) const;
    static std::shared_ptr<MyJson> parse(const std::string& texto);
}; 