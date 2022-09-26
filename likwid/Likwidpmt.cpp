#include "Likwidpmt.h"

#include <iostream>

#include <unistd.h>
#include <omp.h>

#if defined(HAVE_LIKWID)
#include <likwid.h>

// Events to measure
// Note: change code to read counters for all cores
//       when measuring something else than power
#define NR_EVENTS 2
#define EVENTSET "PWR_PKG_ENERGY:PWR0,PWR_DRAM_ENERGY:PWR3"

#endif

namespace pmt {
namespace likwid {

class Likwidpmt_ : public Likwidpmt {
    public:
        Likwidpmt_();
        ~Likwidpmt_();

	private:
        virtual State measure() override;

        virtual const char* getDumpFileName() override {
            return "/tmp/likwidpmt.out";
        }

        virtual int getDumpInterval() override {
            return 100; // milliseconds
        }

    private:
        int nr_groups;
        int nr_threads_group;
        double joulesTotal = 0;
};

Likwidpmt* Likwidpmt::create()
{
    return new Likwidpmt_();
}

Likwidpmt_::Likwidpmt_()
{
    #if defined(HAVE_LIKWID)
    // Load the topology module
    if (topology_init() < 0) {
        std::cerr << "Failed to initialize LIKWID's topology module" << std::endl;
        exit(EXIT_FAILURE);
    }

    // CpuInfo_t contains global information like name, CPU family, ...
    CpuInfo_t info = get_cpuInfo();

    // CpuTopology_t contains information about the topology of the CPUs
    CpuTopology_t topo = get_cpuTopology();

    // Get number of sockets
    int nr_sockets = topo->numSockets;

    // Get number of hardware threads
    int nr_threads = topo->numHWThreads;

    // Number of threads per socket
    int nr_threads_socket = nr_threads / nr_sockets;

    // Measure only first cpu in socket
    nr_threads_socket = 1;

    // Initialize performance monitoring per socket
    nr_groups = nr_sockets;
    nr_threads_group = nr_threads_socket;
    for (int socket = 0; socket < nr_sockets; socket++) {
        // Fill cpu array with acpi id's of threads
        int cpus[nr_threads_socket];
        int count = 0;
        for (int i = 0; i < nr_threads_socket; i++) {
            if (socket == topo->threadPool[i].packageId) {
                cpus[count++] = topo->threadPool[i].apicId;
            }
        }

        // Initialize the perfmon module
        if (perfmon_init(nr_threads_socket, cpus) < 0) {
            std::cerr << "Failed to initialize LIKWID's performance monitoring module" << std::endl;
            topology_finalize();
            exit(EXIT_FAILURE);
        }

        // Add eventset string to the perfmon module
        // Assumption: groupId is equal to socket number
        int groupId = perfmon_addEventSet((char*) EVENTSET);
        if (groupId < 0) {
            std::cerr << "Failed to add event string " << EVENTSET << " to LIKWID's performance monitoring module" << std::endl;
            perfmon_finalize();
            topology_finalize();
            exit(EXIT_FAILURE);
        }

        // Setup the eventset identified by group ID (groupId)
        if (perfmon_setupCounters(groupId) < 0) {
            std::cerr << "Failed to setup group " << groupId << " in LIKWID's performance monitoring module" << std::endl;
            perfmon_finalize();
            topology_finalize();
            exit(EXIT_FAILURE);
        }

        // Start all counters in the previously set up event set
        if (perfmon_startCounters() < 0) {
            std::cerr << "Failed to start counters for group " << groupId << std::endl;
            perfmon_finalize();
            topology_finalize();
            exit(EXIT_FAILURE);
        }
    } // end for socket
    #endif
} // end constructor

Likwidpmt_::~Likwidpmt_() {
    #if defined(HAVE_LIKWID)
    stopDumpThread();
    perfmon_stopCounters();
    perfmon_finalize();
    topology_finalize();
    #endif
} // end destructor

State Likwidpmt_::measure() {
   #if defined(HAVE_LIKWID)
   for (int groupId = 0; groupId < nr_groups; groupId++) {
        // Read performance counters
        if (perfmon_readGroupCounters(groupId) != 0) {
            std::cerr << "perfmon_readCounters() fails" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Accumulate results
        // Only for first thread in group
        int threadId = 0;
        //for (int threadId = 0; threadId < nr_threads_group; threadId++) {
            for (int eventId = 0; eventId < NR_EVENTS; eventId++) {
                double result = perfmon_getLastResult(groupId, eventId, threadId);
                //state.joulesAtRead += result;
                joulesTotal += result;
            }
        //}
    }
    #endif

    State state;
    state.timeAtRead = get_wtime();
    state.joulesAtRead = joulesTotal;

    return state;
}

} // end namespace likwid
} // end namespace pmt
