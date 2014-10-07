#ifndef RANDOM_H
#define	RANDOM_H

namespace nodewalker {

    class random {
    private:
        static int seed_;
        static const int PREC = 1000;
        random();
    public:
        static int get_random_number(int low, int high);
        static void set_seed(int seed);
        static bool get_probability(double p);
    };
}

#endif	/* RANDOM_H */

