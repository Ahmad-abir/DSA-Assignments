#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

class Node{
    public:
    int key;
    int height;
    Node*left;
    Node*right;
    
    Node(int k):key(k),left(nullptr),right(nullptr),height(1){}
};

class AVLTree{
    Node*root;

    public: 
    AVLTree():root(nullptr){}
    int getHeight(Node*node){
        if(node==nullptr) return 0;
        return node->height;
    }
    int getBF(Node*node){
        if(node==nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }
    void updateHeight(Node*node){
        if(node==nullptr) return;
        node->height = 1+max(getHeight(node->left),getHeight(node->right));
    }
    Node* RotateLeft(Node*node){
        Node*x = node->right;
        Node*t = x->left;
        x->left = node;
        node->right=t;
        updateHeight(node);
        updateHeight(x);
        return x;
    }
    Node* RotateRight(Node*node){
        Node*x = node->left;
        Node*t = x->right;
        x->right = node;
        node->left = t;
        updateHeight(node);
        updateHeight(x);
        return x;
    }
    Node* Balance(Node* node){
        updateHeight(node);
        int BalanceFactor = getBF(node);
        if(BalanceFactor > 1){
            if(getBF(node->left) < 0){
                node->left = RotateLeft(node->left);
            }
            return RotateRight(node);
        }
        if(BalanceFactor < -1){
            if(getBF(node->right)>0){
                node->right = RotateRight(node->right);
            }
            return RotateLeft(node);
        }
        return node;
    }
    bool insert(int key){
        if(root == nullptr){
            root = new Node(key);
            return true;
        }
        vector<Node*> path;
        Node* y = root;
        while(y!=nullptr){
            path.push_back(y);
            if(key > y->key){
                y = y->right;
            }
            else if(key < y->key){
                y = y->left;
            }
            else{
                return false;
            }
        }
        Node*parent = path.back();
        Node * newNode = new Node(key);
        if(key > parent->key) parent->right = newNode;
        else parent->left = newNode;

        for(int i = path.size()-1 ; i>=0 ;i--){
            Node*current = path[i];
            Node* rebalanced = Balance(current);
            if(i==0){
                root = rebalanced;
            }
            else{
                Node*parent = path[i-1];
                if(parent->left == current)  parent->left = rebalanced;
                else parent->right = rebalanced;
            }
        }

        return true;
    }
    bool find(int key){
        Node*temp=root;
        while(temp!=nullptr){
            if(key > temp->key) temp = temp->right;
            else if(key < temp->key) temp = temp->left;
            else return true;
        }
        return false;
    }
    vector<int> traverse(){
        vector<int> nodes;
        Inorder(root,nodes);
        return nodes;
    }
    void Inorder(Node*node,vector<int> & nodes){
        if(node==nullptr) return;
        Inorder(node->left,nodes);
        nodes.push_back(node->key);
        Inorder(node->right,nodes);
    }
    bool erase(int key){
        if (root == nullptr) return false;
        Node* current = root;
        Node* parent = nullptr;
        vector<Node*> path;
        while(current != nullptr && current->key != key){
            path.push_back(current);
            parent = current;
            if(key>current->key) current = current->right;
            else current = current->left;
        }

        if(current == nullptr ) return false;
        //Deleting
        if(current->left != nullptr && current->right != nullptr){
            path.push_back(current);
            Node*successor=current->right;
            Node*successorParent = current;
            while(successor->left != nullptr){
                path.push_back(successor);
                successorParent = successor;
                successor = successor->left;
            }
            Node*successorRight=successor->right;
            current->key = successor->key;
            if(successor==successorParent->left) successorParent->left = successorRight;
            else successorParent->right = successorRight;
            delete successor;
        }
        else if(current->left == nullptr && current->right == nullptr){
            if(parent == nullptr){
                root = nullptr;
                delete current;
            }
            else if(current == parent->left){
                parent->left = nullptr;
                delete current;
            }
            else{
                parent->right = nullptr;
                delete current;
            }
        }
        else{
            Node*child = (current->right != nullptr) ? current->right : current->left;
            if(parent == nullptr) root = child;
            else if(current == parent->left) parent->left = child;
            else parent->right = child;
            delete current ;
        }
        for (int i = path.size() - 1; i >= 0; i--) {
            Node* current_Node = path[i];
            Node* rebalanced = Balance(current_Node);

            if (i == 0) {
                root = rebalanced;
            } else {
                Node* p = path[i - 1];
                if (p->left == current_Node) p->left = rebalanced;
                else p->right = rebalanced;
            }
        }
        return true;
    }
    string Serialize(Node* node) {
        if (!node) return "";
        if (!node->left && !node->right) return to_string(node->key);

        string leftStr = Serialize(node->left);
        string rightStr = Serialize(node->right);
        return to_string(node->key) + "(" + leftStr + "," + rightStr + ")";
    }
    string Print(){
        return Serialize(root);
    }


};

int main(int argc, char* argv[]) {

    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);

    AVLTree tree;

    long long insert_count = 0, delete_count = 0, find_count = 0, traverse_count = 0;
    long long insert_ns = 0, delete_ns = 0, find_ns = 0, traverse_ns = 0;

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        char cmd;
        ss >> cmd;

        if (cmd == 'I') {
            int key;
            ss >> key;
            auto start = steady_clock::now();
            bool status = tree.insert(key);
            auto end = steady_clock::now();
            insert_ns += duration_cast<nanoseconds>(end - start).count();
            insert_count++;

            if (status) outFile << tree.Print() << "\n";
            else outFile << "duplicate\n";
        } else if (cmd == 'D') {
            int key;
            ss >> key;
            auto start = steady_clock::now();
            bool status = tree.erase(key);
            auto end = steady_clock::now();
            delete_ns += duration_cast<nanoseconds>(end - start).count();
            delete_count++;

            if (status) outFile << tree.Print() << "\n";
            else outFile << "not found\n";
        } else if (cmd == 'F') {
            int key;
            ss >> key;
            auto start = steady_clock::now();
            bool found = tree.find(key);
            auto end = steady_clock::now();
            find_ns += duration_cast<nanoseconds>(end - start).count();
            find_count++;

            if (found) outFile << "found\n";
            else outFile << "not found\n";
        } else if (cmd == 'T') {
            auto start = steady_clock::now();
            vector<int> res = tree.traverse();
            auto end = steady_clock::now();
            traverse_ns += duration_cast<nanoseconds>(end - start).count();
            traverse_count++;

            for (size_t i = 0; i < res.size(); ++i) {
                outFile << res[i] << (i + 1 == res.size() ? "" : " ");
            }
            outFile << "\n";
        }
    }

    cout << "operation, count, total_ns, average_ns\n";
    cout << "insert, " << insert_count << ", " << insert_ns << ", " << (insert_count ? to_string(insert_ns / insert_count) : "N/A") << "\n";
    cout << "delete, " << delete_count << ", " << delete_ns << ", " << (delete_count ? to_string(delete_ns / delete_count) : "N/A") << "\n";
    cout << "find, " << find_count << ", " << find_ns << ", " << (find_count ? to_string(find_ns / find_count) : "N/A") << "\n";
    cout << "traverse, " << traverse_count << ", " << traverse_ns << ", " << (traverse_count ? to_string(traverse_ns / traverse_count) : "N/A") << "\n";

    return 0;
}












