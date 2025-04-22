#!/bin/bash
if [[ ! -d perf ]]; then
  mkdir perf
fi
LD_LIBRARY_PATH=/usr/local/lib64 perf stat -e cache-references,cache-misses -- build/release-with-debug-info/benchmarks/wge/wge_benchmark