//
//  triang.cpp
//
//
//  (C) Alexander A. Gaifullin, 2023
//


using namespace std;

#include <iostream>
#include "triang.hpp"


/***********************************
 **** Functions of class Triple ****
 ***********************************
 */

Triple::Triple (unsigned long int p, unsigned long int q, unsigned long int r) {
    if (p < q && p < r) {
        delta[0] = p;
        delta[1] = q;
        delta[2] = r;
    } else if (q < r) {
        delta[0] = q;
        delta[1] = r;
        delta[2] = p;
    } else {
        delta[0] = r;
        delta[1] = p;
        delta[2] = q;
    }
}

bool Triple::operator < (const Triple & t) const {
    for (int i = 0; i < 3; i++) {
        if (delta[i] < t.delta[i]) return true;
        if (delta[i] > t.delta[i]) return false;
    }
    return false;
}

bool Triple::operator == (const Triple & t) const {
    return delta[0] == t.delta[0] && delta[1] == t.delta[1] && delta[2] == t.delta[2];
}

Triple Triple::opp () const {
    return Triple (delta[0],delta[2],delta[1]);
}

Triple operator * (const Permutation & g, const Triple & t) {
    return Triple (g*t.delta[0], g*t.delta[1], g*t.delta[2]);
}


/***********************************
 **** Functions of class Triple ****
 ***********************************
 */

Triang::Triang (set <unsigned long int> & simp) :
    simp(simp) { compute_cert(); }


bool Triang::isomorph (const Triang & b, const set <Permutation> & group, Permutation & isomorphism) const {
    if (cert != b.cert ) {  return false; }

    vector <vector <vector <int> > > mat1 = neigh_matrix();
    vector <vector <vector <int> > > mat2 = b.neigh_matrix();

    vector <int> nu;
    /* Vector encoding a permutation
     * providing a required isomorphism.
     * We try to choose the values nu[i] one-by-one
     */

    set <int> taken;
    /* The set of vertices that are currently taken to nu */

    nu.push_back(0);
    taken.insert(0);
    int len = 1;
    /* The number of vertices that are currently taken to nu */

    while (true) {

        /* Checking whether the current partial permutation nu
         * is compatible with the 3-dim matrices mat1 and mat2.
         * If not, then nu cannot be extended to an isomorphism.
         * The third argument in mat1[i][j][k] is set to len-1,
         * since smaller values had already been checked on previous steps.
         */
        bool good = true;
        for (int i = 0; good && i < len; i++) {
            for (int j = i; good && j < len; j++) {
                if  (mat1[i][j][len-1] != mat2[nu[i]][nu[j]][nu[len-1]]) {
                    good = false;
                }
            }
        }


        /* If nu is good but not yet a complete permutation, then
         * we try to extend nu by one more element
         */
        if (good && len < 15) {
            for (int k = 0; k < 15; k++) {
                if (taken.count(k) != 0) continue;
                nu.push_back(k);
                taken.insert(k);
                ++len;
                break;
            }
            continue;
        }

        /* If nu is good and is already a complete permutation, then
         * we check directly whether nu is a weak G-equivariant isomorphism
         */
        if (good) {
            Permutation g (nu);
            bool is_symmetry = true;

            // Checking whether nu is an isomorphism
            for (auto s : this->simp) {
                if (b.simp.count(g*s) == 0) {
                    is_symmetry = false;
                    break;
                }
            }

            // Checking whether nu is weak G-equivariant
            if (is_symmetry) {
                for (auto h : group) {
                    if (group.count(g*h*g.inv()) == 0) {
                        is_symmetry = false;
                        break;
                    }
                }
            }

            // If "yes", we are done
            if (is_symmetry) {
                isomorphism = g;
                return true;
            }
        }

        /* Trying to pass to the next partial permutation nu
         * by removing several last entries of it
         * and adding one new entry.
         */
        while (true) {
            if (len == 0) return false; /* If this happens, we have
                                         * already considered all the permutations
                                         */
            int m = nu[len-1];
            nu.pop_back();
            taken.erase(m);
            --len;

            int flag = 0; /* Flag indicating whether we have succeeded
                           * to find the next partial permutation nu
                           */
            for (int k = m+1; k < 15; k++) {
                if (taken.count(k) == 0) {
                    nu.push_back(k);
                    taken.insert(k);
                    ++ len;
                    flag = 1;
                    break;
                }
            }
            if (flag == 1) break;
        }
    }
    return false;
}



bool Triang::isomorph (const Triang & b, const set <Permutation> & group) const {
    Permutation isomorphism (15);
    return isomorph(b, group, isomorphism);
}

set <Permutation> Triang::symm_group () const {
    set <Permutation> trivial_group = {Permutation(15)};
    return equiv_symm_group(trivial_group);
}

int Triang::symm_group_order () const {
    return symm_group().size();
}

set <Permutation> Triang::equiv_symm_group (const set <Permutation> & group) const {
    set <Permutation> result_group; // The elements of the required group
    vector <vector <vector <int> > > mat = neigh_matrix();

    vector <int> nu;
    /* Vector encoding a permutation
     * providing a required isomorphism.
     * We try to choose the values nu[i] one-by-one
     */

    set <int> taken;
    /* The set of vertices that are currently taken to nu */

    nu.push_back(0);
    taken.insert(0);
    int len = 1;
    /* The number of vertices that are currently taken to nu */

    while (true) {
        /* Checking whether the current partial permutation nu
         * is compatible with the 3-dim matrix mat.
         * If not, then nu cannot be extended to an automorphism.
         * The third argument in mat[i][j][k] is set to len-1,
         * since smaller values had already been checked on previous steps.
         */
        bool good = true;
        for (int i = 0; good && i < len; i++) {
            for (int j = i; good && j < len; j++) {
                if  (mat[i][j][len-1] != mat[nu[i]][nu[j]][nu[len-1]]) {
                    good = false;
                }
            }
        }

        /* If nu is good but not yet a complete permutation, then
         * we try to extend nu by one more element
         */
        if (good && len < 15) {
            for (int k = 0; k < 15; k++) {
                if (taken.count(k) != 0) continue;
                nu.push_back(k);
                taken.insert(k);
                ++len;
                break;
            }
            continue;
        }

        /* If nu is good and is already a complete permutation, then
         * we check directly whether nu is a weak G-equivariant automorphism
         */
        if (good) {
            Permutation g (nu);
            bool is_symmetry = true;

            // Checking whether nu is an isomorphism
            for (auto h : group) {
                if (group.count(g*h*g.inv()) == 0) {
                    is_symmetry = false;
                    break;
                }
            }

            // Checking whether nu is weak G-equivariant
            if (is_symmetry) {
                for (auto s : simp) {
                    if (simp.count(g*s) == 0) {
                        is_symmetry = false;
                        break;
                    }
                }
            }

            // If "yes" add g to the resulting group
            if (is_symmetry) result_group.insert(g);
        }

        /* Trying to pass to the next partial permutation nu
         * by removing several last entries of it
         * and adding one new entry.
         */
        while (true) {
            if (len == 0) {
                /* If this happens, we have
                 * already considered all the permutations
                 */
                return result_group;
            }

            int m = nu[len-1];
            nu.pop_back();
            taken.erase(m);
            --len;

            int flag = 0; /* Flag indicating whether we have succeeded
                           * to find the next partial permutation nu
                           */
            for (int k = m+1; k < 15; k++) {
                if (taken.count(k) == 0) {
                    nu.push_back(k);
                    taken.insert(k);
                    ++ len;
                    flag = 1;
                    break;
                }
            }
            if (flag == 1) break;
        }
    }
    return result_group;
}


vector <unsigned int> Triang::distrib() const {
    unsigned long long int cert_copy = cert;
    vector <unsigned int> res;
    res.resize(6);
    for (int i = 1; i < 6; i++) {
        res[i] = cert_copy % 4096;
        cert_copy = (cert_copy>>12);
    }
    res[0] = 4230 - res[1] - res[2] - res[3] - res[4] - res[5];
    return res;
}




set <Triple> Triang::compute_triples (const set <Permutation> & group) const {
    set <Triple> result;
    set <unsigned long int> simplices_considered;
    for (auto s : simp) {

        set <unsigned long int> nu_vertices[15];
        /* For each i not in the simplex s,
         * nu_vertices[i] will be the set of all vertices j in s
         * such that (s\cup j)\setminus i is a simplex of K.
         * So nu_vertices[i].size() will be the nu-parameter nu(s,i).
         */

        // Computing the sets nu_vertices
        for (int i = 0; i < 15; i++) {
            if ( (s & (1UL<<i)) == 0 ) continue;
            unsigned long int q = s ^ (1UL<<i);
            for (int j = 0; j < 15; j++) {
                if ( (q & (1UL<<j)) == 0 && j != i && simp.count(q|(1UL<<j)) != 0 ) nu_vertices[j].insert(i);
            }
        }

        // Generating the weak G-equivariant automorphism group
        set <Permutation> auto_group = equiv_symm_group(group);

        /* Finding (the smallest) representatives of distinguished triples
         * using the property that a 8-simplex lies in a distinguished subcomplex
         * if and only if one of its nu-parameters is 4
         */
        for (int j = 0; j < 15; j++) {
            if (nu_vertices[j].size() == 4) {
                unsigned long int p = 1UL<<j;
                for (auto i : nu_vertices[j]) p |= 1UL<<i;
                unsigned long int r = s & ~p;
                unsigned long int q = ((1UL<<15) - 1) & ~p & ~r;
                Triple t(p,q,r);
                if (simplices_considered.count(t.delta[0]) != 0) continue;
                Triple t_min = t;

                bool conflict = false;
                /* Variable conflict indicates whether a distinguished triple is admissible
                 * with respect to G.
                 * conflict has value true if the distinguished subcomplex can be taken by a
                 * weak G-equivariant automorphism to a different distinguished subcomplex that
                 * has a common 8-simplex with the original one.
                 */

                for (auto g : auto_group) {
                    Triple t_curr = g * t;
                    if (t_curr < t_min) t_min = t_curr;
                    simplices_considered.insert(t_curr.delta[0]);
                    if (group.count(g) != 0 && !conflict) {
                        for (int i = 0; i < 3; i++) {
                            if ((t_curr.delta[i] != p && t_curr.delta[i] != q
                                && (t_curr.delta[i]|p|q) == (p|q))
                                || (t_curr.delta[i] != q && t_curr.delta[i] != r
                                && (t_curr.delta[i]|q|r) == (q|r))
                                || (t_curr.delta[i] != p && t_curr.delta[i] != r
                                && (t_curr.delta[i]|p|r) == (p|r))) {
                                conflict = true;
                                break;
                            }
                        }
                    }

                }
                if (!conflict) result.insert(t_min);
            }
        }
    }
    return result;
}


void Triang::apply_triple_flip (const set <Permutation> & group, const Triple & t) {

    // Computing the G-orbit of triple t
    set <Triple> orbit_of_triple;
    for (auto g : group) {
        orbit_of_triple.insert(g*t);
    }

    // Appllying triple flips
    for (auto triple : orbit_of_triple) {
        for (int i = 0; i < 15; i++) {
            int k = 0;
            while ((triple.delta[k]&(1UL<<i)) == 0) ++k;
            int l = (k+1)%3;
            int m = (k+2)%3;
            simp.erase(triple.delta[m]|(triple.delta[k]^(1UL<<i)));
            simp.insert(triple.delta[l]|(triple.delta[k]^(1UL<<i)));
        }
    }

    // Computing the certificate of the obtained triangulation
    compute_cert();
}

void Triang::compute_cert () {
    cert = 0;

    set <unsigned long int> codim_two_faces; // codimension 2 faces already considered

    for (auto s : simp) {      // cycle over all max simplices
        for (int i = 0; i < 15; i++) {
            if ( ( s & (1UL<<i) ) == 0 ) continue;
            for (int j = i + 1; j < 15; j++) {
                if ( ( s & (1UL<<j) ) == 0 ) continue;

                unsigned long int q = s^(1UL<<i)^(1UL<<j);   // codim 2 face
                if (codim_two_faces.count(q) != 0) continue; // skip if already considered
                codim_two_faces.insert(q);

                int number = 0;
                for (int k = 0; k < 15; k++) {
                    if ( ( q & (1UL<<k) ) != 0 ) continue;
                    for (int l = k + 1; l < 15; l++) {
                        if ( ( q & (1UL<<l) ) != 0 ) continue;
                        if (  simp.count(q | (1UL<<k) | (1UL<<l)) != 0 ) number++;
                    }
                }
                if (number > 3) {
                    cert += (1UL<<(12*(number-4)));
                }
            }
        }
    }
}




vector <vector <vector <int> > > Triang::neigh_matrix () const {
    vector <vector <vector <int> > > mat;
    mat.resize(15);
    for (int i = 0; i < 15; i++) {
        mat[i].resize(15);
        for (int j = 0; j < 15; j++) {
            mat[i][j].resize(15);
            for (int k = 0; k < 15; k++) {
                if (j < i) {
                    mat[i][j][k] = mat[j][i][k];
                    continue;
                }
                if (k < j) {
                    mat[i][j][k] = mat[i][k][j];
                    continue;
                }
                unsigned long int t = (1UL<<i)|(1UL<<j)|(1UL<<k);
                mat[i][j][k] = 0;
                for (auto s : simp) {
                    if ((s&t)==t) mat[i][j][k]++;
                }
            }
        }
    }
    return mat;
}


