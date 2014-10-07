#include <string>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include "mpi.h"
#include "simulation.h"

using namespace nodewalker;

int main(int argc, char** argv) {
    std::string filename = "nwdata.txt";
    int runtime = 1;
    int nodenum, implID;

    // arg: nodenum implementationID
    if (argc != 3) {
        printf("Please use following format:\n nwsim [nodenum] [implID]\n");
        return 1;
    } else {
        nodenum = atoi(argv[1]);
        implID = atoi(argv[2]);
        if (nodenum <= 0) {
            printf("Node number is invalid!\n");
            return 1;
        }
        if (implID < 0 || implID > 1) {
            printf("Implementation ID should be 0 or 1!\n");
            return 1;
        }
    }

    for (int i = 1; i <= runtime; ++i) {
        printf("Totally %d nodes....\n", nodenum);
        simulation s(nodenum, 0.5, 5, 0, 5);
        if (implID == 0){
            printf("Implementation 0 is running....\n");
            s.startImpl1(filename);
        }
        else{
            printf("Implementation 1 is running....\n");
            s.startImpl2(filename);
        }
    }

    return 0;
}
