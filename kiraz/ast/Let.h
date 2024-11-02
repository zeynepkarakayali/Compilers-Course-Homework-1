#include <kiraz/Node.h>
#include <memory>
#include <fmt/core.h>

namespace ast {

class Let : public Node {
protected:
    Ptr m_left;  // identifier icin
    Ptr m_right; // expression icin

public:
    Let(const Node::Ptr &left, const Node::Ptr &right)
        : Node(KW_LET), m_left(left), m_right(right) {
        assert(m_left);
        assert(m_right);
    }

    auto get_left() const { return m_left; }
    auto get_right() const { return m_right; }

    std::string as_string() const override {
        return fmt::format("Let(n={}, t=Id({}), i={})",
                           get_left()->as_string(),
                           get_left()->get_id(),
                           get_right()->as_string());
    }
};

}