/*
 Author:
    Huseyin Karakaya
*/

#include "exercise2_utils.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

typedef Node node;

node * create_bst() {
    node * tree = NULL;
    return tree;
}

node * insert(node *tree, int score, int shirt_no, bool is_home) {
    if(tree == NULL) {
        node *root = (node *)malloc(sizeof(node));
        root->left = NULL;
        root->right = NULL;
        root->score = score;
        root->shirt_no = shirt_no;
        root->is_home = is_home;
        return root;
    }
    if(tree->score < score) {
        tree->right = insert(tree->right, score, shirt_no, is_home);
        return tree;
    }
    tree->left = insert(tree->left, score, shirt_no, is_home);
    return tree;
}

void traverse(node *tree, int min, int max) {
    if(tree == NULL) {
        return;
    }
    traverse(tree->left, min, max);
    if((tree->score <= max) && (min <= tree->score)) {
        if(tree->is_home == false) {
            printf("\nA\t%d\t%d",tree->shirt_no,tree->score);
        }else {
            printf("\nH\t%d\t%d",tree->shirt_no,tree->score);
        }
    }
    traverse(tree->right, min, max);
}



bool is_home_away(string home_or_away) {
    if(home_or_away == "H") {
        return true;
    }
    return false;
}

int main(int argc, char** argv) {
    
    string path = argv[1];
    //cout << "path = " << path << endl;

    int s_min = atoi(argv[2]);
    //cout << "s_min = " << s_min << endl;

    int s_max = atoi(argv[3]);
    //cout << "s_max = " << s_max << endl;

    ifstream inputFile;
    inputFile.open(path);

    if(inputFile.fail()) {
        cerr << "Unable to open the file: " << path << endl;
        return 1;
    }

    /// Use getline() to read the first line, then begin reading the rest of the stream.
    string line = "";
    getline(inputFile, line);

    line = "";
    Node * root;
    root = create_bst();

    while(getline(inputFile, line)) {
        if(line == "") {
            break;
        }

        string tempString = "";
        string home_or_away;
        bool is_home;
        int shirt_no;
        int score;

        stringstream inputString(line);

        getline(inputString, home_or_away, ',');
        
        getline(inputString, tempString, ',');
        shirt_no = atoi(tempString.c_str());

        tempString = "";
        getline(inputString, tempString, ',');
        score = atoi(tempString.c_str());

        is_home = is_home_away(home_or_away);

        root = insert(root, score, shirt_no, is_home);
        printf("\n");
        print_binary_tree(*root);
        line = "";
    }
    inputFile.close();
    printf("\nDisplaying the players whose scores are in the range...");
    traverse(root, s_min, s_max);
    printf("\n");

    return 0;
}