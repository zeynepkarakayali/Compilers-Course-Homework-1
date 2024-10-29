#include "Identifier.h"

namespace ast {

class Identifier {
public:
    Identifier(std::shared_ptr<Token> token) : token_(token) {}

private:
    std::shared_ptr<Token> token_;
};

}