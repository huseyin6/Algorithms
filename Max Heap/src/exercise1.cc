/*
 Author:
    Huseyin Karakaya
*/

#include "exercise1_utils.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Your heap functions can (but don't have to) be in these forms:
//Node* create_heap(int capacity);
//void push(Node* heap, int size, Node entry);
//Node pop(Node* heap, int size);

Node * create_heap(int capacity) {
    Node* heap = new Node[capacity];
    return heap;
}

void push(Node* heap, int size, Node entry) {
    heap[size] = entry;

    while(size>0 && heap[(size-1)/2].score<heap[size].score) {
        Node temp = heap[(size-1)/2];
        heap[(size-1)/2] = heap[size];
        heap[size] = temp;
        
        size = (size-1)/2;
    }
}

Node pop(Node* heap, int size){
    int current = 0; //Current index is 0 which is root
    Node popped = heap[0];
    heap[0] = heap[size-1];
    size -= 1;

    while((2*current+1)<size) {
        int child;
        if((2*current+2) == size) {
            child = 2*current+1;
        }else {
            if(heap[2*current+1].score>heap[2*current+2].score) {
                child = 2*current+1;
            }else {
                child = 2*current+2;
            }
        }

        if(heap[current].score<heap[child].score) {
            Node temp = heap[current];
            heap[current] = heap[child];
            heap[child] = temp;

            current = child;
        }else {
            break;
        }
    }
    return popped;
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

    int k = atoi(argv[2]);
    //cout << "k = " << k << endl;

    ifstream inputFile;
    inputFile.open(path);

    if(inputFile.fail()) {
        cerr << "Unable to open the file: " << path << endl;
        return 1;
    }
    int size = 20;
    Node* heap = create_heap(size);

    /// Use getline() to read the first line, then begin reading the rest of the stream.
    string line = "";
    getline(inputFile, line);
    line = "";

    int current_size = 0;
    while(getline(inputFile, line)) {
        if(line == "") {
            break;
        }
        Node entry;
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

        entry.is_home = is_home;
        entry.shirt_no = shirt_no;
        entry.score = score;
        
        push(heap, current_size, entry);
        current_size++;

        print_complete_binary_tree(heap, current_size);
        line = "";
    }
    inputFile.close();

    printf("\nDisplaying k=%d best players...", k);
    for (int i = 0; i<k; i++) {
        Node popped = pop(heap, current_size);
        current_size--;
        if(popped.is_home == false) {
            printf("\nA\t%d\t%d",popped.shirt_no, popped.score);
        }else {
            printf("\nH\t%d\t%d",popped.shirt_no, popped.score);
        }
    }
    printf("\n");
    

    // Do this:
    // (1) Construct the heap using the file.
    // (2) Display top k players.

    // This is a simple demo for displaying a heap using dummy data entered manually
    // rather than reading the actual CSV file and relying on heap operations.
    // You code will be completely different!
    /*
    int heap_capacity = 10;
    Node* heap = new Node[heap_capacity];

    heap[0].score = 80;
    heap[0].is_home = true;
    heap[0].shirt_no = 1;

    heap[1].score = 60;
    heap[1].is_home = false;
    heap[1].shirt_no = 99;

    heap[2].score = 75;
    heap[2].is_home = true;
    heap[2].shirt_no = 3;

    heap[3].score = 60;
    heap[3].is_home = false;
    heap[3].shirt_no = 11;

    // We can add more if we want.

    int heap_size = 4;
    print_complete_binary_tree(heap, heap_size);
    */

    return 0;
}
