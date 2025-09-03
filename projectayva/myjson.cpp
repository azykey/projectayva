/*
 * Propriedade Intelectual: Adilson Oliveira 2025
 * Código original e protegido
 * Este parser JSON é exclusivo, feito do zero, e assinado digitalmente.
 */
#include "myjson.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

// Construtores
MyJson::MyJson() : tipo(MyJsonTipo::Nulo), valor(nullptr) {}
MyJson::MyJson(bool b) : tipo(MyJsonTipo::Booleano), valor(b) {}
MyJson::MyJson(double n) : tipo(MyJsonTipo::Numero), valor(n) {}
MyJson::MyJson(const std::string& s) : tipo(MyJsonTipo::String), valor(s) {}
MyJson::MyJson(const MyJsonObjeto& o) : tipo(MyJsonTipo::Objeto), valor(o) {}
MyJson::MyJson(const MyJsonArray& a) : tipo(MyJsonTipo::Array), valor(a) {}

// Métodos utilitários
bool MyJson::isNulo() const { return tipo == MyJsonTipo::Nulo; }
bool MyJson::isBooleano() const { return tipo == MyJsonTipo::Booleano; }
bool MyJson::isNumero() const { return tipo == MyJsonTipo::Numero; }
bool MyJson::isString() const { return tipo == MyJsonTipo::String; }
bool MyJson::isObjeto() const { return tipo == MyJsonTipo::Objeto; }
bool MyJson::isArray() const { return tipo == MyJsonTipo::Array; }

bool MyJson::getBooleano() const { return std::get<bool>(valor); }
double MyJson::getNumero() const { return std::get<double>(valor); }
const std::string& MyJson::getString() const { return std::get<std::string>(valor); }
const MyJsonObjeto& MyJson::getObjeto() const { return std::get<MyJsonObjeto>(valor); }
const MyJsonArray& MyJson::getArray() const { return std::get<MyJsonArray>(valor); }

// Serialização (toString)
std::string MyJson::toString(int indent) const {
    std::ostringstream oss;
    switch (tipo) {
        case MyJsonTipo::Nulo:
            oss << "null";
            break;
        case MyJsonTipo::Booleano:
            oss << (getBooleano() ? "true" : "false");
            break;
        case MyJsonTipo::Numero:
            oss << getNumero();
            break;
        case MyJsonTipo::String:
            oss << '"' << getString() << '"';
            break;
        case MyJsonTipo::Objeto: {
            oss << "{";
            bool first = true;
            for (const auto& [k, v] : getObjeto()) {
                if (!first) oss << ", ";
                first = false;
                oss << '"' << k << '"' << ": " << v->toString(indent + 2);
            }
            oss << "}";
            break;
        }
        case MyJsonTipo::Array: {
            oss << "[";
            bool first = true;
            for (const auto& v : getArray()) {
                if (!first) oss << ", ";
                first = false;
                oss << v->toString(indent + 2);
            }
            oss << "]";
            break;
        }
    }
    return oss.str();
}

// Função auxiliar para pular espaços
static void skipSpaces(const std::string& s, size_t& i) {
    while (i < s.size() && std::isspace(s[i])) ++i;
}

// Parsing básico (aprimorável para unicode, comentários, etc)
std::shared_ptr<MyJson> MyJson::parse(const std::string& texto) {
    size_t i = 0;
    return parseValue(texto, i);
}

// Funções auxiliares de parsing
static std::shared_ptr<MyJson> parseValue(const std::string& s, size_t& i);
static std::shared_ptr<MyJson> parseObjeto(const std::string& s, size_t& i);
static std::shared_ptr<MyJson> parseArray(const std::string& s, size_t& i);
static std::shared_ptr<MyJson> parseString(const std::string& s, size_t& i);
static std::shared_ptr<MyJson> parseNumero(const std::string& s, size_t& i);

static std::shared_ptr<MyJson> parseValue(const std::string& s, size_t& i) {
    skipSpaces(s, i);
    if (i >= s.size()) throw std::runtime_error("JSON inesperado");
    if (s[i] == '{') return parseObjeto(s, i);
    if (s[i] == '[') return parseArray(s, i);
    if (s[i] == '"') return parseString(s, i);
    if (std::isdigit(s[i]) || s[i] == '-') return parseNumero(s, i);
    if (s.compare(i, 4, "true") == 0) { i += 4; return std::make_shared<MyJson>(true); }
    if (s.compare(i, 5, "false") == 0) { i += 5; return std::make_shared<MyJson>(false); }
    if (s.compare(i, 4, "null") == 0) { i += 4; return std::make_shared<MyJson>(); }
    throw std::runtime_error("Valor JSON inválido");
}

static std::shared_ptr<MyJson> parseString(const std::string& s, size_t& i) {
    if (s[i] != '"') throw std::runtime_error("Esperado \" para string");
    ++i;
    std::string out;
    while (i < s.size() && s[i] != '"') {
        if (s[i] == '\\') {
            ++i;
            if (i >= s.size()) throw std::runtime_error("Escape inválido");
            out += s[i]; // Simples, pode expandir para escapes reais
        } else {
            out += s[i];
        }
        ++i;
    }
    if (i >= s.size() || s[i] != '"') throw std::runtime_error("String não terminada");
    ++i;
    return std::make_shared<MyJson>(out);
}

static std::shared_ptr<MyJson> parseNumero(const std::string& s, size_t& i) {
    size_t start = i;
    if (s[i] == '-') ++i;
    while (i < s.size() && std::isdigit(s[i])) ++i;
    if (i < s.size() && s[i] == '.') {
        ++i;
        while (i < s.size() && std::isdigit(s[i])) ++i;
    }
    double val = std::stod(s.substr(start, i - start));
    return std::make_shared<MyJson>(val);
}

static std::shared_ptr<MyJson> parseArray(const std::string& s, size_t& i) {
    if (s[i] != '[') throw std::runtime_error("Esperado [ para array");
    ++i;
    MyJsonArray arr;
    skipSpaces(s, i);
    if (s[i] == ']') { ++i; return std::make_shared<MyJson>(arr); }
    while (true) {
        arr.push_back(parseValue(s, i));
        skipSpaces(s, i);
        if (s[i] == ']') { ++i; break; }
        if (s[i] != ',') throw std::runtime_error("Esperado , em array");
        ++i;
    }
    return std::make_shared<MyJson>(arr);
}

static std::shared_ptr<MyJson> parseObjeto(const std::string& s, size_t& i) {
    if (s[i] != '{') throw std::runtime_error("Esperado { para objeto");
    ++i;
    MyJsonObjeto obj;
    skipSpaces(s, i);
    if (s[i] == '}') { ++i; return std::make_shared<MyJson>(obj); }
    while (true) {
        skipSpaces(s, i);
        auto chave = parseString(s, i)->getString();
        skipSpaces(s, i);
        if (s[i] != ':') throw std::runtime_error("Esperado : em objeto");
        ++i;
        skipSpaces(s, i);
        obj[chave] = parseValue(s, i);
        skipSpaces(s, i);
        if (s[i] == '}') { ++i; break; }
        if (s[i] != ',') throw std::runtime_error("Esperado , em objeto");
        ++i;
    }
    return std::make_shared<MyJson>(obj);
}

// Futuro: suporte a comentários, unicode, validação, etc. 