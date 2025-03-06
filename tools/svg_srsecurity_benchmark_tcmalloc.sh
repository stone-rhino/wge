#!/bin/bash
cd perf
perf script -i srsecurity_benchmark_tcmalloc.perf.data | c++filt > srsecurity_benchmark_tcmalloc.out.perf
~/github.com/FlameGraph/stackcollapse-perf.pl srsecurity_benchmark_tcmalloc.out.perf > srsecurity_benchmark_tcmalloc.out.floded
~/github.com/FlameGraph/flamegraph.pl srsecurity_benchmark_tcmalloc.out.floded > srsecurity_benchmark_tcmalloc.svg
explorer.exe srsecurity_benchmark_tcmalloc.svg
cd -