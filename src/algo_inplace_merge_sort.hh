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
    static inline const char* getStability() {
        return "No";
    }
    static const int NAIVE_INSERT_LIMIT = 16;

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
            swapBlock<TValue>(elemAt, l, a, size1);
            TIndex insertPos = l;
            TIndex pos1 = a;
            TIndex pos2 = m;
            while (size1 > 0 && size2 > 0) {
                std::swap(elemAt(insertPos++), elemAt(elemAt(pos1) < elemAt(pos2)
                            ? (size1--, pos1++)
                            : (size2--, pos2++)));
            }
            while (size1-- > 0) std::swap(elemAt(insertPos++), elemAt(pos1++));
            while (size2-- > 0) std::swap(elemAt(insertPos++), elemAt(pos2++));

        } else {
            swapBlock<TValue>(elemAt, m, a, size2);
            TIndex insertPos = r - 1;
            TIndex pos1 = m - 1;
            TIndex pos2 = a + size2 - 1;
            while (size1 > 0 && size2 > 0) {
                std::swap(elemAt(insertPos--), elemAt(elemAt(pos1) > elemAt(pos2)
                            ? (size1--, pos1--)
                            : (size2--, pos2--)));
            }
            while (size1-- > 0) std::swap(elemAt(insertPos--), elemAt(pos1--));
            while (size2-- > 0) std::swap(elemAt(insertPos--), elemAt(pos2--));
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
            swapBlock<TValue>(elemAt, blockHead((m - l) / blockSize), blockHead(blockNum - 2), blockSize);
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
                swapBlock<TValue>(elemAt, blockHead(insertBlockId), blockHead(minBlockId), blockSize);
            }
        }

        // merge each block pairs
        for (TIndex blockId = 1; blockId < blockNum - 2; blockId++) {
            mergeWithAuxSpace<TValue>(
                    elemAt,
                    blockHead(blockId - 1),
                    blockHead(blockId),
                    blockHead(blockId + 1),
                    blockHead(blockNum - 2));
        }

        TIndex tailSize = r - blockHead(blockNum - 2);
        naiveInsertionSort<TValue>(
                elemAt,
                blockHead(blockNum - 2) - tailSize,
                blockHead(blockNum - 2) + tailSize);
        mergeWithAuxSpace<TValue>(
                elemAt,
                blockHead(0),
                blockHead(blockNum - 2) - tailSize,
                blockHead(blockNum - 2),
                blockHead(blockNum - 2));
        naiveInsertionSort<TValue>(
                elemAt,
                blockHead(blockNum - 2),
                blockHead(blockNum - 2) + tailSize);
    }

    template<typename TIndex> static inline TIndex getMergedBlockSize(TIndex index) {
        TIndex blockSize = 1;
        index++;

        while (index % 2 == 0) {
            index /= 2;
            blockSize *= 2;
        }
        return blockSize;
    }

    template <typename TValue, typename TIndex, typename TElemAt> static inline void sort(TElemAt elemAt, TIndex size) {
        TIndex numBlocks = size / NAIVE_INSERT_LIMIT + (size % NAIVE_INSERT_LIMIT != 0);

        for (TIndex i = 0; i < numBlocks; i++) {
            TIndex bl = i * NAIVE_INSERT_LIMIT;
            TIndex br = std::min((i + 1) * NAIVE_INSERT_LIMIT, size);
            naiveInsertionSort<TValue>(elemAt, bl, br);

            for (TIndex mergedBlockSize = 2; (i + 1) % mergedBlockSize == 0; mergedBlockSize *= 2) {
                TIndex l = (i + 1 - mergedBlockSize) * NAIVE_INSERT_LIMIT;
                TIndex m = std::min(l + mergedBlockSize / 2 * NAIVE_INSERT_LIMIT, size);
                TIndex r = std::min(m + mergedBlockSize / 2 * NAIVE_INSERT_LIMIT, size);
                if (l < m && m < r) {
                    if (r - m < size - r) {
                        mergeWithAuxSpace<TValue>(elemAt, l, m, r, r);
                    } else {
                        mergeInplace<TValue>(elemAt, l, m, r);
                    }
                }
            }
        }

        auto getLastUnmergedBlock = [](TIndex numBlocks) {
            for (TIndex mergedBlockSize = 2; mergedBlockSize < numBlocks; mergedBlockSize *= 2) {
                if (numBlocks % mergedBlockSize != 0) {
                    return mergedBlockSize / 2;
                }
            }
            return numBlocks;
        };

        while (true) {
            TIndex b1 = getLastUnmergedBlock(numBlocks);
            TIndex b2 = getLastUnmergedBlock(numBlocks - b1);
            if (b1 < 1 || b2 < 1) {
                break;
            }
            TIndex l = (numBlocks - b1 - b2) * NAIVE_INSERT_LIMIT;
            TIndex m = (numBlocks - b1) * NAIVE_INSERT_LIMIT;
            mergeInplace<TValue>(elemAt, l, m, size);
            numBlocks -= b1;
        }
    }
};
#endif
