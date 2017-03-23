#ifndef HEADER_ALGO_MULTIHEAP_SORT_HH
#define HEADER_ALGO_MULTIHEAP_SORT_HH

#include <cmath>
#include <algorithm>
#include "util.hh"

// my multi-heap sort algorithm:
//  1. split input array into S=logn blocks, with each block T = n/logn (except the last one)
//  2. for block1 .. blockS, make each block a min-heap
//  3. for each element in block[0]:
//      if the element is bigger then smallest element in all heaps:
//          swap them
//          heapify the swapped heap
//  4. now block[0] contains the smallest n/logn elements
//  5. recursively sort block[0]
//  6. process block1 .. blockS in the same way as (3,4,5)
//
// complexity:
//  F(n) = O(n){init heap} + S*O(log(T)){find smallest n elements} + S*F(T){recursively sort}
//       = O(n) + logn*F(n/logn)
//       = O(nlogn)
//
// advantages:
//  1. no need to build a reversed heap, for ordered input, no swap is required
//
// disadvantages:
//  1. even slower then heap sort
//  2. need recursion
//
struct MultiheapSortAlgorithm {
    static inline const char* getAlgorithmName() {
        return "MultiheapSort";
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
    static const int NAIVE_INSERT_LIMIT = 64;  // insert sort on a small heap is more effective

    template<typename TValue, typename TIndex, typename TElemAt> void heapify(
            TElemAt elemAt,
            TIndex offset,
            TIndex i,
            TIndex size) {

#define elemAt(i) elemAt(offset + (i))
        TValue topElem = elemAt(i);
        TIndex current = i;

        while (current * 4 + 1 < size) {
            TIndex minChild = current * 4 + 1;
            if (current * 4 + 2 < size && elemAt(current * 4 + 2) < elemAt(minChild)) minChild = current * 4 + 2;
            if (current * 4 + 3 < size && elemAt(current * 4 + 3) < elemAt(minChild)) minChild = current * 4 + 3;
            if (current * 4 + 4 < size && elemAt(current * 4 + 4) < elemAt(minChild)) minChild = current * 4 + 4;
            elemAt(current) = elemAt(minChild);
            current = minChild;
        }
        while (current > i && elemAt((current - 1) / 4) > topElem) {
            elemAt(current) = elemAt((current - 1) / 4);
            current = (current - 1) / 4;
        }
        elemAt(current) = topElem;
#undef elemAt
    }

    template<typename TValue, typename TIndex, typename TElemAt> void sortImpl(TElemAt elemAt, TIndex from, TIndex to) {
        if (to - from < NAIVE_INSERT_LIMIT) {
            naiveInsertionSort<TValue>(elemAt, from, to);
            return;
        }
        const TIndex blockSize = (to - from) / std::log2(to - from) * 0.125;
        const TIndex numBlocks = (to - from) / blockSize + ((to - from) % blockSize != 0);

        const auto blockHead = [=](TIndex i) {
            return from + blockSize * i;
        };
        const auto blockTail = [=](TIndex i) {
            return i + 1 < numBlocks ? blockHead(i + 1) : to;
        };

        struct BlockAndValue {
            TValue m_value;
            int m_block;

            bool operator < (const BlockAndValue& that) const {
                return m_value > that.m_value;  // reversed
            }
        } heapedBlocks[numBlocks];

        const auto elemAtHeapBlocks = [&heapedBlocks](int i) -> BlockAndValue& {
            return heapedBlocks[i];
        };

        for (int nblock = 1; nblock < numBlocks; nblock++) {  // init heap block1 .. blockS
            for (TIndex i = (blockTail(nblock) - blockHead(nblock)) / 4; i >= 0; i--) {
                heapify<TValue>(elemAt, blockHead(nblock), i, blockTail(nblock) - blockHead(nblock));
            }
            heapedBlocks[nblock].m_value = elemAt(blockHead(nblock));
            heapedBlocks[nblock].m_block = nblock;
            naiveInsert<BlockAndValue>(elemAtHeapBlocks, 1, nblock);
        }

        for (int nblock = 0; nblock < numBlocks; nblock++) {  // process from blocks0 to blockS
            TIndex currentBlockHead = blockHead(nblock);
            TIndex currentBlockTail = blockTail(nblock);

            if (nblock + 1 < numBlocks) {
                for (TIndex i = currentBlockHead; i < currentBlockTail; i++) {
                    if (heapedBlocks[numBlocks - 1].m_value < elemAt(i)) {  // swap and heapify
                        TIndex minBlockHead = blockHead(heapedBlocks[numBlocks - 1].m_block);
                        TIndex minBlockTail = blockTail(heapedBlocks[numBlocks - 1].m_block);

                        elemAt(minBlockHead) = elemAt(i);
                        heapify<TValue>(elemAt, minBlockHead, 0, minBlockTail - minBlockHead);

                        elemAt(i) = heapedBlocks[numBlocks - 1].m_value;
                        heapedBlocks[numBlocks - 1].m_value = elemAt(minBlockHead);
                        naiveInsert<BlockAndValue>(elemAtHeapBlocks, nblock + 1, numBlocks - 1);
                    }
                }

                for (int i = nblock + 2; i < numBlocks; i++) {  // remove block from ordered blocks
                    if (heapedBlocks[i].m_block == nblock + 1) {
                        heapedBlocks[i] = heapedBlocks[nblock + 1];
                        naiveInsert<BlockAndValue>(elemAtHeapBlocks, nblock + 2, i);
                        break;
                    }
                }
            }
            sortImpl<TValue>(elemAt, currentBlockHead, currentBlockTail);  // recursive sort each block
        }
    }

    template<typename TValue, typename TIndex, typename TElemAt> void sort(TElemAt elemAt, TIndex size) {
        sortImpl<TValue>(elemAt, 0, size);
    }
};
#endif
