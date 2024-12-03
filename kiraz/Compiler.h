#ifndef COMPILER_H
#define COMPILER_H

#include <cassert>
#include <map>

#include "Node.h"
#include "Token.h"
#include <lexer.hpp>
#include <iostream>



enum class ScopeType {
    Module,
    Class,
    Func,
    Method,
};


struct Scope {
    using SymTab = std::map<std::string, Node::Ptr>;

    Scope(const std::map<std::string, Node::Ptr> &map, ScopeType stype, Node::Ptr s )
            : symbols(map), scope_type(stype), stmt(s) {}

    SymTab symbols;
    ScopeType scope_type;
    Node::Ptr stmt;

    std::vector<std::shared_ptr<Scope>> subscopes; // alt scope'lara ulasmak icin

    auto find(const std::string &s) { return symbols.find(s); }
    auto find(const std::string &s) const { return symbols.find(s); }
    auto end() { return symbols.end(); }
    auto end() const { return symbols.end(); }

    decltype(auto) operator[](const std::string &s) { return (symbols[s]); }
    Node::SymTabEntry get_symbol(const std::string &name) const {
        auto iter = symbols.find(name);
        if (iter == symbols.end()) {
            return name;
        }
        return {name, iter->second};
    }
};

class SymbolTable {
public:
    struct ScopeRef {
        ScopeRef(SymbolTable &s) : symtab(s) {}
        ScopeRef(const ScopeRef &) = delete;
        ScopeRef(ScopeRef &&) = default;
        ScopeRef &operator=(const ScopeRef &) = delete;

        ~ScopeRef() { symtab.exit_scope(); }

        SymbolTable &symtab;
    };

    friend class ScopeRef;

    explicit SymbolTable();
    explicit SymbolTable(ScopeType);

    virtual ~SymbolTable();

    Node::Ptr add_symbol(const std::string &name, Node::Ptr m) {
        assert(! name.empty());
        (*m_symbols.back())[name] = m;
        return m;
    }

    Node::SymTabEntry get_symbol(const std::string &name) const {
        return m_symbols.back()->get_symbol(name);
    }

    ScopeRef enter_scope(ScopeType scope_type, Node::Ptr stmt) {
    assert(stmt->get_cur_symtab() == m_symbols.back());
    // eski scope'u kopyalamak yerine yeni scope acilmasi icin
    auto new_scope = std::make_shared<Scope>(std::map<std::string, Node::Ptr>(), scope_type, stmt); 

    m_symbols.back()->subscopes.push_back(new_scope); // yeni scope'u subscope olarak eklemek icin
    m_symbols.emplace_back(new_scope); // yeni scope'u stack'e eklemek icin

    assert(m_symbols.size() > 1);
    return ScopeRef(*this);
}


    void print_symbols(const std::shared_ptr<Scope>& scope, size_t level = 0) const {
        std::string indent(level * 2, ' ');
        std::string sct;
        switch (scope->scope_type) {
            case ScopeType::Module: sct = "Module"; break;
            case ScopeType::Class: sct = "Class"; break;
            case ScopeType::Func: sct = "Func"; break;
            case ScopeType::Method: sct = "Method"; break;
            default: sct = "Unknown"; break;
        }

        fmt::print("{}Scope {} (Type: {}):\n", indent, level, sct);
        for (const auto& [name, node] : scope->symbols) {
            fmt::print("{}  Name: {:<15}", indent, name);
            if (node) {
                fmt::print("    Type: {}", node->as_string());
            }
            fmt::print("\n");
        }

        // subscope'lar icin
        for (const auto& subscope : scope->subscopes) {
            print_symbols(subscope, level + 1);
        }
    }

    
    void print_symbols() const {
        fmt::print("Symbol Table Contents:\n");
        if (!m_symbols.empty()) {
            print_symbols(m_symbols.front()); // Module scope'tan başlayarak bastır
        }
    }


    auto get_cur_symtab() { return m_symbols.back(); }
    auto get_cur_symtab() const { return m_symbols.back(); }
    auto get_scope_type() const { return m_symbols.back()->scope_type; }
    auto get_scope_stmt() const { return m_symbols.back()->stmt; }

    static auto get_module_io() { return s_module_io; }

private:
    void exit_scope() { m_symbols.pop_back(); }

    std::vector<std::shared_ptr<Scope>> m_symbols;

    static Node::Ptr s_module_ki;
    static Node::Ptr s_module_io;
};

class Compiler {
public:
    static Compiler *current() { return s_current; }
    Compiler();

    int compile_file(const std::string &file_name, std::ostream &ostr);
    int compile_string(const std::string &str, std::ostream &ostr);
    Node::Ptr compile_module(const std::string &str);

    static void reset_parser();
    void reset();
    void set_error(const std::string &str) { m_error = str; }
    const auto &get_error() const { return m_error; }

    ~Compiler();

protected:
    int compile(Node::Ptr root, std::ostream &ostr);

private:
    YY_BUFFER_STATE buffer = nullptr;
    std::string m_error;
    static Compiler *s_current;
};
#endif // COMPILER_H
