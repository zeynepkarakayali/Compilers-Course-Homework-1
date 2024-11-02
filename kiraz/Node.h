#ifndef KIRAZ_NODE_H
#define KIRAZ_NODE_H

#include <cassert>
#include <vector>
#include <queue>

#include <fmt/format.h>

#include <kiraz/Token.h>

extern int yylineno;

class Node {
public:
    using Ptr = std::shared_ptr<Node>; // can be read and modified
    using Cptr = std::shared_ptr<const Node>; // can be read but not modified

    Node(int id) : m_id(id) {}
    virtual ~Node();

    virtual std::string as_string() const = 0; // Bu satır, soyut bir sınıf içinde yer alan, uygulanması zorunlu bir fonksiyonu ifade eder. 
                                            //Fonksiyonun görevi, bir nesneyi string (metin) haline getirmektir, 
                                            //ancak temel sınıfta (base class) bunun nasıl yapılacağı belirtilmez. 
                                            //Bu fonksiyonun ne yapacağı, türetilmiş sınıflar tarafından belirlenmelidir.
    void print() { fmt::print("{}\n", as_string()); }

    auto get_id() const { return m_id; }
    void set_pos(int l, int c) {
        m_line = l;
        m_col = c;
    }

    template <typename T, typename... Args>
    static auto add(Args &&...args) {
        auto root = std::make_shared<T>(std::forward<Args>(args)...);
        root->set_pos(yylineno, Token::colno);
        s_roots.back() = root;
        return root;
    }

    static void reset_root() { s_roots.emplace_back(); }// The parameter is added to the vector at the end position.
    static auto &current_root() {
        assert(! s_roots.empty()); // Vector must not be empty
        return s_roots.back(); // The back() method returns a reference to the last element in the vector.
    }

private:
    static std::vector<Node::Ptr> s_roots;

    int m_id;
    int m_line = 0;
    int m_col = 0;
};


class NodeQueue : public Node {
public:
    NodeQueue() : Node(0) {} // Initialize Node with id 0

    // Enqueue a node
    void push(Node::Ptr node) {
        m_queue.push(node);
    }

    // Dequeue a node
    Node::Ptr pop() {
        if (m_queue.empty()) {
            return nullptr;
        }
        Node::Ptr node = m_queue.front();
        m_queue.pop();
        return node;
    }

    // Check if the queue is empty
    bool empty() const {
        return m_queue.empty();
    }

    // Get the size of the queue
    size_t size() const {
        return m_queue.size();
    }

    // Override the as_string method
    std::string as_string() const override {
        return "NodeQueue";
    }

private:
    std::queue<Node::Ptr> m_queue;
};

// Declare nodeQueue as an extern variable
extern NodeQueue nodeQueue;

#endif // KIRAZ_NODE_H
