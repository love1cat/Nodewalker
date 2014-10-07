#include <cstdlib>
#include <vector>
#include "walker.h"
#include "random.h"
#include "error.h"
#include "partialgraph.h"
#include "graph.h"

namespace nodewalker {

    void walker::jump_back() {
        ++step_;
        if (isdead()) return;
        curnode_ = orgnode_;
        prevnode_ = -1;
    }

    void walker::random_walk(const graph& g) {
        ++step_;
        if (isdead()) return;
        const std::vector<NodePtr>& pnvec = g.get_node();
        NodePtr curnptr = pnvec[curnode_];
        std::vector<int>& nbv = curnptr->nbvec;
        if (curnptr->degree <= 1 && !isDead_) {
            if (prevnode_ != -1 || curnptr->degree < 1) {
                isDead_ = true;
            } else {
                prevnode_ = curnode_;
                curnode_ = nbv[0];
            }
            return;
        }
        //random pick up one neighbor
        //since we may pick up the one previously stayed
        //we random pick up two neighbors
        //we swap the two to the front of nbv for convenience
        //note that prevnode_ can be NULL here
        generate_perm(nbv, 2);
        if (nbv[0] != prevnode_) {
            prevnode_ = curnode_;
            curnode_ = nbv[0];
        } else {
            prevnode_ = curnode_;
            curnode_ = nbv[1];
        }        
    }
    
    void walker::random_walk(const partialgraph& pg) {
        ++step_;
        if (isdead()) return;
        if(curnode_<pg.get_lownode() || curnode_>pg.get_highnode()) throw error("walker should not be in current graph!");
        const std::vector<NodePtr>& pnvec = pg.get_node();
        NodePtr curnptr = pnvec[curnode_ - pg.get_nodeoffset()];
        std::vector<int>& nbv = curnptr->nbvec;
        if (curnptr->degree <= 1 && !isDead_) {
            if (prevnode_ != -1 || curnptr->degree < 1) {
                isDead_ = true;
            } else {
                prevnode_ = curnode_;
                curnode_ = nbv[0];
            }
            return;
        }
        //random pick up one neighbor
        //since we may pick up the one previously stayed
        //we random pick up two neighbors
        //we swap the two to the front of nbv for convenience
        //note that prevnode_ can be NULL here
        generate_perm(nbv, 2);
        if (nbv[0] != prevnode_) {
            prevnode_ = curnode_;
            curnode_ = nbv[0];
        } else {
            prevnode_ = curnode_;
            curnode_ = nbv[1];
        }
    }

    void walker::generate_perm(std::vector<int>& vec, int num) {
        if (num > vec.size())
            //throw error("Error: number of random permuated element cannot exceed the size of the vector!");
            printf("number of random permuated element cannot exceed the size of the vector\n");
            return;
        for (int i = 0; i < num; i++) {
            int rid = random::get_random_number(i, vec.size() - 1);
            if (rid != i) {
                int temp = vec[i];
                vec[i] = vec[rid];
                vec[rid] = temp;
            }
        }
    }
}
