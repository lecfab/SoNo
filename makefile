CC=g++ -std=c++14 -fconcepts
# CFLAGS=-O9 -lm

DEBUG?=0 # optimisation option

EXEC=read ord undirect rankedges
MF=makefile # recompile when Makefile has been modified

ifeq ($(DEBUG), 1)
	CFLAGS=-Og -Wextra -g3 -D DEBUG
	o=debug.o
else
	CFLAGS=-Ofast # https://cpluspluspedia.com/fr/tutorial/4708/compiler-et-construire
	o=o
endif


UTILS_H =	utils/tools.h \
			utils/inout.h \
			utils/edgelist.h \
			utils/adjlist.h \
			utils/heap.h \
			utils/unitheap.h
			# utils/CLI11.h
ALGOS_H = 	algo/algo_nq.h \
			algo/algo_bfs.h \
			algo/algo_dfs.h \
			algo/algo_bellman.h \
			algo/algo_diameter.h \
			algo/algo_kcore.h \
			algo/algo_tarjan.h
ORDERS_H = 	order/order_core.h \
			order/order_deg.h \
			order/order_rand.h \
			order/order_rcm.h \
			order/order_gorder.h

UTILS_O =	$(UTILS_H:.h=.$(o))
ALGOS_O = 	$(ALGOS_H:.h=.$(o))
ORDERS_O = 	$(ORDERS_H:.h=.$(o))

all: $(EXEC)

read: read.$(o) $(UTILS_O) $(ALGOS_O) $(ORDERS_O)
	$(CC) $^ $(CFLAGS) -o $@

ord: ord.$(o) $(UTILS_O) $(ALGOS_O) $(ORDERS_O) #algo/algo_tarjan.$(o)
	$(CC) $^ $(CFLAGS) -o $@

undirect: undirect.$(o) $(UTILS_O)
	$(CC) $^ $(CFLAGS) -o $@

rankedges: rankedges.$(o) $(UTILS_O)
	$(CC) $^ $(CFLAGS) -o $@


read.$(o): $(UTILS_H) $(ALGOS_H) $(ORDERS_H)
ord.$(o): $(UTILS_H) $(ALGOS_H) $(ORDERS_H)
undirect.$(o): $(UTILS_H)
rankedges.$(o): $(UTILS_H)


algo/algo_diameter.$(o): algo/algo_bellman.h
algo/algo_tarjan.$(o): algo/algo_dfs.h
order/order_core.$(o): algo/algo_kcore.h
order/order_rcm.$(o): order/order_deg.h algo/algo_bfs.h
order/order_gorder.$(o): order/order_rcm.h


# utils/CLI11.$(o): utils/CLI11.h
# 	$(CC) -c $< $(CFLAGS) -o $@

%.debug.o: %.cpp $(MF)
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.cpp $(MF)
	@$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean mrproper

clean:
	rm -rf */*.$(o) *.$(o)

mrproper: clean
	rm -rf $(EXEC)
