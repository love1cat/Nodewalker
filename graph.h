#ifndef GRAPH_H
#define	GRAPH_H

#include <string>
#include <map>
#include <vector>
#include <list>
//#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/unordered_map.hpp>
#include "node.h"
#include "walker.h"

namespace nodewalker {

  struct cluster {
    int headid;
    int lowid;
    int highid;
  };

  //typedef boost::shared_ptr<cluster> ClusterPtr;
  typedef boost::shared_array<cluster> ClusterArray;

  class graph {
    public:
      graph(int, int, bool isWalkerList=true);
      graph(const char *inputfile, const int);

      inline const std::vector<NodePtr>& get_node() const {
        return pnvec_;
      };

      inline std::list<WalkerPtr>& get_walker() {
        return pwlist_;
      };

      inline std::vector<WalkerPtr>& get_walker_vec(){
        return pwvec_;
      }

      inline const ClusterArray get_cluster() const {
        return pch_;
      }

      inline const int get_node_num() const {
        return nnode_;
      }
    protected:
      graph(){}
      int nnode_;
      int ncluster_;
      std::vector<NodePtr> pnvec_;
      std::list<WalkerPtr> pwlist_;
      std::vector<WalkerPtr> pwvec_;
      ClusterArray pch_;
    private:
      int getID(std::string curp, bool issearch);
      void generate_walker(bool isWalkerList);
      void connect(int, int);
      typedef boost::unordered_map<std::string, int> IDMap_t;
      IDMap_t idmap_;
      std::string inputfile_;
      int maxnode_;
  };
}

#endif	/* GRAPH_H */

