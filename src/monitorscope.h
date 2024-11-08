#ifndef LAMINAR_MONITORSCOPE_H_
#define LAMINAR_MONITORSCOPE_H_

#include <string>

// Simple struct to define which information a frontend client is interested
// in, both in initial request phase and real-time updates. It corresponds
// loosely to frontend URLs
struct MonitorScope {
    enum Type {
        HOME, // home page: recent builds and statistics
        ALL,  // browse jobs
        JOB,  // a specific job page
        RUN,  // a specific run page
    };

    MonitorScope(Type type = HOME, std::string job = std::string(), uint num = 0) :
        type(type),
        job(job),
        num(num),
        page(0),
        field("number"),
        order_desc(true)
    {}

    // whether this scope wants status information for the specified job
    bool wantsStatus(std::string ajob, uint anum = 0) const {
        if(type == HOME || type == ALL) return true;
        else return ajob == job;
        // we could have checked that the run number matches, but actually the
        // run page needs to know about a non-matching run number in order to
        // know whether to display the "next" arrow.
    }

    Type type;
    std::string job;
    uint num ;
    // sorting
    uint page;
    std::string field;
    bool order_desc;
};

#endif // LAMINAR_MONITORSCOPE_H_

