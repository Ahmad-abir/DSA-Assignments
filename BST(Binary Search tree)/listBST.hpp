#ifndef LISTBST_H
#define LISTBST_H

#include "BST.hpp"
#include <iostream>
#include <stdexcept>

/**
 * Binary Search Tree implementation using linked list structure
 * 
 * @tparam Key - The type of keys stored in the BST
 * @tparam Value - The type of values associated with keys
 */
template <typename Key, typename Value>
class ListBST : public BST<Key, Value> {
private:
    /**
     * Node class for the binary search tree
     */
    class Node {
    public:
        Key key;
        Value value;
        Node* left;
        Node* right;
        
        Node(Key k, Value v) : key(k), value(v), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    size_t node_count;
    
    // TODO: Implement private helper functions as needed
    // Start your private helper functions here
    void clear(Node*node){
        if(node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    void Default_print(Node*node) const{
        if(node== nullptr) {
            return;
        }
        std::cout<<" ("<<node->key<<":"<<node->value;
        if((node->right!=nullptr && node->left == nullptr)) std::cout<<" ()";
        Default_print(node->left);
        Default_print(node->right);
        std::cout<<") ";
    }
    void PreOrder(Node*node) const{
        if(node == nullptr) return;
        std::cout<<"("<<node->key<<":"<<node->value<<") ";
        PreOrder(node->left);
        PreOrder(node->right);

    }
    void inOrder(Node*node) const{
        if(node == nullptr) return;
        inOrder(node->left);
        std::cout<<"("<<node->key<<":"<<node->value<<") ";
        inOrder(node->right);
    }
    void postOrder(Node*node) const{
        if(node == nullptr) return;
        postOrder(node->left);
        postOrder(node->right);
        std::cout<<"("<<node->key<<":"<<node->value<<") ";
    }
    // End your private helper functions here

public:
    /**
     * Constructor
     */
    ListBST() : root(nullptr), node_count(0) {}
    
    /**
     * Destructor
     */
    ~ListBST() {
        // TODO: Implement destructor to free memory
        clear(root);
    }
    
    /**
     * Insert a key-value pair into the BST
     */
    bool insert(Key key, Value value) override {
        // TODO: Implement insertion logic
        if(find(key)) return false;
        
        Node*newNode = new Node(key,value);
        if(node_count==0){
            root = newNode ; 
        }
        else{
            Node*temp1 = root;
            Node*temp2=root;
            while(temp1!= nullptr){
                temp2 = temp1;
                if(temp1->key < key ) temp1 = temp1->right;
                else temp1 = temp1->left;
            }
            if(temp2->key < key) temp2->right = newNode;
            else temp2->left = newNode;
        }
        node_count++;
        return true;

    }
    
    /**
     * Remove a key-value pair from the BST
     */
    bool remove(Key key) override {
        // TODO: Implement removal logic
        Node*temp = root;
        Node*parent = nullptr;
        while(temp != nullptr && temp->key != key ){
            parent = temp;
            if(temp->key < key) temp = temp->right;
            else temp= temp->left;
        }
        if(temp == nullptr){
            return false;
        }
        else if(temp->left == nullptr && temp->right == nullptr){
            if(parent == nullptr){
                delete root;
                root = nullptr;
            }
            else if(temp == parent->right) {
                delete temp;
                parent->right =nullptr;
            }
            else{
                delete temp;
                parent->left = nullptr;
            }
            node_count--;
            return true;
        }
        else if(temp->left != nullptr && temp->right != nullptr){
            Node*successor= temp->right;
            Node*successor_parent = temp;
            while(successor->left !=nullptr){
                successor_parent = successor;
                successor = successor->left;
            }
            temp->key=successor->key;
            temp->value=successor->value;
            if(successor_parent-> left == successor){
                successor_parent->left = successor->right;
            }
            else{
                successor_parent->right = successor->right;
            }
            delete successor;
            node_count--;
            return true;
        }
        else{
            Node*child_temp = (temp->left != nullptr)? temp->left:temp->right;
            if(parent == nullptr){
                root = child_temp;
            }
            else if(parent->right == temp){
                parent->right = child_temp;
            }
            else{
                parent->left = child_temp;
            }
            delete temp;
            node_count--;
            return true;
        }

    }
    
    /**
     * Find if a key exists in the BST
     */
    bool find(Key key) const override {
        // TODO: Implement find logic
        Node*temp = root;
        while(temp !=nullptr &&  temp->key != key ){
            if(temp->key < key) temp = temp->right;
            else temp = temp->left;
        }
        if(temp == nullptr ) return false;
        else return true;
    }

    /**
     * Find a value associated with a given key
     */
    Value get(Key key) const override {
        // TODO: Implement get logic
        Node*temp = root;
        while(temp != nullptr){
            if(temp -> key == key){
                return temp->value;
            }
            else if(temp->key < key){
                temp = temp->right;
            }
            else temp = temp->left;
        }
        throw std::runtime_error("Not found!");
    }

    /**
     * Update the value associated with a given key
     */
    void update(Key key, Value value) override {
        // TODO: Implement update logic
        Node*temp = root;
        while(temp != nullptr){
            if(temp -> key == key){
                temp->value = value;
                return ;
            }
            else if(temp->key < key){
                temp = temp->right;
            }
            else temp = temp->left;
        }
    }

    /**
     * Clear all elements from the BST
     */
    void clear() override {
        // TODO: Implement clear logic
        clear(root);
        root = nullptr;
        node_count = 0;
    }
    
    /**
     * Get the number of keys in the BST
     */
    size_t size() const override {
        // TODO: Implement size logic
        return node_count;
    }
    
    /**
     * Check if the BST is empty
     */
    bool empty() const override {
        // TODO: Implement empty check logic
        return root==nullptr;
    }
    
    /**
     * Find the minimum key in the BST
     */
    Key find_min() const override {
        // TODO: Implement find_min logic
        if(node_count == 0 ){
            throw std::runtime_error("Tree empty");
        }
        else{
            Node*temp = root;
            while(temp->left!=nullptr){
                temp =  temp ->left;
            }
            return temp->key;
        }
    }
    
    /**
     * Find the maximum key in the BST
     */
    Key find_max() const override {
        // TODO: Implement find_max logic
        if(node_count == 0 ){
            throw std::runtime_error("Tree empty");
        }
        else{
            Node*temp = root;
            while(temp -> right != nullptr){
                temp = temp->right;
            }
            return temp->key;

        }
    }

    /**
     * Print the BST using specified traversal method
     */
    void print(char traversal_type = 'D') const override {
        // TODO: Implement print logic
        if(traversal_type == 'D' || traversal_type == 'd'){
            std::cout<<"BST (Default): ";
            Default_print(root);
            std::cout<<std::endl;
        }
        else if(traversal_type == 'P' || traversal_type == 'p' ){
            std::cout<<"BST (Pre-order): ";
            PreOrder(root);
            std::cout<<std::endl;
        }
        else if(traversal_type == 'I' || traversal_type == 'i' ){
            std::cout<<"BST (In-order): ";
            inOrder(root);
            std::cout<<std::endl;
        }
        else if(traversal_type == 'O' || traversal_type == 'o' ){
            std::cout<<"BST (Post-order): ";
            postOrder(root);
            std::cout<<std::endl;
        }
    }
    
};

#endif // LISTBST_H