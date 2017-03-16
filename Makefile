#!/bin/bash

benchSize = $(or $S, 1000000)

bench:
	@ c++ -std=c++11 -Wall -O3 -o /tmp/bench src/sorter_bencher.cc
	@ /tmp/bench ${benchSize}

.PHONY: .bench
