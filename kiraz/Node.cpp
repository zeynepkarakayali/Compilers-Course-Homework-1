
#include "Node.h"

/*
Node::s_roots: This part defines a static member variable named s_roots for the class Node. 
Static member variables are shared across all instances of the class, meaning there is only one copy of s_roots for all objects of type Node. 
It belongs to the class itself, not to individual objects.

Static Member Declaration and Definition:
In C++, static member variables must be defined outside the class definition if they are declared inside the class. 
This is necessary because static members exist independently of any object instances.

*****
std::vector<Node::Ptr> Node::s_roots;
This defines s_roots as a static member variable of the Node class.
It belongs to the class itself, not to any particular instance of the class.

td::vector<Node::Ptr> s_roots;
It is not associated with the Node class, but it can still store Node::Ptr (shared pointers to Node objects).
*/

std::vector<Node::Ptr> Node::s_roots;
Token::Ptr curtoken;

Node::~Node() {}