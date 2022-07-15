//
//  operations.cpp
//  
//  (C) Alexander A. Gaifullin, 2022
//
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <string>

using namespace std;


#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"

#define ifile_name1 "triang1.dat"
#define ifile_name2 "triang2.dat"
#define ofile_name "triang_new.dat"

void compare (const set <string> & triang1, const set <string> & triang2) {
    bool cont1in2 = true, cont2in1 = true;
    for (auto s : triang1) {
        if (triang2.count(s) != 0) {
            cont1in2 = false;
            break;
        }
    }
    for (auto s : triang2) {
        if (triang1.count(s) != 0) {
            cont2in1 = false;
            break;
        }
    }
    
    if (cont1in2 && cont2in1) cout << "The complexes coincide\n";
    if (cont1in2 && !cont2in1) cout << "The first complex is contained in the second one\n";
    if (!cont1in2 && cont2in1) cout << "The second complex is contained in the first one\n";
    if (!cont1in2 && !cont2in1) cout << "Neither of the complexes is contained in the other one\n";
}

set <string> take_union (const set <string> & triang1, const set <string> & triang2) {
    set <string> triang;
    for (auto s : triang1) {
        triang.insert (s);
    }
    for (auto s : triang2) {
        triang.insert (s);
    }
    return triang;
}

set <string> take_intersection (const set <string> & triang1, const set <string> & triang2) {
    set <string> triang;
    for (auto s : triang1) {
        if (triang2.count(s) != 0) triang.insert (s);
    }
    return triang;
}

set <string> take_difference (const set <string> & triang1, const set <string> & triang2) {
    set <string> triang;
    for (auto s : triang1) {
        if (triang2.count(s) == 0) triang.insert (s);
    }
    return triang;
}


int main() {

    set <string> triang1, triang2, triang;
    int num1, num2;
    
    // Reading the first triangulation
    ifstream file1 (ifile_name1, ios::in);
    file1 >> num1;
    for (int i = 0; i < num1; i++) {
        string s;
        file1 >> s;
        triang1.insert(s);
    }
    file1.close();
    
    // Reading the second triangulation
    ifstream file2 (ifile_name2, ios::in);
    file2 >> num2;
    for (int i = 0; i < num2; i++) {
        string s;
        file2 >> s;
        triang2.insert(s);
    }
    file2.close();
    
    // Asking for the type of operation
    cout << "Input the type of the operation:\n";
    cout << "1 compare\n";
    cout << "2 union\n";
    cout << "3 intersection\n";
    cout << "4 difference\n";
    
    // Reading the type of the operation
    int operation_type;
    cin >> operation_type;
    
    // Performing the operation
    switch (operation_type) {
        case 1:
            compare (triang1, triang2);
            break;
        
        case 2:
            triang = take_union (triang1, triang2);
            break;
            
        case 3:
            triang = take_intersection (triang1, triang2);
            break;
        
        case 4:
            triang = take_difference (triang1, triang2);
            break;
            
        default:
            cout << "Bad operation type\n";
    }
    
    
    // Writing the result
    if (2 <= operation_type && operation_type <= 4) {
        ofstream ofile (ofile_name, ios::out);
        ofile << triang.size() << endl;
        for (auto s : triang) {
            ofile << s << endl;
        }
        ofile.close();
    }
        
    return 0;
}
 

