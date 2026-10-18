#include <iostream>
#include <fstream>
#include <string>
#include "listBST.hpp"
using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Usage: filename" << "\n";
        return 1;
    }
    ifstream in_file(argv[1]);
    if (!in_file) {
        cerr << "Unable to open file\n";
        return 1;
    }

    // TODO: Declare data structures to track bid statistics for each item
    // You need to track: total bids, successful bids, rejected bids for each item
    // Hint: You may use additional BSTs or other data structures
    // Start your code here
    BST<string, int> *bst = new ListBST<string, int>();
    string name;
    int bid;
    int total_Bids =0,success_bids=0,reject_bids=0;
    BST<string, int> *copy_BST1 = new ListBST<string, int>();
    BST<string, int> *copy_BST2 = new ListBST<string, int>();
    BST<string, int> *successfulBids = new ListBST<string, int>();
    BST<string, int> *rejectedBids = new ListBST<string, int>();

    // End your code here

    int n;
    in_file >> n;
    for (int i = 0; i < n; ++i) {
        // TODO: Implement the logic to read initial items and their starting bids
        // Initialize statistics tracking for each item
        // Start your code here
        in_file>>name;
        in_file>>bid;
        bst->insert(name,bid);
        copy_BST1->insert(name,bid);
        successfulBids->insert(name,0);
        rejectedBids->insert(name,0);
        // End your code here
    }

    // TODO: Implement the logic to print the initial auction state
    // Start your code here
    cout<<"Initial auction items: "<<endl;
    bst->print('i');
    cout<<endl;
    // End your code here
    cout << "\nAuction starts!\n\n";
    cout << "==============================\n";
    
    string operation;
    while (in_file >> operation) {
        // TODO: Implement the logic to process operations (ADD, BID, CHECK, STATS, REPORT) and print auction state after each operation
        // For BID operations: update statistics (total bids, successful/rejected counts)
        // For STATS operations: display statistics for the specified item
        // For REPORT operations: display comprehensive auction statistics
        // For other operations: print auction state using in-order traversal
        // Start your code here
        if(operation=="ADD"){
            in_file>>name;
            in_file>>bid;
            if(bst->insert(name,bid)){
                cout<<"Item " <<name<<" added with starting bid "<<bid<<endl;
                bst->print('i');
                copy_BST1->insert(name,bid);
                successfulBids->insert(name,0);
                rejectedBids->insert(name,0);
            }
            else{
                cout<<"Could not add the bid item,there was some error."<<endl;
            }
        }
        else if(operation=="BID"){
            in_file>>name;
            in_file>>bid;
            int highest=bst->get(name);
            total_Bids++;
            if(bid>highest){
                bst->update(name,bid);
                copy_BST1->update(name,bid);
                cout<<"Bid of "<<bid<<" on "<<name <<" accepted. Current bid: "<<bid<<endl;
                bst->print('i');
                success_bids++;
                int l = successfulBids->get(name);
                l++;
                successfulBids->update(name,l);
            }
            else{
                cout<<"Bid of "<<bid<<" on "<<name<<" rejected. Current bid: "<<highest<<endl;
                bst->print('i');
                reject_bids++;
                int l = rejectedBids->get(name);
                l++;
                rejectedBids->update(name,l);
            }
        }
        else if(operation=="CHECK"){
            in_file>>name;
            cout<<"Current bid for "<<name<<": "<<bst->get(name)<<endl;
            bst->print('i');
        }
        else if(operation=="STATS"){
            in_file>>name;
            cout<<"Statistics for "<<name<<":"<<endl;
            cout<<" Current highest bid: "<<bst->get(name)<<endl;
            cout<<" Total bids placed: "<<successfulBids->get(name)+rejectedBids->get(name)<<endl;
            cout<<" Successful bids: "<<successfulBids->get(name)<<endl;
            cout<<" Rejected bids: "<<rejectedBids->get(name)<<endl;
        }
        else if(operation=="REPORT"){
            cout<<"Auction Report:"<<endl;
            cout<<"Total items: "<<bst->size()<<endl;
            cout<<"Total bids placed: "<<total_Bids<<endl;
            cout<<"Total successful bids: "<<success_bids<<endl;
            cout<<"Total rejected bids: "<<reject_bids<<endl<<endl;
            cout<<"Item Statistics:"<<endl;
            while(!copy_BST1->empty()){
                string nm = copy_BST1->find_min();
                int b = copy_BST1->get(nm);//book: Current bid: 75, Total bids: 1, Successful: 1, Rejected: 0
                cout<<" "<<nm<<": Current bid:"<<b<<", Total bids: "<<successfulBids->get(nm)+rejectedBids->get(nm);
                cout<<", Successsful: "<<successfulBids->get(nm)<<", Rejected: "<<rejectedBids->get(nm)<<endl;
                copy_BST2->insert(nm,b);
                copy_BST1->remove(nm);
            }
            while(!copy_BST2->empty()){
                string nm = copy_BST2->find_min();
                int b = copy_BST2->get(nm);
                copy_BST1->insert(nm,b);
                copy_BST2->remove(nm);
            }


        }
        // End your code here
        cout << "==============================\n";
    }

    in_file.close();
    // TODO: Delete data structures you created
    // Start your code here

    // End your code here
    return 0;
}
