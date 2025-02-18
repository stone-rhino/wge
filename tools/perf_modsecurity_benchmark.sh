#!/bin/bash
if [[ ! -d perf ]]; then
  mkdir perf
fi
LD_LIBRARY_PATH=/usr/local/lib64 perf record -g -F99 -o perf/modsecurity_benchmark.perf.data -- build/debug/benchmarks/modsecurity/modsecurity_benchmark