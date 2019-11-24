CC = g++
CFLAGS = -c -Wall -std=c++11
AR = ar
ARFLAGS = rcv
DBGFLAGS = -g -D_DEBUG_ON_ -Wall -std=c++11
OPTFLAGS = -O3 # -O3

RM = rm -rf

all : bin/net_open_finder
	@echo -n ""

#optimized version
bin/net_open_finder : shape_opt.o graph_opt.o main_opt.o lib
	${CC} ${OPTFLAGS} shape_opt.o main_opt.o graph_opt.o  -ltm_usage -Llib -o bin/net_open_finder
main_opt.o 			: src/main.cpp lib/tm_usage.h
	${CC} ${CFLAGS} $< -Ilib -o $@
shape_opt.o 			: src/shapeMgr.cpp src/shapeMgr.h src/obj.h src/graph.h
	${CC} ${CFLAGS} ${OPTFLAGS} $< -o $@
graph_opt.o 			: src/graph.cpp src/graph.h 
	${CC} ${CFLAGS} ${OPTFLAGS} $< -o $@

#DEBUG version
dbg : bin/net_open_finder_dbg
	@echo -n ""

bin/net_open_finder_dbg : shape_dbg.o graph_dbg.o main_dbg.o lib
	${CC} ${DBGFLAGS} shape_dbg.o graph_dbg.o main_dbg.o -ltm_usage -Llib -o bin/net_open_finder_dbg
main_dbg.o 				: src/main.cpp lib/tm_usage.h
	${CC} ${CFLAGS} $< -Ilib -o $@
shape_dbg.o 			: src/shapeMgr.cpp src/shapeMgr.h src/obj.h src/graph.h
	${CC} ${CFLAGS} ${DBGFLAGS} $< -o $@
graph_dbg.o 			: src/graph.cpp src/graph.h 
	${CC} ${CFLAGS} ${DBGFLAGS} $< -o $@


lib : lib/libtm_usage.a

lib/libtm_usage.a 	: tm_usage.o
	${AR} ${ARFLAGS} $@ $<
tm_usage.o 			: lib/tm_usage.cpp lib/tm_usage.h
	${CC} ${CFLAGS} $<

clean :
	${RM} *.o lib/*.a lib/*.o bin/*
