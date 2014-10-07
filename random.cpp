#include <cstdlib>
#include "random.h"
#include "error.h"

namespace nodewalker {
    int random::seed_;

    void random::set_seed(const int seed) {
        random::seed_ = seed;
        srand(seed);
    }

    int random::get_random_number(const int low, const int high) {
        if (low > high) throw error("Error: low bound of range is larger than high bound!");
        if (low == high) return low;
        //srand(random::seed_); //set srand() in case that someone called srand() elsewhere.
        return rand() % (high - low + 1) + low;
    }

    bool random::get_probability(double p) {
        if (p > 1 || p<0) throw error("Error: p cannot be greater than 1 or negative!");
        //srand(random::seed_); //set srand() in case that someone called srand()
        return (rand() % random::PREC + 1) <= p * random::PREC;
    }
}
