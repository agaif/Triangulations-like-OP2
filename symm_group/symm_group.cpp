//
//  symm_group.cpp
//  
//
//  (C) Alexander A. Gaifullin, 2023
//


using namespace std;

#include <iostream>
#include "../common/permutation.hpp"
#include "../common/myiofunctions.hpp"
#include "../common/triang.hpp"


#define ifile_name_triang "triang.dat"
#define ofile_name "symm_group.dat"


int main() {
    
    // Reading the triangulation (list of all max simplices)
    ifstream file_triang (ifile_name_triang, ios::in);
    int number_of_vertices_in_simplex, number_of_simplices;
    set <unsigned long int> all_max_simp;
    if ( !read_triang(file_triang, 15, number_of_vertices_in_simplex,
                      number_of_simplices, all_max_simp) || number_of_vertices_in_simplex != 9 || number_of_simplices != 490) {
        cout << "INCORRECT INITIAL DATA : Triangulation" << endl;
        file_triang.close();
        return 0;
    }
    cout << "Triangulation read\n";
    file_triang.close ();
    
    // Opening output file
    ofstream ofile(ofile_name, ios::out);

    set <Permutation> group = Triang(all_max_simp).symm_group();
    Permutation trivial_permutation(15);

    ofile << group.size() << endl;

    for (auto g : group) {
        if (g != trivial_permutation) {
            ofile << g.cycle_structure() << endl;
        }
    }


    // Closing output file
    ofile.close();

    return 0;
}
 

