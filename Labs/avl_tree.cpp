#include <iostream>
using namespace std;

// Node class to represent each node in the AVL tree
class Node {
public:
    int key;          // Value of the node
    Node* left;       // Pointer to the left child
    Node* right;      // Pointer to the right child
    int height;       // Height of the node for balancing

    // Constructor for Node
    Node(int value) {
        key = value;
        left = right = nullptr;
        height = 1;   // New nodes are initially added at height 1
    }
};

// AVLTree class for performing AVL tree operations
class AVLTree {
public:
    Node* root;

    AVLTree() {
        root = nullptr;
    }

    // Utility function to get the height of a node
    int height(Node* node) {
        return (node == nullptr) ? 0 : node->height;
    }

    // Utility function to get the balance factor of a node
    int getBalance(Node* node) {
        return (node == nullptr) ? 0 : height(node->left) - height(node->right);
    }

    // Right rotation to balance the tree
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        // Return new root
        return x;
    }

    // Left rotation to balance the tree
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        // Return new root
        return y;
    }

    // Insert a node into the AVL tree
    Node* insert(Node* node, int key) {
        // 1. Perform the normal BST insertion
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else // Duplicates are not allowed in AVL tree
            return node;

        // 2. Update the height of the ancestor node
        node->height = 1 + max(height(node->left), height(node->right));

        // 3. Get the balance factor to check if this node became unbalanced
        int balance = getBalance(node);

        // If node becomes unbalanced, perform rotations
        // Left Left Case
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node; // Return the unchanged node
    }

    // Find the node with minimum value (used in deletion)
    Node* minValueNode(Node* node) {
        Node* current = node;
        // Loop to find the leftmost leaf
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Delete a node from the AVL tree
    Node* deleteNode(Node* root, int key) {
        // 1. Perform standard BST delete
        if (root == nullptr)
            return root;

        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            // Node with one or no child
            if (root->left == nullptr || root->right == nullptr) {
                Node* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp; // Copy the contents of the non-empty child

                delete temp;
            } else {
                // Node with two children: Get the inorder successor
                Node* temp = minValueNode(root->right);

                // Copy the inorder successor's data to this node
                root->key = temp->key;

                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->key);
            }
        }

        // If the tree had only one node, return
        if (root == nullptr)
            return root;

        // 2. Update height of the current node
        root->height = 1 + max(height(root->left), height(root->right));

        // 3. Get the balance factor of this node
        int balance = getBalance(root);

        // If the node becomes unbalanced, perform rotations
        // Left Left Case
        if (balance > 1 && getBalance(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalance(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Search for a key in the AVL tree
    Node* search(Node* root, int key) {
        // Base cases: root is null or key is present at root
        if (root == nullptr || root->key == key)
            return root;

        // Key is greater than root's key
        if (root->key < key)
            return search(root->right, key);

        // Key is smaller than root's key
        return search(root->left, key);
    }

    // In-order traversal to display the tree
    void inOrder(Node* root) {
        if (root != nullptr) {
            inOrder(root->left);
            cout << root->key << " ";
            inOrder(root->right);
        }
    }

    // Public methods for easy interface
    void insert(int key) {
        root = insert(root, key);
    }

    void deleteNode(int key) {
        root = deleteNode(root, key);
    }

    void search(int key) {
        Node* result = search(root, key);
        if (result != nullptr)
            cout << "Node " << key << " found in the tree." << endl;
        else
            cout << "Node " << key << " not found in the tree." << endl;
    }

    void displayInOrder() {
        inOrder(root);
        cout << endl;
    }
};

int main() {
    AVLTree tree;

    // Insert nodes
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    cout << "In-order traversal of the AVL tree: ";
    tree.displayInOrder();

    // Search for a node
    tree.search(30);
    tree.search(60);

    // Delete nodes
    tree.deleteNode(40);
    cout << "After deletion of 40, In-order traversal: ";
    tree.displayInOrder();

    return 0;
}