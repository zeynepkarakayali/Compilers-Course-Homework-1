#ifndef KIRAZ_NODELIST_H
#define KIRAZ_NODELIST_H

#include "Node.h"
#include <queue>
#include <memory>
#include <string>

class NodeList : public Node {
public:
    using NodePtr = std::shared_ptr<Node>;

    enum Mode {
        WITH_MODULE,
        WITHOUT_MODULE
    };

    explicit NodeList(Mode mode = WITH_MODULE) : Node(), mode(mode) {}

    ~NodeList() override = default;

    void addNode(const NodePtr& node) {
        nodeQueue.push(node);
    }

    std::string as_string() const override {
        std::string result= "";
        if (mode == WITH_MODULE) {
            result += "Module([";
        } 
        std::queue<NodePtr> tempQueue = nodeQueue; 

        while (!tempQueue.empty()) {
            result += tempQueue.front()->as_string() + ", ";
            tempQueue.pop();
        }
        result.resize(result.size() - 2);
        if (mode == WITH_MODULE) {
            result += "])";
        }
        return result;
    }

    void processNodes() {
        while (!nodeQueue.empty()) {
            auto node = nodeQueue.front();
            nodeQueue.pop();

            node->print();
        }
    }

    size_t getQueueSize() const {
        return nodeQueue.size();
    }

    std::vector<NodePtr> get_nodes() const {
        std::vector<NodePtr> nodes;
        std::queue<NodePtr> tempQueue = nodeQueue;
        while (!tempQueue.empty()) {
            nodes.push_back(tempQueue.front());
            tempQueue.pop();
        }
        return nodes;
    }
    void setMode(Mode newMode) {
        mode = newMode;
    }

private:
    std::queue<NodePtr> nodeQueue;
    Mode mode;
};

#endif // KIRAZ_NODELIST_H