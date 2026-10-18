#include <bits/stdc++.h>
using namespace std;

class Node {
public: 
    int key;
    int degree;
    Node* child;
    Node* sibling;
    Node* parent;

    Node(int val) : key(val), degree(0), child(nullptr), sibling(nullptr), parent(nullptr) {}
};

class BinomialHeap {
    int size;
    Node* root;
    int id;

public:
    BinomialHeap(int val) : root(nullptr), size(0), id(val) {}

    int getSize() const { return size; }
    Node* getRoot() const { return root; }
    int getId() const { return id; }

    void link(Node* parentNode, Node* childNode) {
        childNode->parent = parentNode;
        childNode->sibling = parentNode->child;
        parentNode->child = childNode;
        parentNode->degree++;
    }

    Node* merge(Node* h1, Node* h2) {
        if (!h1) return h2;
        if (!h2) return h1;

        Node* head = nullptr;
        Node** tail = &head;

        while (h1 && h2) {
            if (h1->degree <= h2->degree) {
                *tail = h1;
                h1 = h1->sibling;
            } else {
                *tail = h2;
                h2 = h2->sibling;
            }
            tail = &((*tail)->sibling);
        }
        *tail = h1 ? h1 : h2;
        return head;
    }

    void Union(BinomialHeap& other) {
        size += other.size;
        root = merge(this->root, other.root);
        other.root = nullptr;
        other.size = 0;

        if (!root) return;

        Node* prev = nullptr;
        Node* x = root;
        Node* next = x->sibling;

        while (next != nullptr) {
            if ((x->degree != next->degree) ||
                (next->sibling != nullptr && next->sibling->degree == x->degree)) {
                prev = x;
                x = next;
            } else {
                if (x->key <= next->key) {
                    x->sibling = next->sibling;
                    link(x, next);
                } else {
                    if (prev == nullptr) root = next;
                    else prev->sibling = next;
                    link(next, x);
                    x = next;
                }
            }
            next = x->sibling;
        }
    }

    void UnionVisualization(BinomialHeap& other, ostream& out) {
        out<<"\n-----------------------------------------------------\n";
        out<<"              Union Visualization             \n";
        out<<"--------------------------------------------------------\n";
        out<<"--- Heap H" << this->id<<" before union ---";
        this->visualizeHeap(out);
        out<< "--- Heap H" << other.id<<" before union ---";
        other.visualizeHeap(out);

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
                    out << "Step " << step++ << ": Linking B" << next->degree 
                        << " (root: " << next->key << ") under B" << x->degree 
                        << " (root: " << x->key << ") -> Forms B" << (x->degree + 1) << "\n";
                    x->sibling = next->sibling;
                    link(x, next);
                } else {
                    out << "Step " << step++ << ": Linking B" << x->degree 
                        << " (root: " << x->key << ") under B" << next->degree 
                        << " (root: " << next->key << ") -> Forms B" << (next->degree + 1) << "\n";
                    if (prev == nullptr) root = next;
                    else prev->sibling = next;
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
        this->visualizeHeap(out);
    }

    Node* insert(int key) {
        BinomialHeap temp(10);
        Node* newNode = new Node(key);
        temp.size = 1;
        temp.root = newNode;
        this->Union(temp);
        return newNode;
    }

    int findMin() const {
        Node* current = root;
        int minKey = INT_MAX;
        while (current) {
            if (current->key < minKey) minKey = current->key;
            current = current->sibling;
        }
        return minKey;
    }

    int extractMin(unordered_map<int, Node*>& nodesMap) {
        if (!root) return -1;

        Node* current = root;
        Node* prev = nullptr;
        Node* currMin = root;
        Node* currMinPrev = nullptr;

        while (current != nullptr) {
            if (current->key < currMin->key) {
                currMin = current;
                currMinPrev = prev;
            }
            prev = current;
            current = current->sibling;
        }

        if (currMinPrev) currMinPrev->sibling = currMin->sibling;
        else root = currMin->sibling;

        int minKey = currMin->key;
        int minDegree = currMin->degree;

        Node* childHead = nullptr;
        Node* childCurr = currMin->child;

        while (childCurr) {
            Node* next = childCurr->sibling;
            childCurr->sibling = childHead;
            childCurr->parent = nullptr;
            childHead = childCurr;
            childCurr = next;
        }

        nodesMap.erase(minKey);
        delete currMin;
        size--;

        if (childHead) {
            BinomialHeap binHeap(10);
            binHeap.root = childHead;
            binHeap.size = (1 << minDegree) - 1;
            this->size -= binHeap.size;
            this->Union(binHeap);
        }
        return minKey;
    }

    void decreaseKey(Node* node, int newValue, unordered_map<int, Node*>& nodesMap) {
        int oldValue = node->key;
        node->key = newValue;

        nodesMap.erase(oldValue);
        nodesMap[newValue] = node;

        Node* curr = node;
        Node* p = curr->parent;
        while (p != nullptr && p->key > curr->key) {
            swap(p->key, curr->key);
            nodesMap[curr->key] = curr;
            nodesMap[p->key] = p;
            curr = p;
            p = p->parent;
        }
    }

    void Remove(int key, unordered_map<int, Node*>& nodesMap) {
        Node* node = nodesMap[key];
        this->decreaseKey(node, -2000000000, nodesMap);
        extractMin(nodesMap);
    }

    void printHeap(ostream& out) const {
        out<<"Printing Binomial Heap H"<<this->id<<"\n";
        out<<"Heap size: "<<size<<"\n";

        if (size == 0) {
            out<<"Heap H"<<this->id<<" is empty.\n";
            return;
        }

        Node* curr = root;
        while (curr != nullptr) {
            out << "Binomial Tree, B" << curr->degree << "\n";
            vector<vector<int>> levels;
            queue<pair<Node*, int>> q;
            q.push({curr, 0});

            while (!q.empty()) {
                auto [node, depth] = q.front();
                q.pop();

                if (depth >= levels.size()) {
                    levels.resize(depth + 1);
                }
                levels[depth].push_back(node->key);

                Node* c = node->child;
                while (c) {
                    q.push({c, depth + 1});
                    c = c->sibling;
                }
            }

            for (size_t i = 0; i < levels.size(); i++) {
                sort(levels[i].begin(), levels[i].end());
                out << "Level " << i << ":";
                for (size_t j = 0; j < levels[i].size(); j++) {
                    out << " " << levels[i][j];
                }
                out<< "\n";
            }
            curr = curr->sibling;
        }
    }

    void printTreeVisualizer(Node* curr, string indent, bool isLast, ostream& out) const {
        if (!curr) return;

        out << indent;
        if (isLast) {
            out<<"`-- ";
            indent+= "    ";
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
            printTreeVisualizer(children[i], indent, i == children.size() - 1, out);
        }
    }

    void visualizeHeap(ostream& out){
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
            printTreeVisualizer(curr, " ", true, out);
            out<<"\n";
            curr = curr->sibling;
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
            infile>>h>>x;
            Node* node = heaps[h]->insert(x);
            nodeMap[x] = node;
        } 
        else if (cmd == "F") {
            int h;
            infile >> h;
            int minVal = heaps[h]->findMin();
            cout<<"Find Min returned: "<<minVal<<"\n";
            outfile<<"Find Min returned: "<<minVal<<"\n";
        } 
        else if (cmd == "E") {
            int h;
            infile>>h;
            int minVal = heaps[h]->extractMin(nodeMap);
            cout<<"Extract Min returned: "<<minVal<<"\n";
            outfile<<"Extract Min returned: "<<minVal<<"\n";
        } 
        else if (cmd == "D") {
            int h, x, y;
            infile>>h>>x>>y;
            if (nodeMap.count(x)) {
                heaps[h]->decreaseKey(nodeMap[x],y,nodeMap);
            }
        } 
        else if (cmd == "R") {
            int h, x;
            infile>>h>>x;
            if (nodeMap.count(x)) {
                heaps[h]->Remove(x,nodeMap);
            }
        }
        else if (cmd == "U") {
            int h1, h2;
            infile>>h1>>h2;
            heaps[h1]->UnionVisualization(*heaps[h2], cout);
        } 
        else if (cmd == "P") {
            int h;
            infile>>h;
            heaps[h]->printHeap(cout);
            heaps[h]->printHeap(outfile);
            heaps[h]->visualizeHeap(cout);
        }
    }

    return 0;
}