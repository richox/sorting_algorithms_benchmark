#ifndef HEADER_ALGO_HEAP_SORT_HH
#define HEADER_ALGO_HEAP_SORT_HH

#include <algorithm>

// https://en.wikipedia.org/wiki/Heapsort
struct HeapSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "HeapSort";
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
        return "O(1)";
    }
    static inline const char* getStability() {
        return "No";
    }

    template<typename TValue, typename TIndex, typename TElemAt> void sort(TElemAt elemAt, TIndex size) {
        TIndex top = size / 4;
        TIndex bottom = size;

        while (bottom > 0) {
            if (top > 0) {
                top -= 1;
            }else {
                std::swap(elemAt(0), elemAt(bottom - 1));
                bottom -= 1;
            }

            TValue topElem = elemAt(top);
            TIndex current = top;

            while (current * 4 + 1 < bottom) {
                TIndex maxChild = current * 4 + 1;
                if (current * 4 + 2 < bottom && elemAt(current * 4 + 2) > elemAt(maxChild)) maxChild = current * 4 + 2;
                if (current * 4 + 3 < bottom && elemAt(current * 4 + 3) > elemAt(maxChild)) maxChild = current * 4 + 3;
                if (current * 4 + 4 < bottom && elemAt(current * 4 + 4) > elemAt(maxChild)) maxChild = current * 4 + 4;
                elemAt(current) = elemAt(maxChild);
                current = maxChild;
            }
            while (current > top && elemAt((current - 1) / 4) < topElem) {
                elemAt(current) = elemAt((current - 1) / 4);
                current = (current - 1) / 4;
            }
            elemAt(current) = topElem;
        }
    }
};
#endif
