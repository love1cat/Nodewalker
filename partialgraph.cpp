#include <sstream>
#include <cmath>
#include <cstdio>
#include <vector>
#include "graph.h"
#include "partialgraph.h"

namespace nodewalker {

    int partialgraph::nodepergraph = 0;
    
    int partialgraph::get_node_rank(int nodeid){
        return nodeid / nodepergraph;
    }
    
    void partialgraph::divide_graph(const graph& g, int number, std::string outfile_prefix) {
        // divided to equal nodes per graph
        const int totalnode = g.get_node_num();
        nodepergraph = ceil((double) totalnode / (double) number);
        int nodecnt = 0;
        std::vector<NodePtr> pnvec = g.get_node();
        for (int i = 0; i < number; ++i) {
            // convert i to string
            std::stringstream ss_cnt;
            ss_cnt << i;
            // obtain output file name for this part of the graph
            std::string outfile = outfile_prefix + ss_cnt.str() + ".txt";
            // open file
            FILE* fp = fopen(outfile.c_str(), "w");
            int nodelimit = (nodecnt + nodepergraph) < totalnode ? (nodecnt + nodepergraph) : totalnode;
            // print node number
            fprintf(fp, "%d\n", nodelimit - nodecnt);
            // print start node
            fprintf(fp, "%d\n", nodecnt);
            // print end node
            fprintf(fp, "%d\n", nodelimit - 1);
            for (; nodecnt < nodelimit; ++nodecnt) {
                // print node id
                fprintf(fp, "%d\n", nodecnt);
                // print neighbors
                const std::vector<int>& nbvec = pnvec[nodecnt]->nbvec;
                fprintf(fp, "%d\n", nbvec.size());
                for (int j = 0; j < nbvec.size(); ++j) fprintf(fp, "%d ", nbvec[j]);
                fprintf(fp, "\n");
                // print walkers
                const std::vector<int>& walkervec = pnvec[nodecnt]->walkervec;
                fprintf(fp, "%d\n", walkervec.size());
                for (int j = 0; j < walkervec.size(); ++j) fprintf(fp, "%d ", walkervec[j]);
                fprintf(fp, "\n");
            }
            fclose(fp);
        }
    }

    partialgraph::partialgraph(const std::string infile_prefix, int rank, bool isWalkerList) : graph() {
        // convert i to string
        std::stringstream ss_cnt;
        ss_cnt << rank;
        // obtain output file name for this part of the graph
        std::string infile = infile_prefix + ss_cnt.str() + ".txt";
        FILE* fp = fopen(infile.c_str(), "r");
        fscanf(fp, "%d", &nnode_);
        fscanf(fp, "%d", &lownode_);
        fscanf(fp, "%d", &highnode_);
        nodeoffset_ = lownode_;
        for (int i = 0; i < nnode_; ++i) {
            NodePtr np(new node());
            int nodeid;
            fscanf(fp, "%d", &nodeid);
            np->id = nodeid;
            int nbcnt;
            fscanf(fp, "%d", &nbcnt);
            np->degree = nbcnt;
            for (int j = 0; j < nbcnt; ++j) {
                int curnb;
                fscanf(fp, "%d", &curnb);
                np->nbvec.push_back(curnb);
            }
            int walkercnt;
            fscanf(fp, "%d", &walkercnt);
            for (int j = 0; j < walkercnt; ++j) {
                int curwalker;
                fscanf(fp, "%d", &curwalker);
                WalkerPtr wp(new walker(curwalker, nodeid));
                //np->walkervec.push_back(curwalker);
                if(isWalkerList)
                    pwlist_.push_back(wp);
                else
                    pwvec_.push_back(wp);
            }
            pnvec_.push_back(np);
        }
    }
}
