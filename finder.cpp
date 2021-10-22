#include <iostream>
#include <map>
#include <fstream>
#include <string>

using namespace std;

struct Node {
    map<char, Node> m;
};

char rot13(char c) {
    if (c == '\0' || c == '-') return c;
    return (((c-'a')+13)%26)+'a';
}

string rot13str(string s) {
    string ns = "";
    for (char c : s) {
        ns += rot13(c);
    }
    return ns;
}

// generate tree from word file
Node generateFromFile(const char* path) {
    fstream f(path, ios::in);
    if (!f) throw;
    
    Node n;
    
    string line;
    while (getline(f, line)) {
        // discard entries with non-ascii letters
        bool discard = false;
        for (char c : line) {
            c = tolower(c);
            if (!((c>='a' && c<='z') || c=='\r' || c == '-'))
                discard = true;
        }
        if (discard) continue;
        // add nodes for each letter
        Node *ptr = &n;
        for (char c : line) {
            c = tolower(c);
            if (c == '\r') continue;
            auto it = ptr->m.find(c);
            if (it == ptr->m.end()) {
                ptr->m[c] = Node();
            }
            ptr = &(ptr->m[c]);
        }
        // terminate with null to indicate full word
        ptr->m['\0'] = Node();
    }
    return n;
}

// print list to verify words are correctly registered
void print(Node n, string s="") {
    for (auto it : n.m) {
        char c = it.first;
        if (c == '\0') cout << s << endl;
        else {
            string ns = s;
            ns += c;
            print(it.second, ns);
        }
    }
}

// find if each word has a rot13 counterpart
void find_rot13(Node *ptr, Node *ptr2, string s="", int level=0) {
    for (auto it : ptr->m) {
        char c = it.first;
        if (level == 0 && c >= 'a'+13) return;
        char r = rot13(c);
        // find counterpart for current letter
        auto it2 = ptr2->m.find(r);
        if (it2 == ptr2->m.end()) continue;
        // from this point there are counterparts for partial words
        
        // if terminating character full word is indeed found
        if (c == '\0') {
            cout << s << "/" << rot13str(s) << endl;
        }
            
        string ns = s;
        ns += c;
        // find for next partial words
        find_rot13(&(it.second), &(it2->second), ns,level+1);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        cout << argv[0] << " " << "filename" << endl;
        return 0;
    }
    Node n = generateFromFile(argv[1]);
    //print(n);
    
    find_rot13(&n, &n);
    
    return 0;
}
