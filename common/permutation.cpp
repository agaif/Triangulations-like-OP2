//
//  permutation.cpp
//  
//
//
//

#include "permutation.hpp"


Permutation::Permutation (int deg) {
    if (deg > 0) {
        well_defined = true;
        seq.resize(deg);
        for (int i = 0; i < deg; i++) seq[i] = i;
    } else {
        well_defined = false;
    }
}

Permutation::Permutation (vector <int> & seq) : seq (seq), well_defined (true) {}


Permutation::Permutation (int deg, string str) {
    if (deg <= 0) {
        well_defined = false;
        return;
    }
    
    seq.resize(deg);
    set <int> numbers_occured;
    
    int pos = 0;
    
    while (pos < str.size()) {
        if (str[pos] == ' ') {
            ++pos;
            continue;
        }
        if (str[pos] != '(') {
            well_defined = false;
            return;
        }
    
        ++ pos;
        vector <int> cycle;
        while (pos < str.size() && str[pos] != ')') {
            if (str[pos] == ' ') {
                ++pos;
                continue;
            }
            int current_number = 0;
            while (str[pos] >= '0' && str[pos] <= '9') {
                current_number = 10 * current_number + (str[pos] - '0');
                ++pos;
            }
            if (current_number > 0 && current_number <= deg
                && numbers_occured.count(current_number - 1) == 0) {
                cycle.push_back(current_number - 1);
                numbers_occured.insert(current_number - 1);
            } else {
                well_defined = false;
                return;
            }
        }
        if (str[pos] != ')') {
            well_defined = false;
            return;
        }
        
        for (int j = 0; j < cycle.size() - 1; j++) {
            seq[cycle[j]] = cycle[j + 1];
        }
        seq[cycle[cycle.size() - 1]] = cycle[0];
        ++pos;
    }
    
    for (int i = 0; i < deg; i++) {
        if (numbers_occured.count(i) == 0) {seq[i] = i;}
    }
    well_defined = true;
}


int Permutation::get_seq (int i) const { return seq[i]; }

int Permutation::degree () const { return seq.size(); }

bool Permutation::is_well_defined () const { return well_defined; }

Permutation Permutation::operator * (const Permutation & second_permutation) const {
    vector <int> result;
    result.resize(degree());
    for (int i = 0; i < degree(); i++) {
        result[i] = seq[ second_permutation.get_seq(i) ];
    }
    return Permutation (result);
}

unsigned long int Permutation::operator * (const unsigned long int & s) const {
    unsigned long int q = 0;
    for (int i = 0; i < degree(); i++) {
        if  ( ( s & (1UL << i)) != 0 ) {
            q |= 1UL << seq[i];
        }
    }
    return q;
}

bool Permutation::operator == (const Permutation & second_permutation) const {
    if (!well_defined || !second_permutation.is_well_defined() ||
        degree() != second_permutation.degree () ) {return false;}
    
    for (int i = 0; i < degree(); i++) {
        if (seq[i] != second_permutation.get_seq (i) ) {return false;}
    }
    
    return true;
}

bool Permutation::operator != (const Permutation & second_permutation) const {
    return ! (*this == second_permutation);
}

bool Permutation::operator < (const Permutation & second_permutation) const {
    for (int i = 0; i < degree(); i++) {
        if (seq[i] < second_permutation.get_seq (i) ) {return true;}
        if (seq[i] > second_permutation.get_seq (i) ) {return false;}
    }
    return false;
}


