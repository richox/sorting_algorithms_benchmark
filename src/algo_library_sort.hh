#ifndef HEADER_ALGO_RICHOX_SORT_HH
#define HEADER_ALGO_RICHOX_SORT_HH

#include <algorithm>

// https://en.wikipedia.org/wiki/Library_sort
// my variant version, using pointers for gapped insertion, archiving smaller auxility space and faster sorting
struct LibrarySortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "LibrarySort";
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
        return "O(n)";
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        TIndex* next = new TIndex[size];
        next[0] = -1;

        TIndex sorted = 1;
        for (TIndex pos = 1; pos < size; pos++) {
            TIndex l = 0;
            TIndex r = sorted - 1;

            // binary search for insert pos
            while (l <= r) {
                elemAt(pos) < elemAt((l + r) / 2)
                    ? r = (l + r) / 2 - 1
                    : l = (l + r) / 2 + 1;
            }
            if (r < 0) {  // insert into first element
                std::swap(elemAt(pos), elemAt(0));
                next[pos] = next[0];
                next[0] = pos;
                continue;
            }

            // insert
            while (next[r] >= 0 && elemAt(next[r]) < elemAt(pos)) {
                r = next[r];
            }
            next[pos] = next[r];
            next[r] = pos;

            // rebalance
            if (pos > sorted * 3 || pos == size - 1) {
                TIndex rank = 0;
                for (TIndex i = 0; i <= pos; i++) {
                    TIndex nextRank = next[rank];
                    next[rank] = i;
                    rank = nextRank;
                }
                for (TIndex i = 0; i <= pos; i++) {
                    while (i != next[i]) {
                        std::swap(elemAt(i), elemAt(next[i]));
                        std::swap(next[i], next[next[i]]);
                    }
                    next[i] = i + 1;
                }
                next[pos] = -1;
                sorted = pos + 1;
            }
        }
        delete[] next;
    }
};
#endif
