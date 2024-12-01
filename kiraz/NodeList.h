#ifndef KIRAZ_NODELIST_H
#define KIRAZ_NODELIST_H

#include "Node.h"
#include <queue>
#include <memory>
#include <string>

class NodeList : public Node {
public:
    //using NodePtr = std::shared_ptr<Node>;


    explicit NodeList() : Node() {}

    ~NodeList() override = default;

    void addNode(const Node::Ptr& node) {
        nodeQueue.push(node);
    }

    std::string as_string() const override {

        std::string result= "";

        std::queue<Node::Ptr> tempQueue = nodeQueue; 

        while (!tempQueue.empty()) {
            result += tempQueue.front()->as_string() + ", "; 
            tempQueue.pop();
        }
        result.resize(result.size() - 2);

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

    std::vector<Node::Ptr> get_nodes() const {
        std::vector<Node::Ptr> nodes;
        std::queue<Node::Ptr> tempQueue = nodeQueue;
        while (!tempQueue.empty()) {
            nodes.push_back(tempQueue.front());
            tempQueue.pop();
        }
        return nodes;
    }


private:
    std::queue<Node::Ptr> nodeQueue;
};

#endif // KIRAZ_NODELIST_H