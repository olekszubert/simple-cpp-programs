#include <iostream>

/*
Create a tree:
            4
    2               6
1       3       5       7


Print the tree:
1, 2, 3, 4, 5, 6, 7
*/

struct Node
{
    int value;
    Node* left;
    Node* right;
};

class BinaryTree 
{
public:
    BinaryTree();
    ~BinaryTree();

    void CreateExample();

    void Insert(int val);

    void DeleteNode(int val) { DeleteNode(m_Root, val); }
    void DeleteTree() { DeleteTree(m_Root); }
    Node* Search(int val) { return Search(m_Root, val); }

    void Print() const { 
        std::cout << "Binary Tree: "; 
        Print(m_Root);
        std::cout << std::endl;
    }

    void PrintReversed() const {
        std::cout << "Binary Tree Reversed: ";
        //Print(m_Root);
        std::cout << std::endl;
    }

private:
    void Insert(Node* node, int val);
    Node* DeleteNode(Node* node, int val);
    void DeleteTree(Node* node);
    void Print(Node* node) const;
    Node* Search(Node* node, int val);

    Node* NewNode(int val);
    Node* LeftMostNode(Node* node);

    Node* m_Root;
};

BinaryTree::BinaryTree()
{
    m_Root = nullptr;
}

BinaryTree::~BinaryTree()
{
    DeleteTree();
}

void BinaryTree::CreateExample()
{
    this->Insert(4); //first value according to the task
    this->Insert(6);
    this->Insert(5);
    this->Insert(1);
    this->Insert(3);
    this->Insert(2);
    this->Insert(7);

    //Two invalid cases
    this->Insert(4);
    this->Insert(2);
}

Node* BinaryTree::NewNode(int val)
{
    Node* node = new Node;
    if (!node)
    {
        std::cout << "Crap...\n";
        return nullptr;
    }

    node->value = val;
    node->left = node->right = nullptr;
    return node;
}

void BinaryTree::Insert(int val)
{
    if (m_Root != nullptr)
        Insert(m_Root, val);
    else
        m_Root = NewNode(val);
}

void BinaryTree::Insert(Node* node, int val)
{
    //Check if value is less or greater
    if (val < node->value)
    {
        //Continue if you can
        if (node->left != nullptr)
            Insert(node->left, val);
        //If next node nullptr, create new and insert value, return
        else
            node->left = NewNode(val);
    }
    else if (val > node->value)
    {
        if (node->right != nullptr)
            Insert(node->right, val);
        else
            node->right = NewNode(val);
    }
    else
        std::cout << "Error: Invalid value: Binary tree must have unique values: Insert skipped\n";
}

void BinaryTree::Print(Node* node) const
{
    //Return if empty node reached
    if (node != NULL)
    {
        //Go left recursively
        Print(node->left);

        //Print value
        std::cout << node->value << ", ";

        //Go right recursively
        Print(node->right);
    }
}

//Find the left-most value of a binary tree (minimum value)
Node* BinaryTree::LeftMostNode(Node* node) {
    Node* currNode = node;

    //Find the left-most node
    while (currNode && currNode->left != NULL)
        currNode = currNode->left;

    return currNode;
}

Node* BinaryTree::DeleteNode(Node* node, int val)
{
    //If the node is empty, return
    if (node == nullptr) return node;

    //Find the node to be deleted
    if (val < node->value)
        node->left = DeleteNode(node->left, val);
    else if (val > node->value)
        node->right = DeleteNode(node->right, val);

    //Node found
    else {
        //If the node has 1 or 0 subnodes
        if (node->left == NULL) {
            Node* tmp = node->right;
            free(node);
            return tmp;
        }
        else if (node->right == NULL) {
            Node* tmp = node->left;
            free(node);
            return tmp;
        }

        //If the node has 2 subnodes
        //Find the node closest in value to current node 
        //(could be either right-most of node->left or left-most of node->right
        Node* tmp = LeftMostNode(node->right);

        //Place the found value in the position of the node to be deleted
        node->value = tmp->value;

        //Delete the value we just used to replace the deleted value
        //If it is the only value in that node, returns NULL
        node->right = DeleteNode(node->right, tmp->value);
    }
    return node;
}

//Delete tree, free memory
void BinaryTree::DeleteTree(Node* node)
{
    if (node != NULL)
    {
        DeleteTree(node->left);
        DeleteTree(node->right);

        delete node;
        node = nullptr;
    }
}

Node* BinaryTree::Search(Node* node, int val)
{
    if (node != nullptr)
    {
        if (node->value == val)
            return node;
        else if (node->value < val)
            Search(node->right, val);
        else if (node->value > val)
            Search(node->left, val);
    }
    else return nullptr;
}


int main(void)
{
    BinaryTree* myTree = new BinaryTree;

    myTree->CreateExample();

    //---Print---
    myTree->Print();
    std::cout << std::endl;

    //---Search---
    Node* found = myTree->Search(7);
    (found) ?
        std::cout << "Found: " << found->value << std::endl
        : std::cout << "Not Found" << std::endl;

    found = myTree->Search(9);
    (found) ?
        std::cout << "Found: " << found->value << std::endl
        : std::cout << "Not Found" << std::endl;

    std::cout << std::endl;

    //---Remove a node---
    myTree->DeleteNode(4);
    myTree->Print();

    myTree->DeleteNode(2);
    myTree->Print();
    
    //---Delete---
    delete myTree;
    myTree = nullptr;

    return 0;
}
