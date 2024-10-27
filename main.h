
#pragma once

#include <memory>

class Node;
<<<<<<< HEAD
#define YYSTYPE std::shared_ptr<Node>
=======

extern "C" int yylex(void);
#define YY_DECL int yylex(void)
#define YYSTYPE std::shared_ptr<Node>

>>>>>>> original-repo/master
#include "parser.hpp"
