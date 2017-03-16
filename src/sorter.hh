#ifndef HEADER_SORTER_HH
#define HEADER_SORTER_HH

#include <algorithm>

template <typename SortAlgorithm> struct Sorter {
    static const char* getAlgorithmName() {
        return SortAlgorithm::getAlgorithmName();
    }
    static inline const char* getAlgorithmBestTimeComplex() {
        return SortAlgorithm::getAlgorithmBestTimeComplex();
    }
    static inline const char* getAlgorithmAvgTimeComplex() {
        return SortAlgorithm::getAlgorithmAvgTimeComplex();
    }
    static inline const char* getAlgorithmWorstTimeComplex() {
        return SortAlgorithm::getAlgorithmWorstTimeComplex();
    }
    static inline const char* getAlgorithmSpaceComplex() {
        return SortAlgorithm::getAlgorithmSpaceComplex();
    }
    static inline const char* getStability() {
        return SortAlgorithm::getStability();
    }

    template<
        typename It,
        typename TValue = typename It::value_type,
        typename TIndex = typename It::difference_type> static inline void sort(It begin, It end) {

        SortAlgorithm().template sort<TValue, TIndex>(
                [begin](TIndex i)-> TValue& {
                    return begin[i];
                },
                std::distance(begin, end));
    }
};
#endif
