#!/bin/sh
cd build
valgrind --tool=helgrind ./ConcurrentList 2> ../result_helgrind.txt
