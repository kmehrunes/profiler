#include "profilers.h"

#include <thread>
#include <chrono>

using namespace std;

void stats()
{
    vector<int> data = {1, 2, 2, 3, 4, 5, 6, 7};
    stats_report<int> report = create_stats_report<int>(data, data.size());
    print_stats_report(report, cout);
}

void manual_profile()
{
    PROFILE_START;
    this_thread::sleep_for(chrono::milliseconds(1000));
    cout << PROFILE_END << '\n';
}

void single_cost_example()
{
    cout << single_cost([]() {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }) << '\n';
}

void accumelated_cost_example()
{
    cout << accumelated_cost([]() {
        this_thread::sleep_for(chrono::milliseconds(100));
    }, 10) << '\n';
}

void multiple_costs_example()
{
    vector<uint64_t> costs = multiple_costs([]() {
        this_thread::sleep_for(chrono::milliseconds(100));
    }, 10);
    cout << "Costs:\n";
    for (uint64_t cost : costs)
        cout << '\t' << cost << '\n';
    cout << '\n';
}

int main(int argc, char const *argv[]) {
    stats();
    manual_profile();
    single_cost_example();
    accumelated_cost_example();
    multiple_costs_example();
    return 0;
}
