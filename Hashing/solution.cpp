#include<bits/stdc++.h>
using namespace std;

int INITIAL_SIZE =13;
double LOAD_FACTOR_HIGH = 0.5;
double LOAD_FACTOR_LOW = 0.25;
int C1 = 3;
int C2 = 5;

enum collisionTechnique{
        Chaining,
        Double_Hashing,
        Custom_Probing
};

bool isPrime(int n){
    if(n<2) return false;
    if(n<4) return true;
    if(n%2 == 0) return false;
    for(int i = 3 ; i * i <= n ; i+=2){
        if(n%i == 0) return false;
    }
    return true;
}

int nextPrime(int n){
    int c = n+1;
    while(!isPrime(c)) c++;
    return c;
}

int prevPrime(int n){
    int c = n-1;
    while(c>=2 && !isPrime(c)) c--;
    return c;
}

long long Hash1(string & key){
    long long hash = 0;
    long long p = 31;
    long long p_power = 1;
    for(int i =0 ; i < key.size() ;i++){
        hash = hash + ((key[i] - 'a' + 1) * p_power);
        p_power*=p;
    }
    return hash;
}

long long Hash2(string & key){
    long long hash = 0 ; 
    long long p = 53;
    long long m = 1e9 + 7;
    for(auto c : key){
        hash = (hash * p + static_cast<long long>(c)) % m;
    }
    return hash;

}

class HashElements{
    public:
    string key;
    int value;
    bool occupied;
    bool isDeleted;

    HashElements():key("") , value(0),occupied(false),isDeleted(false){}
    HashElements(string s,int v):key(s),value(v),occupied(true),isDeleted(false){}
};


class HashTable{
    int currentCapacity;
    int currentSize;
    int insertionsSinceResize;
    int deletionsSinceResize;
    int elementsAtLastResize;
    vector<list<HashElements>> chainTable;
    vector<HashElements> normalTable;
    collisionTechnique Collision;
    int HashType;

    long long primaryHash(string& key){
        if(HashType == 1){
            return Hash1(key);
        }
        else{
            return Hash2(key);
        }
    }

    int getProbeIndex(string key,int i){
        long long hash = primaryHash(key);
        long long auxhash = auxHash(key);
        if(Collision == Double_Hashing){
            return (hash + (i * auxhash)) % currentCapacity;
        }
        else{
            return (hash+C1 * i * auxhash + C2 * i * i) % currentCapacity;
        }
    }

    bool insertHelper(string key,int value){
        if(Collision == Chaining){
            int idx = primaryHash(key) % currentCapacity;
            for(auto c : chainTable[idx]){
                if(c.key == key) return false;
            }
            chainTable[idx].push_back(HashElements(key,value));
            currentSize++;
            return true;
        }
        else{
            int firstdelete =-1;
            for(int i = 0; i < currentCapacity ;i++){
                int idx = getProbeIndex(key,i);
                if(normalTable[idx].occupied){
                    if(!normalTable[idx].isDeleted && normalTable[idx].key == key) return false;
                    if(normalTable[idx].isDeleted && firstdelete == -1) firstdelete = idx;
                }
                else{
                    int finalidx = (firstdelete != -1)? firstdelete : idx;
                    normalTable[finalidx] = HashElements(key,value);
                    currentSize++;
                    return true;
                }
            }
        }
        return false;
    }

    void rehash(int capacity){
        int oldCapacity = currentCapacity;
        currentCapacity=capacity;

        if(Collision == Chaining){
            vector<list<HashElements>>oldList = move(chainTable);
            chainTable.assign(currentCapacity,list<HashElements>());
            currentSize=0;
            for(int i =0 ;i<oldList.size();i++){
                for(auto c : oldList[i]){
                    insertHelper(c.key,c.value);
                }
            }
        }

        else{
            vector<HashElements> oldOpen = move(normalTable);
            normalTable.assign(currentCapacity, HashElements());
            currentSize = 0;
            for (int i = 0; i < oldCapacity; i++) {
                if (oldOpen[i].occupied && !oldOpen[i].isDeleted) {
                    insertHelper(oldOpen[i].key, oldOpen[i].value);
                }
            }
        }
        elementsAtLastResize = currentSize;
        insertionsSinceResize =0;
        deletionsSinceResize=0;
    }

    public: 
    
    HashTable(collisionTechnique col ,int HashT):
    currentCapacity(INITIAL_SIZE),currentSize(0),HashType(HashT),Collision(col),
    insertionsSinceResize(0),deletionsSinceResize(0),elementsAtLastResize(0){
        if(Collision == Chaining){
            chainTable.resize(currentCapacity);
        }
        else{
            normalTable.resize(currentCapacity);
        }
    }

    long long auxHash(string & key){
        int R = 7;
        if(R >= currentSize){
            R = 3;
        }
        long long hashVal = Hash1(key);
        return R - (hashVal % R);
    }

    bool insert(string key,int value,int & totalCollision){
        int primaryIndex = primaryHash(key) % currentCapacity;
        if(Collision == Chaining){
            if(!chainTable[primaryIndex].empty()) totalCollision++;
        }
        else{
            if(normalTable[primaryIndex].occupied && !normalTable[primaryIndex].isDeleted) totalCollision++;
        }
        bool insertStatus = insertHelper(key,value) ;
        if(insertStatus){
            insertionsSinceResize++;
            double loadFactor = (double)currentSize / currentCapacity;
            if(loadFactor>LOAD_FACTOR_HIGH && insertionsSinceResize * 2 >= elementsAtLastResize){
                int nextCapacity = nextPrime(2*currentCapacity);
                rehash(nextCapacity);
            }
        }
        return insertStatus;
    }

    bool search(string key,int & hits){
        hits = 0 ; 
        if(Collision == Chaining){
            int idx = primaryHash(key) % currentCapacity;
            for(auto c : chainTable[idx]){
                hits++;
                if(c.key == key) return true;
            }
            return false;
        }
        else{
            for(int i =0;i<currentCapacity;i++){
                hits++;
                int idx = getProbeIndex(key,i);
                if(!normalTable[idx].occupied) return false;
                if(!normalTable[idx].isDeleted && normalTable[idx].key == key) return true;
            }

        }
        return false;
    }

    bool remove(string key){
        bool Deleted = false;
        if(Collision == Chaining){
        int idx = primaryHash(key) % currentCapacity;
        for(auto i = chainTable[idx].begin(); i!= chainTable[idx].end();i++){
            if(i->key == key){
                chainTable[idx].erase(i);
                currentSize--;
                Deleted = true;
                break;
            }
        }
        }
        else{
            for(int i =0;i<currentCapacity;i++){
                int idx = getProbeIndex(key,i);
                if(!normalTable[idx].occupied) break;
                if(!normalTable[idx].isDeleted && normalTable[idx].key == key){
                    normalTable[idx].isDeleted = true;
                    currentSize--;
                    Deleted = true;
                    break;
                }
            }
        }

        if(Deleted){
            deletionsSinceResize++;
            double loadFactor = (double) currentSize / currentCapacity;
            if(currentCapacity > INITIAL_SIZE && loadFactor<LOAD_FACTOR_LOW && deletionsSinceResize * 2 >= elementsAtLastResize){
                int newCap = prevPrime(currentCapacity /2);
                if(newCap < INITIAL_SIZE) newCap = INITIAL_SIZE;
                rehash(newCap);
            }
        }
        return Deleted;
    }

};

vector<string> Wordgenerator(int count,int length){
    mt19937 rng(random_device{}());  //If each time same output is expected: rng(1337)
    uniform_int_distribution<int> dist(0,25);
    unordered_set<string> uniqueSet;
    vector<string> result;
    while(result.size() < count){
        string word = "";
        for(int i = 0;i< length;i++){
            word+= (char)('a' + dist(rng));
        }
        if(uniqueSet.insert(word).second){
            result.push_back(word);
        }
    }
    return result;
}

int main() {
    int N_WORDS = 10000; 
    int WORD_LEN = 10;  
    int SEARCH_COUNT = 1000;  

    vector<string> words = Wordgenerator(N_WORDS, WORD_LEN);  

    collisionTechnique techniques[] = { Chaining, Double_Hashing, Custom_Probing };  
    string techNames[] = { "Chaining Method", "Double Hashing", "Custom Probing" };

    cout <<left <<setw(20) << "Technique" <<setw(20) << "Hash1: Collisions" <<setw(20) << "Hash1: Avg Hits"<<setw(20) << "Hash2: Collisions" <<setw(20) << "Hash2: Avg Hits" << "\n";
    cout <<string(100, '-') << "\n";

    for (int t = 0; t < 3; t++) {

        HashTable ht1(techniques[t], 1);
        int collisions1 = 0;
        for (int i = 0; i < N_WORDS; i++) {
            ht1.insert(words[i], i + 1, collisions1);  
        }
        int totalHits1 = 0;
        for (int i = 0; i < SEARCH_COUNT; i++) {
            int hits = 0;
            ht1.search(words[i * 10], hits);  
            totalHits1 += hits;
        }

        HashTable ht2(techniques[t], 2);
        int collisions2 = 0;
        for (int i = 0; i < N_WORDS; i++) {
            ht2.insert(words[i], i + 1, collisions2);  
        }
        int totalHits2 = 0;
        for (int i = 0; i < SEARCH_COUNT; i++) {
            int hits = 0;
            ht2.search(words[i * 10], hits);  
            totalHits2 += hits;
        }

           cout <<left <<setw(20) << techNames[t]
                  <<setw(20) << collisions1 <<setw(20) << (double)totalHits1 / SEARCH_COUNT  
                  <<setw(20) << collisions2 <<setw(20) << (double)totalHits2 / SEARCH_COUNT << "\n";  
    }

    return 0;
}