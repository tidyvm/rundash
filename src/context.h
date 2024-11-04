#ifndef LAMINAR_CONTEXT_H_
#define LAMINAR_CONTEXT_H_

#include <fnmatch.h>
#include <string>
#include <set>
class Run;

// FNM_EXTMATCH isn't supported under musl
#if !defined(FNM_EXTMATCH)
#define FNM_EXTMATCH 0
#endif

// Represents a context within which a Run will be executed. Allows applying
// a certain environment to a set of Jobs, or setting a limit on the number
// of parallel Runs
class Context {
public:
    Context() {}

    std::string name;
    int numExecutors;
    int busyExecutors = 0;

    bool canQueue(std::set<std::string>& patterns) {
        if(busyExecutors >= numExecutors)
            return false;

        for(std::string pattern : patterns) {
            if(fnmatch(pattern.c_str(), name.c_str(), FNM_EXTMATCH) == 0)
                return true;
        }
        return false;
    }
};


#endif // LAMINAR_CONTEXT_H_
