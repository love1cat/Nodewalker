#ifndef WALKER_H
#define	WALKER_H

#include <boost/shared_ptr.hpp>
#include "node.h"

namespace nodewalker {
    class walker;
    typedef boost::shared_ptr<walker> WalkerPtr;
    
    class partialgraph;
    class graph;
    class walker {
    private:
        static void generate_perm(std::vector<int>&, int);
        const int id_;
        const int orgnode_;
        int curnode_;
        int prevnode_;
        int step_;
        bool isDead_;
    public:
        inline walker(const int id, int orgnode) 
                : id_(id), orgnode_(orgnode), curnode_(orgnode), prevnode_(-1), isDead_(false), step_(0) {}
        inline walker(int orgnode, int curnode, int prevnode)
                : id_(-1), orgnode_(orgnode), curnode_(curnode), prevnode_(prevnode), isDead_(false){}
        const bool isdead() const {return isDead_;}
        void jump_back();
        void random_walk(const partialgraph& pg);
        void random_walk(const graph& g);
        inline int get_curnode() const {return curnode_;}
        inline int get_prevnode() const {return prevnode_;}
        inline int get_orgnode() const {return orgnode_;}
        inline int get_id() const {return id_;}
        
        inline int get_step(){return step_;}
    };
}

#endif	/* WALKER_H */

