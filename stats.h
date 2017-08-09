#ifndef PROFILE_STATS_H
#define PROFILE_STATS_H

#include <stdint.h>

#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <exception>
#include <cmath>

struct stats_exception: public std::exception
{
public:
    stats_exception (std::string _msg)
    {
        msg = _msg;
    }

    const char *what () const noexcept
    {
        return msg.c_str();
    }

private:
    std::string msg;
};

template<typename data_type>
struct stats_report
{
    uint32_t count;
    data_type sum;
    data_type min;
    data_type max;
    data_type range;
    data_type mode;
    double mean;
    double median;
    double variance;
    double standard_deviation;
};

template <typename data_type, typename iteratable>
data_type mode (const iteratable & sorted_data)
{
    unsigned max_occurrences = 0;
    unsigned entry_occurrences = 0;
    data_type mode_val, last_val = sorted_data[0];

    for (const auto & entry : sorted_data)
    {
        if (entry == last_val)
        {
            ++entry_occurrences;
        }
        else
        {
            if (entry_occurrences > max_occurrences)
            {
                max_occurrences = entry_occurrences;
                mode_val = last_val;
            }
            entry_occurrences = 1;
            last_val = entry;
        }
    }

    return mode_val;
}

template <typename data_type, typename iteratable>
std::pair<data_type, data_type> min_max (const iteratable & data)
{
    if (data.begin() == data.end())
        throw stats_exception("Invalid data points");

    data_type min_val, max_val;
    min_val = max_val = *(data.begin());
    for (const auto & entry : data)
    {
        if (entry > max_val)
            max_val = entry;
        else if (entry < min_val)
            min_val = entry;
    }
    return {min_val, max_val};
}

template <typename data_type, typename iteratable>
double median (const iteratable & data, unsigned size)
{
    if (data.begin() == data.end())
        throw stats_exception("Invalid data points");

    return (data[size/2] + data[ceil(size/2)])/2;
}

template <typename data_type, typename iteratable>
double variance (const iteratable & data, uint32_t count, double mean)
{
    auto loss = [mean](const data_type & entry) {
        return pow(((double)entry - mean), 2);
    };

    double sum = 0;
    for (const data_type & entry : data)
        sum += loss(entry);

    return sum/count;
}

template <typename data_type, typename iteratable>
stats_report<data_type> create_stats_report (const iteratable & data, unsigned n)
{
    auto sorted = data;
    std::sort(sorted.begin(), sorted.end());
    auto sum = std::accumulate(sorted.begin(), sorted.end(), 0, std::plus<data_type>());
    auto min = sorted[0];
    auto max = sorted[sorted.size()-1];
    double mean = (double)sum/n;
    double var = variance<data_type>(sorted, n, mean);
    double sd = sqrt(var);

    return stats_report<data_type> {
        n,
        sum,
        min,
        max,
        max - min,
        mode<data_type>(sorted),
        mean,
        median<data_type>(sorted, n),
        var,
        sd
    };
}

template<typename data_type>
void print_stats_report (const stats_report<data_type> & report, std::ostream & stream)
{
    stream << "Stats report:\n"
           << "\tCount: " << report.count << "\n"
           << "\tSum: " << report.sum << "\n"
           << "\tMin: " << report.min << "\n"
           << "\tMax: " << report.max << "\n"
           << "\tRange: " << report.range << "\n"
           << "\tMean: " << report.mean << "\n"
           << "\tMedian: " << report.median << "\n"
           << "\tMode: " << report.mode << "\n"
           << "\tVariance: " << report.variance << "\n"
           << "\tSD: " << report.standard_deviation << "\n";
}

#endif
