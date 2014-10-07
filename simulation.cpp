#include <string>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <ios>
#include <iostream>
#include <fstream>
#include <sstream>
#include "boost/scoped_array.hpp"
#include "boost/scoped_ptr.hpp"
#include "mpi.h"
#include "simulation.h"
#include "random.h"
#include "graph.h"
#include "partialgraph.h"

namespace nodewalker {

    simulation::simulation(int num, double p, int time, int seed, int ncluster)
    : num_(num), p_(p), time_(time), seed_(seed), ncluster_(ncluster) {
        random::set_seed(seed);
    }


    //////////////////////////////////////////////////////////////////////////////
    //
    // process_mem_usage(double &, double &) - takes two doubles by reference,
    // attempts to read the system-dependent data for a process' virtual memory
    // size and resident set size, and return the results in KB.
    //
    // On failure, returns 0.0, 0.0

    void simulation::process_mem_usage(double& vm_usage, double& resident_set) {
        using std::ios_base;
        using std::ifstream;
        using std::string;
    
        vm_usage = 0.0;
        resident_set = 0.0;
    
        // 'file' stat seems to give the most reliable results
        //
        ifstream stat_stream("/proc/self/stat", ios_base::in);
    
        // dummy vars for leading entries in stat that we don't care about
        //
        string pid, comm, state, ppid, pgrp, session, tty_nr;
        string tpgid, flags, minflt, cminflt, majflt, cmajflt;
        string utime, stime, cutime, cstime, priority, nice;
        string O, itrealvalue, starttime;
    
        // the two fields we want
        //
        unsigned long vsize;
        long rss;
    
        stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
                >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
                >> utime >> stime >> cutime >> cstime >> priority >> nice
                >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest
    
        stat_stream.close();
    
        long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
        vm_usage = vsize / 1024.0;
        resident_set = rss * page_size_kb;
    }

    void simulation::startImpl1(const std::string& filename) {
        // begin MPI routine
        MPI::Init();
        MPI::Intracomm comm = MPI::COMM_WORLD;
        int numprocs = comm.Get_size();
        int rank = comm.Get_rank();

        random::set_seed(seed_);
        std::string partialgraphPrefix = "partialGraph";
        if(rank==0){
            graph g(num_, ncluster_, false);
            printf("Dividing graph...\n");
            partialgraph::divide_graph(g, 1, partialgraphPrefix);
            printf("Done dividing graph...\n");
        }

        printf("Processor %d reading graph...\n", rank);
        
        // read in sequence to avoid errors
        boost::scoped_ptr<partialgraph> ptr_g;
        for(int i=0;i<numprocs;++i){
            comm.Barrier();
            if(rank == i){
                ptr_g.reset(new partialgraph(partialgraphPrefix, 0, false));
            }
        }
        printf("Processor %d done reading graph...\n", rank);
        std::vector<WalkerPtr> pwvec = ptr_g->get_walker_vec();
        const int walkerperproc = pwvec.size() / numprocs;
        if (rank == 0) printf("Totally %d walkers\n", pwvec.size());
        int lowerid = rank * walkerperproc;
        int upperid = (rank + 1) * walkerperproc - 1 > pwvec.size() ? pwvec.size() : ((rank + 1) * walkerperproc - 1);
        for (int i = 0; i < numprocs; ++i) {
            comm.Barrier();
            if (rank == i){
                printf("processor %d load %d walkers\n", rank, pwvec.size());
                printf("processor %d deal with walker from %d to %d\n", rank, lowerid, upperid);
            }
        }

        clock_t start_clock, stop_clock;
        if (rank == 0) {
            start_clock = clock();
            printf("Walkers start walking around...\n");
        }

        //#pragma omp parallel for
        for (int i = lowerid; i <= upperid; ++i) {
            if (random::get_probability(p_))
                pwvec[i]->jump_back();
            else
                pwvec[i]->random_walk(*(ptr_g.get()));
        }

        comm.Barrier();

        if (rank == 0) {
            double resident_set, vm_usage;
            process_mem_usage(resident_set, vm_usage);
            printf("Memory usage: resident_set: %.2fMB, vm_usage: %.2fMB\n", resident_set/1024.0, vm_usage/1024.0);
            stop_clock = clock();
            printf("All walkers stopped...\n");
            std::cout << "Running time is " << (double) (stop_clock - start_clock) / (double)CLOCKS_PER_SEC << " seconds" << std::endl;
        }

        if (rank == 0) printf("Start recording...\n");

        for (int i = 0; i < numprocs; ++i) {
            if (rank == i) {
                FILE *fp;
                if (rank == 0)
                    fp = fopen(filename.c_str(), "w");
                else
                    fp = fopen(filename.c_str(), "a");

                for (int i=lowerid; i<=upperid; ++i) {
                    fprintf(fp, "%d %d\n", pwvec[i]->get_orgnode(), pwvec[i]->get_curnode());
                }

                fclose(fp);
            }
            comm.Barrier();
        }

        if (rank == 0) printf("Stop recording...\n");

        if (rank == 0) {
            printf("All done...\n");
            printf("Result saved to %s\n", filename.c_str());
        }

        MPI::Finalize();
    }

    void simulation::startImpl2(const std::string& filename) {
        // begin MPI routine
        MPI::Init();
        MPI::Intracomm comm = MPI::COMM_WORLD;
        int numprocs = comm.Get_size();
        int rank = comm.Get_rank();
        int tag = 0;

        random::set_seed(seed_);

        //graph g("idols-db_id.txt",num_);
        std::string partialgraphPrefix = "partialGraph";
        int totalwalker = 0;
        if (rank == 0) {
            graph g(num_, ncluster_);
            totalwalker = g.get_walker().size();
            printf("Totally %d walkers\n", totalwalker);
            printf("Dividing graph...\n");
            partialgraph::divide_graph(g, numprocs, partialgraphPrefix.c_str());
            printf("Done dividing graph...\n");
        }
        comm.Barrier();
        if (rank == 0)
            printf("total processor number: %d\n", numprocs);
        printf("Processor %d reading graph...\n", rank);
        partialgraph pg(partialgraphPrefix.c_str(), rank);
        printf("Processor %d done reading graph...\n", rank);

        const std::vector<NodePtr> pnvec = pg.get_node();
        std::list<WalkerPtr> pwlist = pg.get_walker();
        printf("Rank %d: list size: %d\n", rank, pwlist.size());
        printf("Rank %d: vec size: %d\n", rank, pnvec.size());

        //num_ = g.get_node_num();
        printf("Rank %d: Node number = %d\n", rank, pg.get_node_num());

        //simulation start

        // compute the start index and end index of walkers for current processor.
        std::vector<walkermsg> outvec; // for slave process

        clock_t start_clock, stop_clock;
        clock_t comm_start_clock, comm_total_clock = 0;
        std::vector< std::vector<walkermsg> > outmsgvecs; // for master process
        if (rank == 0) {
            // init send out walkermsg vecs
            for (int i = 1; i < numprocs; ++i) {
                std::vector<walkermsg> wv;
                outmsgvecs.push_back(wv);
            }
            start_clock = clock();
            printf("Walkers start walking around...\n");
        }
        boost::scoped_array<walkermsg> recvbuf;
        for (int i = 0; i < time_; ++i) {
            if (rank == 0) {
                printf("***********************\n");
                printf("*******Round %d********\n", i + 1);
            }
            for (std::list<WalkerPtr>::iterator iter = pwlist.begin(); iter != pwlist.end();) if (!((*iter)->isdead())) {
                if (random::get_probability(p_))
                    (*iter)->jump_back();
                else
                    (*iter)->random_walk(pg);

                // save walker if it goes out of the graph and remove it from the list
                if ((*iter)->get_curnode() < pg.get_lownode() || (*iter)->get_curnode() > pg.get_highnode()) {
                    walkermsg wm((*iter)->get_orgnode(), (*iter)->get_curnode(), (*iter)->get_prevnode());
                    outvec.push_back(wm);
                    pwlist.erase(iter++);
                } else ++iter;
            }

            // send / receive
            if (rank == 0) {
                comm_start_clock = clock();
                MPI::Status s;
                // process out-of-graph walkers on master process first
                for (int i = 0; i < outvec.size(); ++i) {
                    int proctogo = partialgraph::get_node_rank(outvec[i].curnode);
                    outmsgvecs[proctogo - 1].push_back(outvec[i]); //outmsgvecs is 0-indexed
                }

                // master process, receive out-of-graph walkers from other processors
                printf("Waiting to receive walkers...\n");
                for (int i = 1; i < numprocs; ++i) {
                    comm.Probe(MPI_ANY_SOURCE, tag, s);
                    int count = s.Get_count(MPI_BYTE) / sizeof (walkermsg);
                    recvbuf.reset(new walkermsg[count + 1]);
                    comm.Recv(recvbuf.get(), s.Get_count(MPI_BYTE), MPI_BYTE, s.Get_source(), tag);
                    if (s.Get_count(MPI_BYTE) == 1) {
                        //empty
                        continue;
                        printf("Received empty...\n");
                    }

                    // organize walkers for sending
                    printf("Organize received walkers from process %d...\n", s.Get_source());
                    for (int j = 0; j < count; ++j) {
                        int proctogo = partialgraph::get_node_rank(recvbuf[j].curnode);
                        if (proctogo == 0) {
                            // for self, simply keep it
                            WalkerPtr w(new walker(recvbuf[j].orgnode, recvbuf[j].curnode, recvbuf[j].prevnode));
                            pwlist.push_back(w);
                        } else outmsgvecs[proctogo - 1].push_back(recvbuf[j]); //outmsgvecs is 0-indexed
                    }
                    printf("Done organize received walkers...\n");
                }

                printf("Done receive walkers, begin sending walkers out...\n");
                // send walkers to its processor
                for (int i = 1; i < numprocs; ++i) {
                    if (outmsgvecs[i - 1].empty()) {
                        // sending one byte out if empty
                        char e = 0;
                        comm.Send(&e, 1, MPI_BYTE, i, tag);
                        printf("Sending empty out...\n");
                    } else comm.Send(outmsgvecs[i - 1].data(), outmsgvecs[i - 1].size() * sizeof (walkermsg), MPI_BYTE, i, tag);
                }
                printf("Done sending walkers...\n");

                // clean up
                for (int i = 0; i < outmsgvecs.size(); ++i) outmsgvecs[i].clear();
            } else {
                // send out-of-graph walkers out to process 0
                printf("Processor %d sending walkers out...\n", rank);
                if (outvec.empty()) {
                    // sending one byte out if empty
                    char e = 0;
                    comm.Send(&e, 1, MPI_BYTE, 0, tag);
                    printf("Processor %d sending empty out...\n", rank);
                } else comm.Send(outvec.data(), outvec.size() * sizeof (walkermsg), MPI_BYTE, 0, tag);
                printf("Processor %d done sending...\n", rank);

                // receive walker 
                printf("Processor %d waiting to receive walkers...\n", rank);
                MPI::Status s;
                printf("Processor %d probing...\n", rank);
                comm.Probe(0, tag, s);
                printf("Processor %d probing successfully...\n", rank);
                int count = s.Get_count(MPI_BYTE) / sizeof (walkermsg);
                recvbuf.reset(new walkermsg[count + 1]);
                comm.Recv(recvbuf.get(), s.Get_count(MPI_BYTE), MPI_BYTE, 0, tag);
                printf("Processor %d done receive walkers...\n", rank);

                // insert received walkers to list
                printf("Processor %d process received walkers...\n", rank);
                if (s.Get_count(MPI_BYTE) != 1) { //noneempty
                    for (int i = 0; i < count; ++i) {
                        WalkerPtr w(new walker(recvbuf[i].orgnode, recvbuf[i].curnode, recvbuf[i].prevnode));
                        pwlist.push_back(w);
                    }
                } else printf("Processor %d received empty...\n", rank);
                printf("Processor %d done process received walkers...\n", rank);
            }

            // clean up
            outvec.clear();

            comm.Barrier();
            comm_total_clock += clock() - comm_start_clock;
        }

        comm.Barrier();

        if (rank == 0) {
            double resident_set, vm_usage;
            process_mem_usage(resident_set, vm_usage);
            printf("Master process memory usage: resident_set: %.2fMB, vm_usage: %.2fMB\n", resident_set/1024.0, vm_usage/1024.0);
            stop_clock = clock();
            printf("Walkers stopped...\n");
            std::cout << "Running time is " << (double) (stop_clock - start_clock) / (double)CLOCKS_PER_SEC << " seconds" << std::endl;
            std::cout << "Communication time is " << (float) (comm_total_clock) / CLOCKS_PER_SEC << " seconds" << std::endl;
        }
        if (rank == 1){
            double resident_set, vm_usage;
            process_mem_usage(resident_set, vm_usage);
            printf("Slave process memory usage: resident_set: %.2fMB, vm_usage: %.2fMB\n", resident_set/1024.0, vm_usage/1024.0);
        }

        if (rank == 0) printf("Start recording...\n");

        for (int i = 0; i < numprocs; ++i) {
            if (rank == i) {
                FILE *fp;
                if (rank == 0)
                    fp = fopen(filename.c_str(), "w");
                else
                    fp = fopen(filename.c_str(), "a");
                for (std::list<WalkerPtr>::iterator iter = pwlist.begin(); iter != pwlist.end(); ++iter) {
                    fprintf(fp, "%d %d\n", (*iter)->get_orgnode(), (*iter)->get_curnode());
                }
                fclose(fp);
            }
            comm.Barrier();
        }

        if (rank == 0) printf("Stop recording...\n");

        if (rank == 0) {
            printf("All done...\n");
            printf("Result saved to %s\n", filename.c_str());
        }

        MPI::Finalize();
    }
}
