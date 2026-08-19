import os

from ..SimulationWrapperTypes import *


max_threads = os.cpu_count()

thread_count_list = []

n = 1
while n < max_threads:
    thread_count_list.append(n)
    n *= 2
thread_count_list.append(max_threads)
