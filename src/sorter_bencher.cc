
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <random>
#include <vector>
#include <stdexcept>

#include "algo_library_sort.hh"
#include "algo_quick_sort.hh"
#include "algo_heap_sort.hh"
#include "algo_comb_sort.hh"
#include "algo_shell_sort.hh"
#include "algo_merge_sort.hh"
#include "algo_smooth_sort.hh"
#include "algo_inplace_merge_sort.hh"
#include "sorter.hh"

struct SorterBencher {
    std::vector<long double> randomTestDataset;
    std::vector<long double> randomTestDatasetSorted;

    SorterBencher(int benchSize) {
        std::generate_n(std::back_inserter(randomTestDataset), benchSize,
                std::bind(std::uniform_real_distribution<long double>(0.0, 1.0), std::default_random_engine()));
        randomTestDatasetSorted = randomTestDataset;
        std::sort(randomTestDatasetSorted.begin(), randomTestDatasetSorted.end());
    }

    template<typename Sorter> void bench(Sorter sorter) {
        auto dataset = randomTestDataset;
        auto t1 = std::clock();
        {
            sorter.template sort<std::vector<long double>::iterator, long double, int>(
                    dataset.begin(),
                    dataset.end());
        }
        auto t2 = std::clock();
        auto timeCostMillis = (t2 - t1) * 1000 / CLOCKS_PER_SEC;

        if (dataset != randomTestDatasetSorted) {
            fprintf(stderr, "%s: bad sorting result\n", sorter.getAlgorithmName());
            throw std::runtime_error("bad sorting result");
        }
        fprintf(stderr, "%-16s >>>> best: %-8s | avg: %-8s | worst: %-8s | space: %-8s >>>> cost time: %ldms\n",
                sorter.getAlgorithmName(),
                sorter.getAlgorithmBestTimeComplex(),
                sorter.getAlgorithmAvgTimeComplex(),
                sorter.getAlgorithmWorstTimeComplex(),
                sorter.getAlgorithmSpaceComplex(),
                timeCostMillis);
    }
};

int main() {
    SorterBencher bencher(1000000);
    bencher.bench(Sorter<QuickSortAlgorithm>());
    bencher.bench(Sorter<MergeSortAlgorithm>());
    bencher.bench(Sorter<HeapSortAlgorithm>());
    bencher.bench(Sorter<ShellSortAlgorithm>());
    bencher.bench(Sorter<CombSortAlgorithm>());
    bencher.bench(Sorter<SmoothSortAlgorithm>());
    bencher.bench(Sorter<InplaceMergeSortAlgorithm>());
    bencher.bench(Sorter<LibrarySortAlgorithm>());
    return 0;
}
