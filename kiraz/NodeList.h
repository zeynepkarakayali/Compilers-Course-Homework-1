#ifndef KIRAZ_NODELIST_H
#define KIRAZ_NODELIST_H

#include "Node.h"
#include <queue>
#include <memory>

class NodeList : public Node {
public:
    using NodePtr = std::shared_ptr<Node>;

    // Temel Node sinifina bir id aktarir
    NodeList(int id) : Node(id) {}

    ~NodeList() override = default;

    void addNode(const NodePtr& node) {
        nodeQueue.push(node);
    }

    // NodeList'in iceriklerini duzgun sekilde formatlayip bastirmak icin
    std::string as_string() const override {
        std::string result = "[ "; // NodeList adini testler yuzunden sildim
        std::queue<NodePtr> tempQueue = nodeQueue; // kuyrugu gezmek icin temp

        while (!tempQueue.empty()) {
            result += tempQueue.front()->as_string() + "; "; //terimleri ayirmak icin ;
            tempQueue.pop();
        }
        result += "]";
        return result;
    }

    // Node'lari islemek icin
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

private:
    std::queue<NodePtr> nodeQueue;
};

#endif // KIRAZ_NODELIST_H