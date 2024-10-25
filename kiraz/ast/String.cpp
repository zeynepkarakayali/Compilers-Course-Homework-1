#include "String.h"
#include<cassert>
#include <kiraz/token/String.h>

namespace ast {

StringLiteral::StringLiteral(Token::Ptr t) : Node(L_STRING){

    assert(t->get_id() == L_STRING);
    auto token_str = std::static_pointer_cast<const token::StringLiteral>(t);
    try{
        m_value = token_str->get_value();
    }
    catch (std::out_of_range &e){
        // TODO mark this node as invalid
    }
} 

}