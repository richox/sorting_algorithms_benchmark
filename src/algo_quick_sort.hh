#ifndef HEADER_ALGO_QUICK_SORT_HH
#define HEADER_ALGO_QUICK_SORT_HH

#include <algorithm>
#include "util.hh"

// https://en.wikipedia.org/wiki/Quicksort
struct QuickSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "QuickSort";
    }
    static inline const char* getAlgorithmBestTimeComplex() {
        return "O(nlogn)";
    }
    static inline const char* getAlgorithmAvgTimeComplex() {
        return "O(nlogn)";
    }
    static inline const char* getAlgorithmWorstTimeComplex() {
        return "O(n^2)";
    }
    static inline const char* getAlgorithmSpaceComplex() {
        return "O(1)";
    }
    static const int NAIVE_INSERT_LIMIT = 32;

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        TIndex stacka[64] = {0};
        TIndex stackb[64] = {size - 1};
        int stackTop = 1;

        while (stackTop > 0) {
            TIndex a = stacka[stackTop - 1];
            TIndex b = stackb[stackTop - 1];
            stackTop -= 1;

            if (b + 1 - a < NAIVE_INSERT_LIMIT) {
                naiveInsertionSort<TValue>(elemAt, a, b + 1);
                continue;
            }
            if (elemAt((a + b) / 2) > elemAt(b)) std::swap(elemAt((a + b) / 2), elemAt(b));
            if (elemAt((a + b) / 2) > elemAt(a)) std::swap(elemAt((a + b) / 2), elemAt(a));
            if (elemAt(a) > elemAt(b)) std::swap(elemAt(a), elemAt(b));

            TIndex l = a + 1;
            TIndex r = b - 1;
            while (l <= r) {
                while (elemAt(l) < elemAt(a)) l++;
                while (elemAt(r) > elemAt(a)) r--;
                if (l <= r) {
                    std::swap(elemAt(l++), elemAt(r--));
                }
            }
            if (a < r) {
                stacka[stackTop] = a;
                stackb[stackTop] = r;
                stackTop += 1;
            }
            if (l < b) {
                stacka[stackTop] = l;
                stackb[stackTop] = b;
                stackTop += 1;
            }
        }
    }
};
#endif
