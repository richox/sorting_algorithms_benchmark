#ifndef HEADER_ALGO_INPLACE_MERGE_SORT_HH
#define HEADER_ALGO_INPLACE_MERGE_SORT_HH

#include <cmath>
#include <algorithm>
#include "util.hh"

// https://en.wikipedia.org/wiki/Merge_sort#Variants
// Kronrod's variant version
struct InplaceMergeSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "InplaceMergeSort";
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
    static const int NAIVE_INSERT_LIMIT = 96;

    template <typename TValue, typename TIndex, typename TElemAt> static inline void swapBlock(
            TElemAt elemAt,
            TIndex b1,
            TIndex b2,
            TIndex size) {

        for (TIndex i = 0; i < size; i++) {
            std::swap(elemAt(b1 + i), elemAt(b2 + i));
        }
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void mergeWithAuxSpace(
            TElemAt elemAt,
            TIndex l,
            TIndex m,
            TIndex r,
            TIndex a) {

        TIndex size1 = m - l;
        TIndex size2 = r - m;

        if ((m - l) <= (r - m)) {
            swapBlock<TValue, TIndex>(elemAt, l, a, size1);
            TIndex insertPos = l;
            TIndex pos1 = a;
            TIndex pos2 = m;
            while (pos1 < a + size2 && pos2 < r) {
                std::swap(elemAt(insertPos++), elemAt(elemAt(pos1) < elemAt(pos2) ? pos1++ : pos2++));
            }
            while (pos1 < a + size2) std::swap(elemAt(insertPos++), elemAt(pos1++));
            while (pos2 < r)         std::swap(elemAt(insertPos++), elemAt(pos2++));

        } else {
            swapBlock<TValue, TIndex>(elemAt, m, a, size2);
            TIndex insertPos = r - 1;
            TIndex pos1 = a + size2 - 1;
            TIndex pos2 = m - 1;
            while (pos1 >= a && pos2 >= l) {
                std::swap(elemAt(insertPos--), elemAt(elemAt(pos1) > elemAt(pos2) ? pos1-- : pos2--));
            }
            while (pos1 >= a) std::swap(elemAt(insertPos--), elemAt(pos1--));
            while (pos2 >= l) std::swap(elemAt(insertPos--), elemAt(pos2--));
        }
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void mergeInplace(
            TElemAt elemAt,
            TIndex l,
            TIndex m,
            TIndex r) {
        TIndex blockSize = (TIndex) std::sqrt(r - l);
        TIndex blockNum = (r - l) / blockSize + 1;
        auto blockHead = [l, blockSize](TIndex id) { return l + id * blockSize; };
        auto blockTail = [l, blockSize](TIndex id) { return l + id * blockSize + blockSize - 1; };

        // swap middle block with blockNum-2 block
        if ((m - l) / blockSize < blockNum - 2) {
            swapBlock<TValue, TIndex>(elemAt, blockHead((m - l) / blockSize), blockHead(blockNum - 2), blockSize);
        }

        // sort ordered blocks with first and last elements
        for (TIndex insertBlockId = 0; insertBlockId < blockNum - 2; insertBlockId++) {
            TIndex minBlockId = insertBlockId;
            for (TIndex selectBlockId = insertBlockId + 1; selectBlockId < blockNum - 2; selectBlockId++) {
                if (elemAt(blockHead(selectBlockId)) < elemAt(blockHead(minBlockId)) ||
                        (elemAt(blockHead(selectBlockId)) == elemAt(blockHead(minBlockId)) &&
                         elemAt(blockTail(selectBlockId)) <  elemAt(blockTail(minBlockId)))) {
                    minBlockId = selectBlockId;
                }
            }
            if (minBlockId != insertBlockId) {
                swapBlock<TValue, TIndex>(elemAt, blockHead(insertBlockId), blockHead(minBlockId), blockSize);
            }
        }

        // merge each block pairs
        for (TIndex blockId = 1; blockId < blockNum - 2; blockId++) {
            mergeWithAuxSpace<TValue, TIndex>(
                    elemAt,
                    blockHead(blockId - 1),
                    blockHead(blockId),
                    blockHead(blockId + 1),
                    blockHead(blockNum - 2));
        }

        TIndex tailSize = r - blockHead(blockNum - 2);
        naiveInsertionSort<TValue, TIndex>(
                elemAt,
                blockHead(blockNum - 2) - tailSize,
                blockHead(blockNum - 2) + tailSize);
        mergeWithAuxSpace<TValue, TIndex>(
                elemAt,
                blockHead(0),
                blockHead(blockNum - 2) - tailSize,
                blockHead(blockNum - 2),
                blockHead(blockNum - 2));
        naiveInsertionSort<TValue, TIndex>(
                elemAt,
                blockHead(blockNum - 2),
                blockHead(blockNum - 2) + tailSize);
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        for (TIndex l = 0; l < size; l += NAIVE_INSERT_LIMIT) {
            naiveInsertionSort<TValue, TIndex>(elemAt, l, std::min(l + NAIVE_INSERT_LIMIT, size));
        }
        for (TIndex stepWidth = NAIVE_INSERT_LIMIT; stepWidth < size; stepWidth *= 2) {
            for (TIndex l = 0; l + stepWidth < size; l += stepWidth * 2) {
                TIndex m = l + stepWidth;
                TIndex r = std::min(m + stepWidth, size);
                mergeInplace<TValue, TIndex>(elemAt, l, m, r);
            }
        }
    }
};
#endif
