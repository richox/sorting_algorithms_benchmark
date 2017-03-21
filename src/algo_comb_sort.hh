#ifndef HEADER_ALGO_COMB_SORT_HH
#define HEADER_ALGO_COMB_SORT_HH

#include <algorithm>
#include "util.hh"

// https://en.wikipedia.org/wiki/Comb_sort
struct CombSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "CombSort";
    }
    static inline const char* getAlgorithmBestTimeComplex() {
        return "O(nlogn)";
    }
    static inline const char* getAlgorithmAvgTimeComplex() {
        return "O(n^?)";
    }
    static inline const char* getAlgorithmWorstTimeComplex() {
        return "O(n^2)";
    }
    static inline const char* getAlgorithmSpaceComplex() {
        return "O(1)";
    }
    static const int NAIVE_INSERT_LIMIT = 16;

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        TIndex delta = size / 2;

        while (true) {
            if ((delta /= 1.3) < 48) {
                naiveInsertionSort<TValue>(elemAt, 0, size);
                return;
            }
            for (TIndex i = 0; i + delta < size; i++) {
                if (elemAt(i) > elemAt(i + delta)) {
                    std::swap(elemAt(i), elemAt(i + delta));
                }
            }
        }
    }
};
#endif
