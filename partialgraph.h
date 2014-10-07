#ifndef PARTIALGRAPH_H
#define	PARTIALGRAPH_H

#include <vector>
#include <set>
#include <string>
#include "graph.h"

namespace nodewalker{   
    class partialgraph : public graph{
    private:
        int lownode_;
        int highnode_;
        int nodeoffset_;
        static int nodepergraph;
    public:
        static void divide_graph(const graph& g, int number, std::string outfile_prefix);
        static int get_node_rank(int nodeid);
        partialgraph(const std::string infile_prefix, int rank, bool isWalkerList = true);
        inline int get_lownode() const{return lownode_;}
        inline int get_highnode() const{return highnode_;}
        inline int get_nodeoffset() const{return nodeoffset_;}
    };
}

#endif	/* PARTIALGRAPH_H */

