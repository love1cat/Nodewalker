#include <cstdio>
#include <cstring>
#include <set>
#include <iostream>
#include "graph.h"
#include "error.h"
#include "random.h"

namespace {
  const char COMMENT_CHAR  = '#';
  bool is_comment(const char *line) {
    // find first charactor
    while (isspace(*line)) {
      ++line;
    }
    return *line == COMMENT_CHAR;
  }
  const int MAXN = 1000;
}

namespace nodewalker {

  int graph::getID(std::string curp, bool issearch) {
    if (issearch) {
      IDMap_t::iterator iter;
      iter = idmap_.find(curp);
      if (iter != idmap_.end()) return (*iter).second;
    }
    if (nnode_ > maxnode_) return -1;
    NodePtr pn(new node());
    pn->id = nnode_;
    pn->degree = 0;
    pn->ishead = false;
    pnvec_.push_back(pn);
    idmap_.insert(IDMap_t::value_type(curp, nnode_));
    return nnode_++;
  }

  graph::graph(const char *inputfile, const int maxnode) : inputfile_(inputfile), nnode_(0), maxnode_(maxnode) {
    using std::cout;
    using std::endl;

    FILE * pfile;
    char* line = new char[MAXN];
    char* word1 = new char[MAXN];
    char* word2 = new char[MAXN];
    pfile = fopen(inputfile_.c_str(), "r");
    //int linecnt = 0;
    if (pfile == NULL) {
      printf("error !\n");
      throw;
    } else {
      //FILE *edgefp = fopen("nwedges.txt", "w");
      //fclose(edgefp);
      while (fgets(line, MAXN, pfile) != NULL) {
        if (is_comment(line)) {
          continue;
        }
        int len = 0;
        sscanf(line, "%s%s", word1, word2);
        int id1 = getID(word1, true);
        if (id1 == -1 ) continue;
        int id2 = getID(word2, true);
        if (id2 == -1) continue;

        pnvec_[id1]->nbset.insert(id2);
        pnvec_[id2]->nbset.insert(id1);
        //cout << "id1: " << id1 << "id2: " << id2 << endl;
      }
    }
    // construct nbvec from nbset for every node
    IDMap_t::iterator iterm;
    std::set<int>::iterator iters;
    for (iterm = idmap_.begin(); iterm != idmap_.end(); ++iterm) {
      int curid = (*iterm).second;
      NodePtr pn = pnvec_[curid];
      for (iters = pn->nbset.begin(); iters != pn->nbset.end(); ++iters) {
        pn->nbvec.push_back((*iters));
        ++pn->degree;
      }
    }
    fclose(pfile);
    delete[] line;
    delete[] word1;
    delete[] word2;

    this->generate_walker(false);
  }

  graph::graph(int nnode, int ncluster, bool isWalkerList)
    : nnode_(nnode), ncluster_(ncluster) {
      if (nnode <= 0 || ncluster <= 1)
        throw error("negative node number or cluster number!");
      for (int i = 0; i < nnode; ++i) {
        NodePtr nd(new node());
        nd->id = i;
        nd->degree = 0;
        nd->ishead = false;
        pnvec_.push_back(nd);
      }
      // evenly divide cluster, first node of the cluster will be
      // cluster head
      pch_.reset(new cluster[ncluster]);
      int node_per_cluster = nnode / ncluster;
      if (node_per_cluster <= 1)
        throw error("nodes per cluster are too small!");

      for (int i = 0, cid = 0; i < nnode; i += node_per_cluster, ++cid) {
        pnvec_[i]->ishead = true;
        pnvec_[i]->clusterid = cid;
        pch_[cid].headid = i;
        pch_[cid].lowid = i + 1 >= nnode ? nnode - 1 : i + 1;
        if (cid == ncluster - 1) {
          pch_[cid].highid = nnode - 1;
          break;
        }
        pch_[cid].highid = i + node_per_cluster - 1 >= nnode ? nnode - 1 : i + node_per_cluster - 1;
      }

      /* Please make sure no repetitive connection below */
      /* or neighbor and degree or have problems.        */

      // connect cluster head with its member
      // note: lowid may exceed maximum node num (last single cluster)
      for (int i = 0; i < ncluster; ++i)
        for (int j = pch_[i].lowid; j < nnode && j <= pch_[i].highid; ++j) {
          connect(pch_[i].headid, j);
          pnvec_[j]->clusterid = i;
        }

      // connect all cluster head
      for (int i = 0; i < ncluster; ++i)
        for (int j = i + 1; j < ncluster; ++j)
          connect(pch_[i].headid, pch_[j].headid);

      // randomly connect other members among clusters
      // if two nodes are in the same cluster, connect them with high prob.
      // otherwise, connect them with low prob.
      double highprob = 0.85;
      double lowprob = 0.10;
      for (int i = 0; i < nnode; ++i) {
        if (pnvec_[i]->ishead) continue;
        for (int j = i + 1; j < nnode; ++j) {
          if (pnvec_[j]->ishead) continue;
          bool okconn;
          if (pnvec_[i]->clusterid == pnvec_[j]->clusterid)
            okconn = random::get_probability(highprob);
          else
            okconn = random::get_probability(lowprob);
          if (okconn) connect(i, j);
        }
      }

      this->generate_walker(isWalkerList);
    }

  void graph::connect(int nid1, int nid2) {
    if (nid1 != nid2) {
      pnvec_[nid1]->nbvec.push_back(nid2);
      ++pnvec_[nid1]->degree;
      pnvec_[nid2]->nbvec.push_back(nid1);
      ++pnvec_[nid2]->degree;
    }
  }

  void graph::generate_walker(bool isWalkerList) {
    printf("Generating walkers....\n");
    int walkerid = 0;
    for (int i = 0; i < nnode_; i++)
      for (int j = 0; j < pnvec_[i]->degree; j++) {
        WalkerPtr ptr_w(new walker(walkerid, i));
        if (isWalkerList) {
          pwlist_.push_back(ptr_w);
        } else pwvec_.push_back(ptr_w);
        pnvec_[i]->walkervec.push_back(walkerid);
        ++walkerid;
      }
    printf("Done generating walkers....\n");
  }
}
