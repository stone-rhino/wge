#!/bin/bash
cd perf
perf script -i wge_benchmark.perf.data | c++filt > wge_benchmark.out.perf
~/github.com/FlameGraph/stackcollapse-perf.pl wge_benchmark.out.perf > wge_benchmark.out.floded
~/github.com/FlameGraph/flamegraph.pl wge_benchmark.out.floded > wge_benchmark.svg
explorer.exe wge_benchmark.svg
cd -