#ifndef HEADER_ALGO_MERGE_SORT_HH
#define HEADER_ALGO_MERGE_SORT_HH

#include <algorithm>
#include "util.hh"

// https://en.wikipedia.org/wiki/Merge_sort
struct MergeSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "MergeSort";
    }
    static inline const char* getAlgorithmBestTimeComplex() {
        return "O(nlogn)";
    }
    static inline const char* getAlgorithmAvgTimeComplex() {
        return "O(nlogn)";
    }
    static inline const char* getAlgorithmWorstTimeComplex() {
        return "O(nlogn)";
    }
    static inline const char* getAlgorithmSpaceComplex() {
        return "O(n)";
    }
    static inline const char* getStability() {
        return "Yes";
    }
    static const int NAIVE_INSERT_LIMIT = 16;

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        TValue* extraSpace = new TValue[size];
        int round = 0;

        for (TIndex l = 0; l < size; l += NAIVE_INSERT_LIMIT) {
            naiveInsertionSort<TValue>(elemAt, l, std::min(l + NAIVE_INSERT_LIMIT, size));
        }

        for (TIndex stepWidth = NAIVE_INSERT_LIMIT; stepWidth < size; stepWidth *= 2) {
            TIndex insertPos = 0;

            for (TIndex l = 0; l < size; l += stepWidth * 2) {
                TIndex m = std::min(l + stepWidth, size);
                TIndex r = std::min(m + stepWidth, size);

                TIndex pos1 = l;
                TIndex pos2 = m;

                if (round % 2 == 0) {  // elem -> extra
                    while (pos1 < m && pos2 < r) {
                        extraSpace[insertPos++] = elemAt(pos1 < m && elemAt(pos1) < elemAt(pos2) ? pos1++ : pos2++);
                    }
                    while (pos1 < m) extraSpace[insertPos++] = elemAt(pos1++);
                    while (pos2 < r) extraSpace[insertPos++] = elemAt(pos2++);
                } else {  // extra -> elem
                    while (pos1 < m && pos2 < r) {
                        elemAt(insertPos++) = extraSpace[extraSpace[pos1] < extraSpace[pos2] ? pos1++ : pos2++];
                    }
                    while (pos1 < m) elemAt(insertPos++) = extraSpace[pos1++];
                    while (pos2 < r) elemAt(insertPos++) = extraSpace[pos2++];
                }
            }
            round++;
        }

        if (round % 2 == 1) {
            for (TIndex i = 0; i < size; i++) {
                elemAt(i) = extraSpace[i];
            }
        }
        delete[] extraSpace;
    }
};
#endif
