
#include <cassert>
#include <cstdio>
#include <string_view>

#include "lexer.hpp"
#include "main.h"
#include "parser.hpp"

#include <kiraz/Node.h>

extern int yydebug;

enum Status {
    OK,
    ERR,
};

enum Mode {
    MODE_UNKNOWN,
    MODE_FILE,
    MODE_TEXT,
    MODE_HELP,
};

static int test(std::string_view str) {
    auto buffer = yy_scan_string(str.data());  //  Normalde lexer, dosya veya standart girişten veri okur. yy_scan_string, belirli bir diziyi bu girişe entegre etmenin bir yolunu sunar.
                                                //yy_scan_string çağrıldığında, bu dize için bir giriş tamponu (buffer) oluşturur ve bu tamponu lexer için hazırlar.
    auto ret = yyparse(); // Bison yy_lex fonksiyonunu çağırır. Lexer, dizedeki karakterleri okur ve tanımlı kurallara göre token'lara dönüştürür. Daha sonrasında lexer’dan alınan token'ları kullanarak verinin yapılandırılmış biçimde doğru olup olmadığını kontrol eder.
                        // Durum kodu gönderir.
    yy_delete_buffer(buffer);

    if (Node::current_root()) {
        fmt::print("{}\n", Node::current_root()->as_string());
    }

    return ret;
}

static int usage(int argc, char **argv) {
    fmt::print("Usage: {} -s [string to parse] ....\n", argv[0]);
    fmt::print("       {} -f [file to parse] ....\n", argv[0]);
    fmt::print("       {} -h Show this help\n", argv[0]);

    return ERR;
}

static int handle_mode_text(std::string_view arg) {
    if (auto ret = test(arg); ret != OK) {
        return ret;
    }

    return OK;
}

static int handle_mode_file(std::string_view arg) {
    fmt::print("TODO\n");
    return ERR;
}

int main(int argc, char **argv) {
    yydebug = 0;

    static Mode mode = MODE_UNKNOWN;

    if (argc < 2) {
        return usage(argc, argv);
    }

    for (auto i = 1; i < argc; ++i) {
        Node::reset_root();

        std::string_view arg(argv[i]); // std::string_view s{ "Hello, world!" };

        if (mode == MODE_UNKNOWN){
            if (arg == "-f") {
                mode = MODE_FILE;
                continue;
            }

            if (arg == "-s") {
                mode = MODE_TEXT;
                continue;
            }

            if (arg == "-h") {
                mode = MODE_HELP;
                continue;
            }
        }

        switch (mode) {
        default:
        case MODE_HELP:
            return usage(argc, argv);


       //if-with-initializer, semicolon in if statement: It allows you to both declare a variable and check a condition in a more concise way.
       //auto: the type of the variable that is being declared will be automatically deducted from its initializer
        case MODE_FILE:
            if (auto ret = handle_mode_file(argv[i]); ret != OK) { 
                return ret;
            }
            return 1;

        case MODE_TEXT:
            if (auto ret = handle_mode_text(argv[i]); ret != OK) {
                return ret;
            }
            break;
        }

        mode = MODE_UNKNOWN;
    }

    if (mode != MODE_UNKNOWN) {
        return usage(argc, argv);
    }

    return 0;
}
