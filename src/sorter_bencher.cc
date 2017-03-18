
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <vector>
#include <random>
#include <stdexcept>

#include "algo_library_sort.hh"
#include "algo_quick_sort.hh"
#include "algo_heap_sort.hh"
#include "algo_comb_sort.hh"
#include "algo_shell_sort.hh"
#include "algo_merge_sort.hh"
#include "algo_smooth_sort.hh"
#include "algo_inplace_merge_sort.hh"
#include "algo_multiheap_sort.hh"
#include "sorter.hh"

struct SorterBencher {
    using TValue = std::pair<long double, long double>;
    using TIndex = int;

    std::vector<TValue> randomTestDataset;
    std::vector<TValue> randomTestDatasetSorted;

    SorterBencher(int benchSize) {
        auto randomDist = std::bind(std::uniform_real_distribution<long double>(), std::default_random_engine());
        auto randomValueGenerator = [&]() {
            return std::make_pair(randomDist(), randomDist());
        };
        std::generate_n(std::back_inserter(randomTestDataset), benchSize, randomValueGenerator);
        randomTestDatasetSorted = randomTestDataset;
        std::sort(randomTestDatasetSorted.begin(), randomTestDatasetSorted.end());
        printBenchHeader();
    }

    template<typename Sorter> void bench(Sorter sorter) {
        auto dataset = randomTestDataset;
        auto t1 = std::clock();
        {
            sorter.template sort<std::vector<TValue>::iterator, TValue, TIndex>(dataset.begin(), dataset.end());
        }
        auto t2 = std::clock();
        auto timeCostMillis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

        if (dataset != randomTestDatasetSorted) {
            fprintf(stderr, "%s: bad sorting result\n", sorter.getAlgorithmName());
        }
        printBenchRow(
                sorter.getAlgorithmName(),
                sorter.getAlgorithmBestTimeComplex(),
                sorter.getAlgorithmAvgTimeComplex(),
                sorter.getAlgorithmWorstTimeComplex(),
                sorter.getAlgorithmSpaceComplex(),
                sorter.getStability(),
                timeCostMillis);
    }

    void printBenchHeader() {
        fprintf(stdout, "| %-16s | %-10s | %-10s | %-10s | %-5s | %-6s | %-9s |\n",
                "algorithm",
                "best time",
                "avg time",
                "worst time",
                "space",
                "stable",
                "time cost");
        fprintf(stdout, "|-|-|-|-|-|-|-|\n");
    }

    void printBenchRow(
            const std::string& algorithmName,
            const std::string& bestTime,
            const std::string& avgTime,
            const std::string& worstTime,
            const std::string& space,
            const std::string& stability,
            long timeCost) {

        fprintf(stdout, "| %-16s | %-10s | %-10s | %-10s | %-5s | %-6s | %7ldms |\n",
                algorithmName.data(),
                bestTime.data(),
                avgTime.data(),
                worstTime.data(),
                space.data(),
                stability.data(),
                timeCost);
    }
};

int main(int argc, char** argv) {
    int benchSize = 1000000;  // as default
    if (argc == 2) {
        benchSize = std::atoi(argv[1]);
    }

    SorterBencher bencher(benchSize);
    bencher.bench(Sorter<QuickSortAlgorithm>());
    bencher.bench(Sorter<MergeSortAlgorithm>());
    bencher.bench(Sorter<HeapSortAlgorithm>());
    bencher.bench(Sorter<ShellSortAlgorithm>());
    bencher.bench(Sorter<CombSortAlgorithm>());
    bencher.bench(Sorter<InplaceMergeSortAlgorithm>());
    bencher.bench(Sorter<SmoothSortAlgorithm>());
    bencher.bench(Sorter<MultiheapSortAlgorithm>());
    bencher.bench(Sorter<LibrarySortAlgorithm>());
    return 0;
}
