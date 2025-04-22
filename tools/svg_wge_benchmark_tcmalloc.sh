#!/bin/bash
cd perf
perf script -i wge_benchmark_tcmalloc.perf.data | c++filt > wge_benchmark_tcmalloc.out.perf
~/github.com/FlameGraph/stackcollapse-perf.pl wge_benchmark_tcmalloc.out.perf > wge_benchmark_tcmalloc.out.floded
~/github.com/FlameGraph/flamegraph.pl wge_benchmark_tcmalloc.out.floded > wge_benchmark_tcmalloc.svg
explorer.exe wge_benchmark_tcmalloc.svg
cd -