#ifndef RAPL_READ_H_
#define RAPL_READ_H_

class Rapl {
    public:
        Rapl();
        double measure();

    private:
        static const int MAX_CPUS         = 1024;
        static const int MAX_PACKAGES     = 16;
        static const int NUM_RAPL_DOMAINS = 4;

        void detect_cpu();
        void detect_packages();

        bool rapl_supported = true;
        int cpu_model       = 0;
        int total_cores     = 0;
        int total_packages  = 0;
        int package_map[MAX_PACKAGES];
};

#endif
