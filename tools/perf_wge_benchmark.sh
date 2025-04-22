#!/bin/bash
if [[ ! -d perf ]]; then
  mkdir perf
fi
LD_LIBRARY_PATH=/usr/local/lib64 perf record -g -F99 -o perf/wge_benchmark.perf.data -- build/release-with-debug-info/benchmarks/wge/wge_benchmark