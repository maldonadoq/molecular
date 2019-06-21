#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <fstream>
#include <unistd.h>
#include <iostream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// > process_mem_usage(double &, double &) - takes two doubles by reference, attempts to read the system-dependent
//   data for a process' virtual memory size and resident set size, and return the results in KB.
// > on failure, returns 0.0, 0.0
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void process_mem_usage(double& vm_usage, double& resident_set){
    vm_usage     = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

#endif