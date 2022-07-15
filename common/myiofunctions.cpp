//
//  myiofunctions.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2022
//

#include "myiofunctions.hpp"


// function for reading from file and generating the symmetry group
bool generate_group (ifstream & file, int & degree, int & group_order,
                     vector <Permutation> & group_elements) {
    
    group_elements.clear();
    string str;
    getline (file, str);
    while (str[0] == ' ') str.erase(0,1);
    degree = 0;
    while ('0' <= str[0] && str[0] <= '9') {
        degree = 10 * degree + (str[0] - '0');
        str.erase(0,1);
    }
    
    set <Permutation> generators;
    
    
    Permutation e (degree);
    
    while(getline(file,str)) {
        Permutation g (degree, str);
        if ( ! g.is_well_defined() ) {return false;}
        if ( g != e ) {
            generators.insert(g);
        }
    }
    
    set <Permutation> set_of_group_elements;
    group_elements.push_back(e);
    set_of_group_elements.insert(e);
    
    int i = 0;
    while (i < group_elements.size()) {
        for (auto g : generators) {
            Permutation h = g * group_elements[i];
            if ( set_of_group_elements.count(h) == 0 ) {
                group_elements.push_back(h);
                set_of_group_elements.insert(h);
            }
        }
        ++i;
    }
    
    group_order = group_elements.size();
    
    return true;
}


// functions for writing simplices to file
void print_simplices (ofstream & file, int number_of_vertices,
                      const set <unsigned long int> & simplices) {
    file << simplices.size() << endl;
    for (auto s : simplices) {
        file << simplex_to_string (number_of_vertices, s) << endl;
    }
}


// functions for transforming string to unsigned long int encoding a simplex
unsigned long int string_to_simplex (const string & str) {
    int num;
    return string_to_simplex (num, str);
}

unsigned long int string_to_simplex (int & num, const string & str) {
    unsigned long int s = 0;
    num = 0;
    while (num < str.size() && (str[num] == '0' || str[num] == '1')) {
        if (str[num] == '1') {
            s += (1UL << num);
        }
        ++num;
    }
    return s;
}

// function that transforms simplex encoded in 'unsigned long int'
// to string of 0's and 1's
string simplex_to_string (int number_of_vertices, unsigned long int s) {
    string str;
    for (int i = 0; i < number_of_vertices; i++) {
        if ( (s & (1UL << i)) != 0) {
            str = str + "1";
        } else {
            str = str + "0";
        }
    }
    return str;
}



// function computing the number of vertices in the simplex
int num_vert (int number_of_vertices, unsigned long int s) {
    int number = 0;
    unsigned long int t = 0;
    for (int i = 0; i < number_of_vertices; i++) {
        if ( (s & (1UL<<i)) != 0 ) { ++number; }
    }
    return number;
}



// functions for reading a triangulation from file
bool read_triang (ifstream & file, int number_of_vertices,
                  set <unsigned long int> & orbit_rep) {
    int a, b;
    return read_triang (file, number_of_vertices, a, b, orbit_rep);
}

bool read_triang (ifstream & file, int number_of_vertices,
                  int & number_of_vertices_in_simplex,
                  int & number_of_orbits, set <unsigned long int> & orbit_rep) {
    string str;
    
    getline(file, str);
    number_of_orbits = stoi (str);
    
    for (int i = 0; i < number_of_orbits; i++) {
        getline(file, str);
        int num;
        unsigned long int s = string_to_simplex (num, str);
        if (num != number_of_vertices) return false;
        orbit_rep.insert (s);
        if (i == 0) {
            number_of_vertices_in_simplex = num_vert (number_of_vertices, s);
        } else {
            if (num_vert (number_of_vertices, s) != number_of_vertices_in_simplex) return false;
        }
    }
    
    return true;
}



