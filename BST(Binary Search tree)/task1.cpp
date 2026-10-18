#include <iostream>
#include <fstream>
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
        return 2;
    }
    char c, str[5];
    int val;
    BST<int, int> *bst = new ListBST<int, int>();
    if (!bst) {
        cerr << "Memory allocation failed\n";
        return 3;
    }
    while (in_file >> c) {
        // TODO: Implement the logic to read commands from the file and output accordingly
        // After every insertion and removal, print the BST in nested parentheses format
        // Handle exceptions where necessary and print appropriate error messages

        // Start your code here
        if(c == 'F' && in_file>>val){
            if(bst->find(val)){
                std::cout<<"Key "<<val<<" found in BST."<<std::endl;
            }
            else{
                std::cout<<"Key "<<val<<" not found in BST."<<std::endl;
            }

        }
        else if(c == 'E'){
            if(bst->empty()){
                std::cout<<"Empty"<<std::endl;
            }
            else std::cout<<"Not empty"<<std::endl;
        }
        else if(c == 'I' && in_file>>val){
            if(bst->insert(val,val)){
                std::cout<<"Key "<<val<<" inserted into BST, ";
                bst->print();
            }
            else{
                std::cout<<"Insertion failed! Key "<<val<<" already exists in BST, ";
                bst->print();
            }
        }
        else if(c == 'M'){
            in_file>>str ;
            if(str == "Min"){
                std::cout<<"Minimum value: "<<bst->find_min()<<std::endl;
            }
            else if(str == "Max"){
                std::cout<<"Maximum value: "<<bst->find_max()<<std::endl;
            }
        }
        else if(c == 'S'){
            std::cout<<"Size: "<<bst->size()<<std::endl;
        }
        else if(c == 'T'){
            in_file>>str;
            if(str == "In"){
                bst->print('I');
            }
            else if(str == "Pre"){
                bst->print('p');
            }
            else if(str =="Post"){
                bst->print('o');
            }
        }
        else if(c == 'D' && in_file>>val){
            if(bst->remove(val)){
                std::cout<<"Key "<<val<<" removed from BST, ";
                bst->print();
            }
            else{
                std::cout<<"Removal failed! Key "<< val<<" not found in BST, ";
                bst->print();
            }
        }
        // End your code here
    }
    in_file.close();
    delete bst;
    return 0;
}
