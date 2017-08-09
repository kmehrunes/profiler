#ifndef ACC_PROF_H
#define ACC_PROF_H

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "time-helpers.h"
#include "stats.h"

// single run measures
template <uint64_t (*T)(uint64_t) = nanosecs>
uint64_t single_cost (void(*proc)())
{
    PROFILE_START;
    proc();
    return T(PROFILE_END);
}

template <uint64_t (*T)(uint64_t) = nanosecs, typename LT>
uint64_t single_cost (LT lambda)
{
    PROFILE_START;
    lambda();
    return T(PROFILE_END);
}

// multiple runs accumelators
template <uint64_t (*T)(uint64_t) = nanosecs>
uint64_t accumelated_cost (void(*proc)(), unsigned n)
{
    uint64_t cost = 0;
    for (unsigned i = 0; i < n; i++)
        cost += single_cost<T>(proc);
    return cost;
}

template <uint64_t (*T)(uint64_t) = nanosecs, typename LT>
uint64_t accumulated_cost (LT lambda, unsigned n)
{
    uint64_t cost = 0;
    for (unsigned i = 0; i < n; i++)
        cost += single_cost<T, LT>(lambda);
    return cost;
}

// multiple runs reporters
template <uint64_t (*T)(uint64_t) = nanosecs>
std::vector<uint64_t> multiple_costs (void(*proc)(), unsigned n)
{
    std::vector<uint64_t> costs(n);
    for (unsigned i = 0; i < n; i++)
        costs[i] = single_cost<T>(proc);
    return costs;
}

template <uint64_t (*T)(uint64_t) = nanosecs, typename LT>
std::vector<uint64_t> multiple_costs (LT lambda, unsigned n)
{
    std::vector<uint64_t> costs(n);
    for (unsigned i = 0; i < n; i++)
        costs[i] = single_cost<T>(lambda);
    return costs;
}

template <uint64_t (*T)(uint64_t) = nanosecs>
stats_report<uint64_t> get_cost_report (void(*proc)(), unsigned n)
{
    return create_stats_report<uint64_t>(multiple_costs(proc, n), n);
}

template <uint64_t (*T)(uint64_t) = nanosecs, typename LT>
stats_report<uint64_t> get_cost_report (LT lambda, unsigned n)
{
    return create_stats_report<uint64_t>(multiple_costs(lambda, n), n);
}

#endif
