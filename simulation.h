#ifndef SIMULATION_H
#define	SIMULATION_H

#include <cstdio>

namespace nodewalker{
    struct walkermsg{
        int orgnode;
        int curnode;
        int prevnode;
        walkermsg(int orgn, int curn, int prevn) : orgnode(orgn), curnode(curn), prevnode(prevn){}
        walkermsg() : orgnode(0), curnode(0), prevnode(0){}
    };
    
    class simulation{
    public:
        //expected input parameter:
        //1. node number
        //2. probability 0=<p<=1
        //3. run time
        //4. seed
        simulation(int nodenum, double probability, int, int, int);
        void process_mem_usage(double& vm_usage, double& resident_set);
        void startImpl2(const std::string& filename);
        void startImpl1(const std::string& filename);
    private:
        int num_;
        const double p_;
        const int time_;
        //const int th_;
        const int seed_;
        const int ncluster_;
    };
}

#endif	/* SIMULATION_H */

