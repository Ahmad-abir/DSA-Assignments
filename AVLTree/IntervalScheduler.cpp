#include<bits/stdc++.h>
#include<chrono>
using namespace std;
using namespace std::chrono;

constexpr int INF = INT_MIN;

class Node{
    public: 
    int ID;
    int start;
    int end;
    int height;
    int maxEnd;
    Node* left;
    Node* right;

    Node(int id,int s,int e) : 
    ID(id),start(s),end(e),maxEnd(e),height(1),left(nullptr),right(nullptr){}
};


class IntervalAVLTree{
    Node* root=nullptr;
    int nextID = 1;
    unordered_map<int,pair<int,int>> idMap;

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    public: 
    Node* getRoot(){
        return root;
    }
    ~IntervalAVLTree() {
        clear(root);
    }
    int getHeight(Node*node){
        if(node==nullptr) return 0;
        return node->height;
    }
    int getBF(Node*node){
        if(node==nullptr) return 0;
        return getHeight(node->left) - getHeight(node->right);
    }
    int getMaxEnd(Node*node){
        if(node==nullptr) return INF;
        return node->maxEnd;

    }
    void updateNode(Node*node){
        if(node==nullptr) return;
        node->height = 1+max(getHeight(node->left),getHeight(node->right));
        node->maxEnd = max(node->end,max(getMaxEnd(node->left),getMaxEnd(node->right)));
    }
    bool isLess(Node* node1,Node*node2){
        if(node1->start != node2->start) return node1->start < node2->start;
        else return node1->ID < node2->ID;
    }
    Node* RotateLeft(Node*node){
        Node*x = node->right;
        Node*t = x->left;
        x->left = node;
        node->right=t;
        updateNode(node);
        updateNode(x);
        return x;
    }
    Node* RotateRight(Node*node){
        Node*x = node->left;
        Node*t = x->right;
        x->right = node;
        node->left = t;
        updateNode(node);
        updateNode(x);
        return x;
    }
    Node* Balance(Node* node){
        updateNode(node);
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
    bool compareKeys(int s1,int id1,int s2,int id2 ){
        if(s1 != s2) return s1<s2;
        else return id1<id2;
    }
    string CreateTree(Node* node) {
        if (!node) return "";
        if (!node->left && !node->right) return to_string(node->ID);

        string leftStr = CreateTree(node->left);
        string rightStr = CreateTree(node->right);
        return to_string(node->ID) + "(" + leftStr + "," + rightStr + ")";
    }
    string Print(){
        return CreateTree(root);
    }
    bool Add(int s,int e){
        int id = nextID++;
        idMap[id] = {s,e};
        if(root == nullptr){
            root = new Node(id,s,e);
            return true;
        }
        vector<Node*> path;
        Node* y = root;
        while(y!=nullptr){
            path.push_back(y);
            if(compareKeys(s,id,y->start,y->ID)){
                y = y->left;
            }
            else if(compareKeys(y->start,y->ID,s,id)){
                y = y->right;
            }
            else{
                return false;
            }
        }
        Node*parent = path.back();
        Node * newNode = new Node(id,s,e);
        if(compareKeys(parent->start,parent->ID,s,id)) parent->right = newNode;
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
    bool Remove(int id){
        if (idMap.find(id) == idMap.end()) return false;
        if (root == nullptr) return false;
        int s = idMap[id].first;
        int e = idMap[id].second;
        idMap.erase(id);
        Node* current = root;
        Node* parent = nullptr;
        vector<Node*> path;
        while(current != nullptr && current->ID != id){
            path.push_back(current);
            parent = current;
            if(compareKeys(current->start,current->ID,s,id)) current = current->right;
            else current = current->left;
        }

        //if(current == nullptr ) return false;
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
            current->start = successor->start;
            current->end = successor ->end;
            current->ID = successor->ID;
            current->maxEnd = successor->maxEnd;
            idMap[successor->ID] = {successor->start, successor->end};
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
    bool Update(int id,int newS,int newE){
        if (idMap.find(id) == idMap.end()) return false;
        Remove(id);
        idMap[id] = {newS,newE} ;
        if(root == nullptr){
            root = new Node(id,newS,newE);
            return true;
        }
        vector<Node*> path;
        Node* y = root;
        while(y!=nullptr){
            path.push_back(y);
            if(compareKeys(newS,id,y->start,y->ID)){
                y = y->left;
            }
            else if(compareKeys(y->start,y->ID,newS,id)){
                y = y->right;
            }
            else{
                return false;
            }
        }
        Node*parent = path.back();
        Node * newNode = new Node(id,newS,newE);
        if(compareKeys(parent->start,parent->ID,newS,id)) parent->right = newNode;
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
    bool conflict(int s, int e) {
        if (root == nullptr) return false;

        stack<Node*> st;
        st.push(root);

        while (!st.empty()) {
            Node* current = st.top();
            st.pop();
            if (e > current->start && s < current->end) {
                return true;
            }
            if (current->right != nullptr && current->start < e) {
                if (current->right->maxEnd > s) {
                    st.push(current->right);
                }
            }
            if (current->left != nullptr && current->left->maxEnd > s) {
                st.push(current->left);
            }
        }

        return false;
    }
    vector<int> Overlaps(int s,int e){
        Node*current =root;
        vector<int> res;
        stack<Node*> st;
        while(current != nullptr || !st.empty()){
            while(current != nullptr){
                if(current->maxEnd <= s){
                    break;
                }
                st.push(current);
                current = current->left;
                
            }
            if(st.empty()) break;
            current = st.top();
            st.pop();
            if(current->start >= e ){
                current = nullptr;
                continue;  
            } 
            if(current->start < e && s < current->end) res.push_back(current->ID);
            current = current->right; 
        }
        return res;
    }
    vector<int> At(int t){
        Node*current = root;
        vector<int> res;
        stack<Node*> st;
        while(current != nullptr || !st.empty()){
            while(current != nullptr){
                if(current->maxEnd <= t) {
                    break;
                }
                st.push(current);
                current= current->left;
            }
            if (st.empty()) break;

            current = st.top();
            st.pop();

            if (current->start >= t+1) {
                current = nullptr; 
                continue;
            }

            if (current->start <= t && t < current->end) {
                res.push_back(current->ID);
            }

            current = current->right;

        }
        return res;
    }
    Node * Next(int t){
        Node* current = root;
        Node* parent = nullptr;
        while(current != nullptr){
            if(current->start >= t){
                parent = current;
                current = current->left;
            }
            else current = current->right;
        }
        return parent;
    }
    string print(Node* node) {
        if (node == nullptr) return "";
        if (isLeaf(node)) return to_string(node->ID);

        string leftStr = print(node->left);
        string rightStr = print(node->right);
        return to_string(node->ID) + "(" + leftStr + "," + rightStr + ")"; 
    }
    bool isLeaf(Node*node){
        return node->left == nullptr && node->right == nullptr;
    }
};

int main(int argc, char* argv[]){
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    IntervalAVLTree tree;
    int add_count = 0, remove_count = 0, update_count = 0, conflict_count = 0, overlaps_count = 0, at_count = 0, next_count = 0;
    long long add_ns = 0, remove_ns = 0, update_ns = 0, conflict_ns = 0, overlaps_ns = 0, at_ns = 0, next_ns = 0;
    string input;
    while(getline(inFile, input)){
        if (input.empty()) continue;
        stringstream ss(input);
        string cmd;
        ss>>cmd;
        if(cmd == "ADD"){
            int s,e;
            ss>>s>>e;
            auto start_t = steady_clock::now(); //[cite: 1]
            tree.Add(s, e);
            auto end_t = steady_clock::now();
            add_ns += duration_cast<nanoseconds>(end_t-start_t).count();
            add_count++;
            string str = tree.print(tree.getRoot());    
            outFile << str <<"\n";
        }
        else if(cmd == "REMOVE"){
            int id;
            ss>>id;
            auto start_t = steady_clock::now();
            bool t = tree.Remove(id);
            auto end_t = steady_clock::now();
            remove_ns +=duration_cast<nanoseconds>(end_t-start_t).count();
            remove_count++;
            if(t){
                string str = tree.print(tree.getRoot());    
                outFile << str <<"\n";
            }
            else {
                outFile<<"not found\n";
            }
        }
        else if( cmd == "UPDATE"){
            int id,s,e;
            ss>>id>>s>>e;
            auto start_t = steady_clock::now();
            bool t = tree.Update(id,s,e);
            auto end_t = steady_clock::now();
            update_ns +=duration_cast<nanoseconds>(end_t-start_t).count();
            update_count++;
            if(t){
                string str = tree.print(tree.getRoot());    
                outFile << str <<"\n";
            }
            else {
                outFile<<"not found\n";
            }
        }
        else if(cmd == "CONFLICT"){
            int s,e;
            ss>>s>>e;
            auto start_t = steady_clock::now();
            bool t = tree.conflict(s,e);
            auto end_t = steady_clock::now();
            conflict_ns +=duration_cast<nanoseconds>(end_t-start_t).count();
            conflict_count++;
            if(t){
                outFile << "yes" <<"\n";
            }
            else{
                outFile << "no" <<"\n";
            }
        }
        else if(cmd == "OVERLAPS"){
            int s,e;
            ss>>s>>e;
            vector<int> res;
            auto start_t = steady_clock::now();
            res = tree.Overlaps(s,e);
            auto end_t = steady_clock::now();
            overlaps_ns +=duration_cast<nanoseconds>(end_t-start_t).count();
            overlaps_count++;
            if(res.empty()){
                outFile<<"none";
            }
            else{
                for(auto i : res){
                    outFile << i <<" ";
                }
            }
            outFile<<"\n";
        }
        else if(cmd == "AT"){
            int t;
            ss>>t;
            vector<int> res;
            auto start_t = steady_clock::now();
            res = tree.At(t);
            auto end_t = steady_clock::now();
            at_ns +=duration_cast<nanoseconds>(end_t-start_t).count();
            at_count++;
            if(res.empty()){
                outFile<<"none";
            }
            else{
                for(auto i : res){
                    outFile << i <<" ";
                }
            }
            outFile<<"\n";
        }
        else if(cmd == "NEXT"){
            int t;
            ss>>t;
            Node* temp;
            auto start_t = steady_clock::now();
            temp = tree.Next(t);
            auto end_t = steady_clock::now();
            next_ns +=duration_cast<nanoseconds>(end_t-start_t).count();
            next_count++;
            if(temp != nullptr){
                outFile<<temp->ID<<" "<<temp->start<<" "<<temp->end<<"\n";
            }
            else outFile<<"none"<<"\n";
        }
        else continue;

    }
    inFile.close();
    outFile.close();

    cout << "operation, count, total_ns, average_ns\n";
    cout << "add, " << add_count << ", " << add_ns << ", " << (add_count ? to_string(add_ns / add_count) : "N/A") << "\n";
    cout << "remove, " << remove_count << ", " << remove_ns << ", " << (remove_count ? to_string(remove_ns / remove_count) : "N/A") << "\n";
    cout << "update, " << update_count << ", " << update_ns << ", " << (update_count ? to_string(update_ns / update_count) : "N/A") << "\n";
    cout << "conflict, " << conflict_count << ", " << conflict_ns << ", " << (conflict_count ? to_string(conflict_ns / conflict_count) : "N/A") << "\n";
    cout << "overlaps, " << overlaps_count << ", " << overlaps_ns << ", " << (overlaps_count ? to_string(overlaps_ns / overlaps_count) : "N/A") << "\n";
    cout << "at, " << at_count << ", " << at_ns << ", " << (at_count ? to_string(at_ns / at_count) : "N/A") << "\n";
    cout << "next, " << next_count << ", " << next_ns << ", " << (next_count ? to_string(next_ns / next_count) : "N/A") << "\n";
}