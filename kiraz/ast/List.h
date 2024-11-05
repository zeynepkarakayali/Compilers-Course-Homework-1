#ifndef KIRAZ_AST_LIST_H
#define KIRAZ_AST_LIST_H

#include <kiraz/Node.h>
#include <sstream>

namespace ast {


class Argument : public Node {
public:
    using Ptr = std::shared_ptr<Argument>;

    Argument(Node::Ptr name, Node::Ptr type)
        : name(name), type(type) {}

    Node::Ptr get_name() const { return name; }
    Node::Ptr get_type() const { return type; }

    std::string as_string() const override {
        return "FArg(n=" + name->as_string() + ", t=" + type->as_string() + ")";
    }

private:
    Node::Ptr name;
    Node::Ptr type;
};

class ArgList : public Node {
public:
    using Ptr = std::shared_ptr<ArgList>;

    ArgList() = default;

    void add_argument(const Argument::Ptr& arg) {
        arguments.push_back(arg);
    }

    const std::vector<Argument::Ptr>& get_arguments() const {
        return arguments;
    }
    std::string as_string() const override{
        std::ostringstream oss;
        for (size_t i = 0; i < arguments.size(); ++i) {
            oss << arguments[i]->as_string();
            if (i < arguments.size() - 1) {
                oss << ", ";  
            }
        }
        return oss.str();

    }

private:
    std::vector<Argument::Ptr> arguments; 
};



}

#endif
