#ifndef HEADER_UTIL_HH
#define HEADER_UTIL_HH

template <typename TValue, typename TIndex, typename TElemAt> static inline void naiveInsertionSort(
        TElemAt elemAt,
        TIndex from,
        TIndex to) {

    for (TIndex pos = from + 1; pos < to; pos++) {
        TValue posElem = elemAt(pos);
        TIndex insertPos;
        for (insertPos = pos; insertPos > from && posElem < elemAt(insertPos - 1); insertPos--) {
            elemAt(insertPos) = elemAt(insertPos - 1);
        }
        elemAt(insertPos) = posElem;
    }
}
#endif
