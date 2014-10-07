#ifndef NODE_H
#define	NODE_H

#include <vector>
#include <map>
#include <set>
#include <boost/shared_ptr.hpp>

namespace nodewalker {
    class node;
    typedef boost::shared_ptr<node> NodePtr;
    
    class node {
    public:
        std::vector<int> nbvec;
        std::set<int> nbset;
        std::vector<int> walkervec;
        node();
        int id;
        int clusterid;
        bool ishead;
        int degree;

        //friend class walker;
    };
}

#endif	/* NODE_H */

