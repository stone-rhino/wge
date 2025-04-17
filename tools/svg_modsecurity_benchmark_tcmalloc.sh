#!/bin/bash
cd perf
perf script -i modsecurity_benchmark_tcmalloc.perf.data | c++filt > modsecurity_benchmark_tcmalloc.out.perf
~/github.com/FlameGraph/stackcollapse-perf.pl modsecurity_benchmark_tcmalloc.out.perf > modsecurity_benchmark_tcmalloc.out.floded
~/github.com/FlameGraph/flamegraph.pl modsecurity_benchmark_tcmalloc.out.floded > modsecurity_benchmark_tcmalloc.svg
explorer.exe modsecurity_benchmark_tcmalloc.svg
cd -