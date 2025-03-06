#!/bin/bash
cd perf
perf script -i modsecurity_benchmark.perf.data | c++filt > modsecurity_benchmark.out.perf
~/github.com/FlameGraph/stackcollapse-perf.pl modsecurity_benchmark.out.perf > modsecurity_benchmark.out.floded
~/github.com/FlameGraph/flamegraph.pl modsecurity_benchmark.out.floded > modsecurity_benchmark.svg
explorer.exe modsecurity_benchmark.svg
cd -