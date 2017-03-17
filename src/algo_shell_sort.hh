#ifndef HEADER_ALGO_SHELL_SORT_HH
#define HEADER_ALGO_SHELL_SORT_HH

#include <algorithm>
#include "util.hh"

// https://en.wikipedia.org/wiki/Shellsort
struct ShellSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "ShellSort";
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
    static inline const char* getStability() {
        return "No";
    }
    static const int NAIVE_INSERT_LIMIT = 8;

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        TIndex delta = size / 2;

        while (delta > 0) {
            if ((delta /= 2.25) < NAIVE_INSERT_LIMIT) {
                naiveInsertionSort<TValue>(elemAt, 0, size);
                return;
            }

            for (TIndex i = delta; i < size; i++) {
                TValue posElem = elemAt(i);
                TIndex insertPos;
                for (insertPos = i - delta; insertPos >= 0 && posElem < elemAt(insertPos); insertPos -= delta) {
                    elemAt(insertPos + delta) = elemAt(insertPos);
                }
                elemAt(insertPos + delta) = posElem;
            }
        }
    }
};
#endif
