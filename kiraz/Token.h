#ifndef KIRAZ_TOKEN_H
#define KIRAZ_TOKEN_H

#include "main.h"

#include <fmt/format.h>
#include <string>

class Token {
public:
    using Ptr = std::shared_ptr<Token>;
    using Cptr = std::shared_ptr<const Token>;

    Token(int id) : m_id(id) {}
    virtual ~Token(); // Remove `= default`

    static auto New() {}

    virtual std::string as_string() const = 0;
    void print() { fmt::print("{}\n", as_string()); }

    template <typename T, typename... Args>
    static auto New(Args &&...args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    static int colno; 

    auto get_id() const { return m_id; }

private:
    int m_id;
};

#endif // KIRAZ_TOKEN_H