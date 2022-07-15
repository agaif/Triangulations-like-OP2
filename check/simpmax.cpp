//
//  simpmax.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#include "simpmax.hpp"

/******************************/
/*  Methods of class SimpMax  */
/******************************/

SimpMax::SimpMax(int max_num_vert) : max_num_vert (max_num_vert) { }

SimpMax::SimpMax(int max_num_vert, unsigned long int s) : max_num_vert (max_num_vert) {
    max_simplices.empty();
    max_simplices.insert(s);
}

SimpMax SimpMax::link (int v) const {
    SimpMax lk (max_num_vert);
    if (max_simplices.count(1UL << v) != 0) return lk;
    for (auto sigma : max_simplices) {
        if ((sigma & (1UL << v)) != 0) {
            lk.max_simplices.insert (sigma ^ (1UL << v));
        }
    }
    return lk;
}


SimpMax SimpMax::costar (int v) const {
    SimpMax cst (max_num_vert);
    for (auto sigma : max_simplices) {
        if ((sigma & (1UL << v)) == 0) {
            cst.max_simplices.insert (sigma);
        }
    }
    for (auto sigma : max_simplices) {
        if ((sigma & (1UL << v)) != 0) {
            unsigned long int rho = sigma ^ (1UL << v);
            if ( rho != 0 && !cst.contains (rho) )
            cst.max_simplices.insert(rho);
        }
    }
    return cst;
}


bool SimpMax::is_simplex () const {
    return (max_simplices.size() == 1);
}

bool SimpMax::empty () const {
    return (max_simplices.size() == 0);
}

bool SimpMax::contains (unsigned long int tau) const {
    for (auto sigma : max_simplices) {
        if ((sigma|tau) == sigma) return true;
    }
    return false;
}


bool SimpMax::nonevasive () const {
    if (empty()) return false;
    if (is_simplex()) return true;
    
    for (int v = 0; v < max_num_vert; v++) {
        if ( link(v).nonevasive() ) {
            if ( costar(v).nonevasive() ) return true;
        }
    }
    
    return false;
}

void SimpMax::add (const SimpMax & another_complex) {
    for (auto tau : another_complex.max_simplices) {
        add (tau);
    }
}

void SimpMax::add (unsigned long int tau) {
    for (auto it = max_simplices.begin(); it != max_simplices.end(); ) {
        if ((*it|tau) == *it) return;
        if ((*it|tau) == tau) {
            it = max_simplices.erase(it);
        } else {
            ++it;
        }
    }
    max_simplices.insert (tau);
}



