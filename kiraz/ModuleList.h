#ifndef KIRAZ_MODULELIST_H
#define KIRAZ_MODULELIST_H

#include "Node.h"
#include <vector>
#include <memory>
#include <stdexcept>

class ModuleList : public NodeList {
public: 
    using NodeListPtr = std::shared_ptr<NodeList>;

    static constexpr size_t MAX_NODES = 4;

    ModuleList() : NodeList(),
                   imports(std::make_shared<NodeList>()),
                   classes(std::make_shared<NodeList>()),
                   functions(std::make_shared<NodeList>()),
                   generalScopeStatements(std::make_shared<NodeList>()) {}

    ~ModuleList() override = default;

    NodeListPtr imports;
    NodeListPtr classes;
    NodeListPtr functions;
    NodeListPtr generalScopeStatements;

    std::string as_string() const override {
        return "Module([" + imports->as_string() + ", " + classes->as_string() + ", " +
               functions->as_string() + ", " + generalScopeStatements->as_string() + "])";
    }

};

#endif // KIRAZ_MODULELIST_H