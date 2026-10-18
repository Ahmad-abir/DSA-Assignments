#include<bits/stdc++.h>
using namespace std;

class Node{
    public: 
    int key;
    int degree;
    Node* child;
    Node*sibling;
    Node*parent;

    Node(int val) : key(val),degree(0),child(nullptr),sibling(nullptr),parent(nullptr){}
};

class BinomialHeap{
    int size;
    Node*root;
    int id;

    public:
    BinomialHeap(int val) : root(nullptr),size(0),id(val){}
    int getSize(){
        return size;
    }
    Node* getRoot(){
        return root;
    }
    void Union(BinomialHeap& other){
        size+= other.size;
        root  = merge(this->root,other.root);
        other.root = nullptr;
        other.size =0;

        if(!root) return;

        Node*prev=nullptr;
        Node*x = root;
        Node*next = x->sibling;

        while(next != nullptr){
            if((x->degree != next->degree) ||
                (next->sibling != nullptr && next->sibling->degree == x->degree)){
                    prev = x;
                    x = next;
                }
            else{
                if(x->key <= next->key){
                    x->sibling = next->sibling;
                    link(x,next);
                }
                else{
                    if(prev == nullptr) root=next;
                    else prev->sibling=next;
                    link(next,x);
                    x = next;
                }
            }
            next=x->sibling;
        }

    }
    void link(Node*parent,Node*child){
        child->parent = parent;
        child->sibling = parent->child;
        parent->child = child;
        parent->degree++;
    }
    Node* merge(Node*h1,Node*h2){
        if(!h1) return h2;
        if(!h2) return h1;

        Node*head = nullptr;
        Node** tail = &head;

        while(h1 && h2){
            if(h1->degree <= h2->degree){
                *tail = h1;
                h1 = h1->sibling;
            }
            else{
                *tail = h2;
                h2 = h2->sibling;
            }
            tail = &((*tail)->sibling);
        }
        *tail = h1 ? h1: h2;
        return head;
    }
    Node* insert(int key){
        BinomialHeap temp(10);
        Node* newNode = new Node(key);
        temp.size=1;
        temp.root = newNode;
        this->Union(temp);
        return newNode;
    }
    int findMin(){
        Node*current = root;
        int min = INT_MAX;
        while(current){
            if(current->key < min) min = current->key;
            current = current->sibling;
        }
        return min;
    }
    int extractMin(unordered_map<int,Node*> & nodesMap){
        if(!root) return -1;

        Node* current = root;
        Node*prev = nullptr;
        Node* currMin = root;
        Node*CurrminPrev = nullptr;
        while(current != nullptr){
            if(currMin->key > current->key){
                currMin = current;
                CurrminPrev = prev;
            }
            prev = current;
            current = current->sibling;
        }

        if(CurrminPrev) CurrminPrev->sibling = currMin->sibling;
        else root = currMin->sibling;

        int minKey = currMin->key;
        int minDegree = currMin->degree;
        Node* childHead = nullptr;
        Node* childCurr = currMin->child;
        int childCount=0;

        while(childCurr){
            Node*next = childCurr->sibling;
            childCurr->sibling = childHead;
            childCurr->parent = nullptr;
            childHead = childCurr;
            childCurr = next;
            childCount++;
        }

        nodesMap.erase(minKey);
        delete currMin;
        size--;

        if(childHead){
            BinomialHeap binHeap(10);
            binHeap.root = childHead;
            binHeap.size = pow(2,minDegree) - 1;
            this->size-=binHeap.size;
            this->Union(binHeap);
        }
        return minKey;
    }
    void decreaseKey(Node* node,int newValue,unordered_map<int,Node*> &nodesmap){
        int oldValue = node->key;
        node->key = newValue;

        nodesmap.erase(oldValue);
        nodesmap[newValue] = node; 
        Node*curr = node;
        Node*p = curr->parent;
        while(p!= nullptr && p->key >curr->key){
            swap(p->key,curr->key);
            nodesmap[curr->key] = curr;
            nodesmap[p->key]=p;
            curr = p;
            p = p->parent;
        }

    }
    void Remove(int key,unordered_map<int,Node*> &nodesmap){
        Node*node = nodesmap[key];
        this->decreaseKey(node,INT_MIN,nodesmap);
        extractMin(nodesmap);
    }
    void printHeap(ostream& out){
        out<<"Printing Binomial Heap H"<<this->id<<"\n";
        out<<"Heap size: "<<size<<"\n";

        if(size ==0 ){
            out<<"Heap H"<<this->id<<" is empty.\n";
            return;
        }
        Node* curr = root;
        while (curr != nullptr) {
            out << "Binomial Tree, B" << curr->degree << "\n";
            vector<vector<int>> levels;
            queue<pair<Node*,int>> q;
            q.push({curr,0});
            while(!q.empty()){
                auto [node,depth] = q.front();
                q.pop();
                levels.resize(depth+1);
                levels[depth].push_back(node->key);
                Node*c = node->child;
                while(c){
                    q.push({c,depth+1});
                    c = c->sibling;
                }
            }
            for(int i = 0; i<levels.size();i++){
                sort(levels[i].begin(),levels[i].end());
                out<<"Level "<<i<<": ";
                for(auto t : levels[i]){
                    out<<t<<" ";
                }
                out<<endl;
            }
            curr=curr->sibling;
        }
    }
};

int main() {
    ifstream infile("input.txt");
    ofstream outfile("output.txt");

    if (!infile.is_open()) {
        return 1;
    }

    BinomialHeap H1(1), H2(2);
    BinomialHeap* heaps[] = { nullptr, &H1, &H2 };
    unordered_map<int, Node*> nodeMap;

    string cmd;
    while (infile >> cmd) {
        if (cmd == "I") {
            int h, x;
            infile >> h >> x;
            Node* node = heaps[h]->insert(x);
            nodeMap[x] = node;
        } 
        else if (cmd == "F") {
            int h;
            infile >> h;
            int min_val = heaps[h]->findMin();
            cout << "Find Min returned: " << min_val << "\n";
            outfile << "Find Min returned: " << min_val << "\n";
        } 
        else if (cmd == "E") {
            int h;
            infile >> h;
            int minVal = heaps[h]->extractMin(nodeMap);
            cout << "Extract Min returned: " << minVal << "\n";
            outfile << "Extract Min returned: " << minVal << "\n";
        } 
        else if (cmd == "D") {
            int h, x, y;
            infile >> h >> x >> y;
            if (nodeMap.count(x)) {
                heaps[h]->decreaseKey(nodeMap[x], y, nodeMap);
            }
        } 
        else if (cmd == "R") {
            int h, x;
            infile >> h >> x;
            if (nodeMap.count(x)) {
                heaps[h]->Remove(x, nodeMap);
            }
        }
        else if (cmd == "U") {
            int h1, h2;
            infile >> h1 >> h2;
            heaps[h1]->Union(*heaps[h2]);
        } 
        else if (cmd == "P") {
            int h;
            infile >> h;
            heaps[h]->printHeap(cout);
            heaps[h]->printHeap(outfile);
        }
    }

    return 0;
}
