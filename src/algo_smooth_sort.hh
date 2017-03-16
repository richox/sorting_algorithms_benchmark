#ifndef HEADER_ALGO_SMOOTH_SORT_HH
#define HEADER_ALGO_SMOOTH_SORT_HH

#include <algorithm>

// https://en.wikipedia.org/wiki/Smoothsort
struct SmoothSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "SmoothSort";
    }
    static inline const char* getAlgorithmBestTimeComplex() {
        return "O(n)";
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

    static inline unsigned getLeonardo(int i) {
        static const unsigned leonardo[] = {
            1, 1, 3, 5, 9, 15, 25, 41, 67, 109,
            177, 287, 465, 753, 1219, 1973, 3193, 5167, 8361, 13529,
            21891, 35421, 57313, 92735, 150049, 242785, 392835, 635621, 1028457, 1664079,
            2692537, 4356617, 7049155, 11405773, 18454929, 29860703, 48315633, 78176337, 126491971, 204668309,
            331160281, 535828591, 866988873, 1402817465, 2269806339u, 3672623805u,
        };
        return leonardo[i];
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void fixHeaps(
            TElemAt elemAt,
            TIndex currentHeap,
            int levelIndex,
            int levels[]) {
        while (levelIndex > 0) {
            TIndex prevHeap = currentHeap - getLeonardo(levels[levelIndex]);
            if (elemAt(currentHeap) < elemAt(prevHeap)) {
                if (levels[levelIndex] > 1) {
                    TIndex childHeap1 = currentHeap - 1 - getLeonardo(levels[levelIndex] - 2);
                    TIndex childHeap2 = currentHeap - 1;
                    if (elemAt(prevHeap) < elemAt(childHeap1)) break;
                    if (elemAt(prevHeap) < elemAt(childHeap2)) break;
                }
                std::swap(elemAt(currentHeap), elemAt(prevHeap));
                currentHeap = prevHeap;
                levelIndex -= 1;
            } else {
                break;
            }
        }

        int currentLevel = levels[levelIndex];
        while (currentLevel > 1) {
            TIndex maxChildHeap = currentHeap;
            TIndex childHeap1 = currentHeap - 1 - getLeonardo(currentLevel - 2);
            TIndex childHeap2 = currentHeap - 1;

            if (elemAt(maxChildHeap) < elemAt(childHeap1)) maxChildHeap = childHeap1;
            if (elemAt(maxChildHeap) < elemAt(childHeap2)) maxChildHeap = childHeap2;
            if (maxChildHeap == currentHeap) {
                break;
            }
            std::swap(elemAt(currentHeap), elemAt(maxChildHeap));
            currentHeap = maxChildHeap;
            currentLevel -= 1 + (maxChildHeap == childHeap2);
        }
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        int topLevel = 0;
        int levels[64] = {1};

        for (TIndex i = 1; i < size; i++) {
            if (topLevel > 0 && levels[topLevel - 1] - levels[topLevel] == 1) {
                levels[--topLevel] += 1;
            } else if (levels[topLevel] != 1) {
                levels[++topLevel] = 1;
            } else {
                levels[++topLevel] = 0;
            }
            fixHeaps<TValue, TIndex>(elemAt, i, topLevel, levels);
        }

        for (TIndex i = size - 2; i > 0; i--) {
            if (levels[topLevel] <= 1) {
                topLevel -= 1;
            } else {
                levels[topLevel] -= 1;
                levels[topLevel + 1] = levels[topLevel] - 1;
                topLevel += 1;
                fixHeaps<TValue, TIndex>(elemAt, i - getLeonardo(levels[topLevel]), topLevel - 1, levels);
                fixHeaps<TValue, TIndex>(elemAt, i, topLevel, levels);
            }
        }
    }
};
#endif
