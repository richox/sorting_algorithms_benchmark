#!/bin/bash

bench:
	@ c++ -std=c++11 -Wall -O3 -o /tmp/bench src/sorter_bencher.cc
	@ /tmp/bench

.PHONY: .bench
