CC=g++ #-std=c++14
# CFLAGS=-O9 -lm

FAST?=0 # optimisation option

ifeq ($(FAST), 1)
	CFLAGS=-Ofast # https://cpluspluspedia.com/fr/tutorial/4708/compiler-et-construire
else
	CFLAGS=-Og -Wextra
endif

EXEC=read ord

all: $(EXEC)

read: read.o utils orders algos 
	$(CC) $< $(CFLAGS) -o read

ord: ord.o utils orders 
	$(CC) $< $(CFLAGS) -o ord


utils: utils/tools.o utils/inout.o utils/adjlist.o utils/heap.o

algos: utils algo/algo_nq.o algo/algo_bfs.o algo/algo_dfs.o algo/algo_bellman.o algo/algo_diameter.o algo/algo_kcore.o

orders: utils algo/algo_kcore.o order/order_core.o order/order_deg.o order/order_rand.o

%.o: %.cpp
	@$(CC) -c $< $(CFLAGS) -o $@

# main.o: main.c
# 	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm -rf */*.o *.o

mrproper: clean
	rm -rf $(EXEC)
