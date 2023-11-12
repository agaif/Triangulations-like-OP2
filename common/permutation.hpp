//
//  permutation.hpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#ifndef permutation_hpp
#define permutation_hpp

#include <vector>
#include <set>
#include <string>

using namespace std;

class Permutation {
private:
    vector <int> seq;
    bool well_defined;
    
public:
    Permutation (int deg);
    Permutation (int deg, string cycle_structure);
    Permutation (vector <int> & seq);
    int get_seq (int i) const;
    int degree () const;
    bool is_well_defined () const;
    Permutation inv () const;
    Permutation operator * (const Permutation & sec) const;
    unsigned long int operator * (const unsigned long int & s) const;
    bool operator == (const Permutation & sec) const;
    bool operator != (const Permutation & sec) const;
    bool operator < (const Permutation & sec) const;
    string cycle_structure();
    bool is_trivial();
};


#endif /* permutation_hpp */
