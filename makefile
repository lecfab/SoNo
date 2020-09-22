CC=g++ #-std=c++14
# CFLAGS=-O9 -lm

DEBUG?=0 # optimisation option

ifeq ($(DEBUG), 1)
	CFLAGS=-Og -Wextra -D DEBUG
	o=debug.o
else
	CFLAGS=-Ofast # https://cpluspluspedia.com/fr/tutorial/4708/compiler-et-construire
	o=o
endif

EXEC=read ord
MF=makefile # recompile when Makefile has been modified

all: $(EXEC)

read: read.$(o) utils orders algos $(MF)
	$(CC) $< $(CFLAGS) -o read

ord: ord.$(o) utils orders $(MF) 
	$(CC) $< $(CFLAGS) -o ord


utils: utils/tools.$(o) utils/inout.$(o) utils/adjlist.$(o) utils/heap.$(o)

algos: utils algo/algo_nq.$(o) algo/algo_bfs.$(o) algo/algo_dfs.$(o) algo/algo_bellman.$(o) algo/algo_diameter.$(o) algo/algo_kcore.$(o) algo/algo_tarjan.$(o)

orders: utils algo/algo_kcore.$(o) order/order_core.$(o) order/order_deg.$(o) order/order_rand.$(o)

%.$(o): %.cpp $(MF)
	$(CC) -c $< $(CFLAGS) -o $@

# main.$(o): main.c
# 	$(CC) -o main.$(o) -c main.c $(CFLAGS)

.PHONY: clean mrproper

clean:
	rm -rf */*.$(o) *.$(o)

mrproper: clean
	rm -rf $(EXEC)
