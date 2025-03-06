#!/bin/bash
cd perf
perf script -i srsecurity_benchmark.perf.data | c++filt > srsecurity_benchmark.out.perf
~/github.com/FlameGraph/stackcollapse-perf.pl srsecurity_benchmark.out.perf > srsecurity_benchmark.out.floded
~/github.com/FlameGraph/flamegraph.pl srsecurity_benchmark.out.floded > srsecurity_benchmark.svg
explorer.exe srsecurity_benchmark.svg
cd -