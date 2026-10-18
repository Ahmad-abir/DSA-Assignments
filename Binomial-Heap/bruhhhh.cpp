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
    void UnionVisualizer(BinomialHeap& other, ostream& out) {
        out<<"\n-----------------------------------------------------\n";
        out<<"              Union Visualization             \n";
        out<<"--------------------------------------------------------\n";
        out<<"--- Heap H" << this->id<<" before union ---";
        this->DrawHeap(out);
        out<< "--- Heap H" << other.id<<" before union ---";
        other.DrawHeap(out);

        out << "--- Step By Step Linking ---\n";
        size += other.size;
        root = merge(this->root, other.root);
        other.root = nullptr;
        other.size = 0;

        if (!root) {
            out << "Both heaps empty. Resulting heap is empty.\n";
            return;
        }

        Node* prev = nullptr;
        Node* x = root;
        Node* next = x->sibling;
        int step = 1;

        while (next != nullptr) {
            if ((x->degree != next->degree) ||
                (next->sibling != nullptr && next->sibling->degree == x->degree)) {
                prev = x;
                x = next;
            } else {
                if (x->key <= next->key) {
                    out<< "Step " <<step++ << ": Connecting B" << next->degree 
                        <<" (root: " <<next->key << ") with B" << x->degree 
                        << " (root: " << x->key << ") ->  B" << (x->degree + 1) << "\n";
                    x->sibling = next->sibling;
                    link(x, next);
                } else {
                    out<< "Step " << step++ << ": Connecting B" << x->degree 
                        <<" (root: " << x->key <<") with B" << next->degree 
                        << " (root: "<< next->key << ") -> B" << (next->degree + 1) << "\n";
                    if (prev == nullptr) root =next;
                    else prev->sibling=next;
                    link(next, x);
                    x = next;
                }
            }
            next = x->sibling;
        }

        if (step == 1) {
            out << "No duplicate degrees found. Direct root list merge performed.\n";
        }

        out<<"\n--- Final Heap H"<<this->id<<" after union ---";
        this->DrawHeap(out);
    }    
    void DrawHeap(ostream& out){
        out<<"\n-----------------------------------------------------\n";
        out<<"Binomial Heap H"<<this->id<<"\n";
        out<<"Total Heap Size: "<<size<<" nodes\n";
        out<<"-------------------------------------------------------\n";

        if (size == 0) {
            out<<"Heap H"<<this->id<<" is currently empty.\n";
            return;
        }
        Node* curr = root;
        while (curr) {
            out<<"Tree B"<<curr->degree<<"(Root: "<<curr->key<<"):\n";
            DrawTree(curr, " ", true, out);
            out<<"\n";
            curr = curr->sibling;
        }
    }
    void DrawTree(Node* curr, string indent, bool isLast, ostream& out) const {
        if (!curr) return;
        out << indent;
        if (isLast) {
            out<<"`-- ";
            indent+="    ";
        } else {
            out<< "|-- ";
            indent += "|   ";
        }
        int subNodes = (1 << curr->degree);
        out<<"["<<curr->key<<"] \n";
        vector<Node*> children;
        Node* c = curr->child;
        while (c) {
            children.push_back(c);
            c = c->sibling;
        }
        reverse(children.begin(), children.end());
        for (size_t i = 0; i < children.size(); i++) {
            DrawTree(children[i], indent, i == children.size() - 1, out);
        }
    }
    void decreaseKeyVisualizer(Node* node, int newValue, unordered_map<int, Node*>& nodesmap, ostream& out) {
        out << "\n          Decrease Key Operation Visualization       \n";
        out << "--------------------------------------------------------\n";
        out << "Decreasing key " << node->key << " to " << newValue << " in Heap H" << this->id << "\n";

        int oldValue = node->key;
        node->key = newValue;
        nodesmap.erase(oldValue);
        nodesmap[newValue] = node;

        Node* curr = node;
        Node* p = curr->parent;
        int swapCount = 0;

        out << "Starting Bubble-Up Path Trace:\n";

        while (p != nullptr && p->key > curr->key) {
            swapCount++;
            out << " Swap #" << swapCount << ": Parent node [" << p->key 
                << "] > Child node [" << curr->key << "]. Swapping values...\n";

            swap(p->key, curr->key);
            nodesmap[curr->key] = curr;
            nodesmap[p->key] = p;

            curr = p;
            p = p->parent;
        }

        if (swapCount == 0) {
            out << " Min-heap property maintained. No parent-child swaps required.\n";
        } else {
            out << " Heap property restored after " << swapCount << " swap(s).\n";
        }

        out << "\n--- Updated Heap H" << this->id << " Structure ---";
        this->DrawHeap(out);
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
                heaps[h]->decreaseKeyVisualizer(nodeMap[x], y, nodeMap, cout);
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
            heaps[h1]->UnionVisualizer(*heaps[h2], cout);
        } 
        else if (cmd == "P") {
            int h;
            infile >> h;
            heaps[h]->printHeap(cout);
            heaps[h]->printHeap(outfile);
            heaps[h]->DrawHeap(cout);
        }
    }

    return 0;
}
