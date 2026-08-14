#include <iostream>
#include <memory>
#include <vector>

class Parent; // Forward declaration of Parent class

class Child {
    private:
        std::weak_ptr<Parent> parent; // Use weak_ptr to avoid circular reference and memory leak

    public:
        void setParent(std::shared_ptr<Parent> p) {
            parent = p;
        }

        void CheckParent() {
            if (auto p = parent.lock()) { // Check if the parent is still valid
                std::cout << "Parent is valid." << std::endl;
            } else {
                std::cout << "Parent has been destroyed." << std::endl;
            }
        }
};

class Parent {
    private:
        std::vector<std::shared_ptr<Child>> children;
    public:
        void addChild(std::shared_ptr<Child> child) {
            
            children.push_back(child); // Add the child to the vector of children
        }
};

int main() {
    // Create a parent and a child, and set the parent for the child
    auto parent = std::make_shared<Parent>();
    auto child1 = std::make_shared<Child>();
    auto child2 = std::make_shared<Child>();
    auto child3 = std::make_shared<Child>();

    parent->addChild(child1); // Add child1 to the parent's children vector
    child1->setParent(parent); // Set the parent for child1
    child1->CheckParent(); // Check if the parent is valid

    parent->addChild(child2); // Add child2 to the parent's children vector
    child2->setParent(parent); // Set the parent for child2
    child2->CheckParent(); // Check if the parent is valid

    parent->addChild(child3); // Add child3 to the parent's children vector
    child3->setParent(parent); // Set the parent for child3 
    child3->CheckParent(); // Check if the parent is valid

    // Reset the parent shared pointer to simulate the parent being destroyed
    parent.reset(); // This will destroy the parent object

    child1->CheckParent(); // Check if the parent is valid after destruction
    child2->CheckParent(); // Check if the parent is valid after destruction
    child3->CheckParent(); // Check if the parent is valid after destruction

    

    return 0;
}
