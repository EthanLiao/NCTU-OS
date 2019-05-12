#include <iostream>

/**
 * Binary Search Tree Node structure
*/
using namespace std;
struct Node
{
    int value;

    Node *right;
    Node *left;

    Node(int value)
    {
        this->value = value;
        this->right = NULL;
        this->left = NULL;
    }
};
/**
 * Binary Search Tree class
*/
class BST
{
    private:
        // Root node
        Node *root;

        // Insert private operation
        void insert(Node *root, int value);

        // Remove private operation
        bool remove(Node *parent, Node *current, int value);

        // Find next larger node for deletion
        Node* nextLarger(Node *node);
        // Find node with the given value
        Node* findNodeWithValue(int value);

        // Printing
        void print(Node *node);

    public:
        void insert(int value);
        bool remove(int value);

        // Find node with minimum value
        Node* findMin();
        // Find node with maximum value
        Node* findMax();
        // Return root node
        Node* getRoot();

        void printTree();
};

Node* BST::findNodeWithValue(int value)
{
    // Save the current node, starting with root
    Node *current = root;

    // Loop for left/right node while current node is not NULL
    while (current)
    {
        if (current->value == value)
            break;

        if (value > current->value)
            current = current->right;
        else
            current = current->left;

    }
    return current;
}

Node* BST::nextLarger(Node *node)
{
    // Start with the right node
    Node *nextLarger = node->right;

    // Get left node while it's not null
    while (nextLarger->left)
    {
        nextLarger = nextLarger->left;
    }

    return nextLarger;
}
void BST::insert(Node *root, int value)
{

    if (value > root->value)
    {
        // Higher value than root
        if (!root->right)
            // Insert on the right of the root
            root->right = new Node(value);
        else
            // Go deeper
            insert(root->right, value);
    }
    else
    {
        // Lower value than root

        if (!root->left)
            // Insert on the left of the root
            root->left = new Node(value);
        else
            // Go deeper
            insert(root->left, value);
    }
}
void BST::insert(int value)
{

    // If tree is empty, new node becomes root
    if (!root){
        root = new Node(value);
    }
    else
    {
        this->insert(root, value);
    }

}

bool BST::remove(Node *parent, Node *current, int value)
{
    if (!current)
    {
        // Did not find any node with the given value
        return false;
    }
    // Right node to remove
    if (current->value == value)
    {
        // Check if node is leaf
        if (!current->left && !current->right)
        {
            // Found node and is leaf
            // Remove and return
            if (parent->right == current)
                parent->right = NULL;
            else
                parent->left = NULL;
            delete current;
            current = NULL;
            return true;
        }
        // Check if node to delete has one child
        else if (!current->left || !current->right)
        {
            // Get the only child
            Node *child = current->left;
            if (!child)
                child = current->right;


            // If deleting root node, child becomes root
            if (!parent)
            {
                this->root = child;
            }
            else
            {
                // Parent now points to current's child
                if (child->value > parent->value)
                    parent->right = child;
                else
                    parent->left = child;
            }
            delete current;
            current = NULL;
        }

        // Current has two children
        else
        {
            //TODO
            // Get the next larger node from current
            Node *nextLargerNode = nextLarger(current);
            // Set next larger node's value into current node
            current->value = nextLargerNode->value;
            // Remove next larger node, which is a leaf
            delete nextLargerNode;
            nextLargerNode = NULL;
        }
        return true;
    }

    // Not the right node to remove
    if (value > current->value){
        return remove(current, current->right, value);
    }else {
        return remove(current, current->left, value);
    }
}

void BST::print(Node *root)
{
    if (!root)
        return;
    print(root->left);
    cout<<root->value<<' ';
    print(root->right);
}

void BST::printTree()
{
    if(!root)
        return;
    print(root);
    cout << endl;
}

Node* BST::findMax()
{
    Node *max = root;

    while (max->right)
    {
        max = max->right;
    }

    return max;
}

Node* BST::findMin()
{
    Node *min = root;

    while (min->left)
    {
        min = min->left;
    }

    return min;
}

bool BST::remove(int value)
{
    return remove(NULL, root, value);
}

/*
int main() {
    // Start tree
    BST *bst = new BST();

    // Add nodes
    bst->insert(10);
    bst->insert(20);
    bst->insert(25);
    bst->insert(8);
    bst->insert(9);
    bst->insert(2);
    bst->insert(4);
    bst->insert(1);
    bst->insert(3);

    // Print sorted tree
    bst->printTree();
    cout << endl;

    // Find max
    Node *max = bst->findMax();
    cout << "Max node: " << max->value << endl;

    // Find min
    Node *min = bst->findMin();
    cout << "Min node: " << min->value << endl;

    cout << endl;


    cout << "Removing: 9" << endl;
    // Remove leaf
    bst->remove(9);
    bst->printTree();

    cout << "Removing: 4" << endl;
    // Remove node with 1 child
    bst->remove(4);
    bst->printTree();

    cout << "Removing: 8" << endl;
    // Remove node with 2 children
    bst->remove(8);
    bst->printTree();
}
*/
