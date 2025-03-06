#!/bin/bash
if [[ ! -d perf ]]; then
  mkdir perf
fi
LD_LIBRARY_PATH=/usr/local/lib64 perf record -g -F99 -o perf/srsecurity_benchmark_tcmalloc.perf.data -- build/release-with-debug-info/benchmarks/srsecurity/srsecurity_benchmark_tcmalloc