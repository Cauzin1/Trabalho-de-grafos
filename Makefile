# ============================================================================
#  Makefile - Trabalho Pratico 1 (DCC059)
#  Tema: Componentes Conexas | Estrutura: Lista de Adjacencia
# ============================================================================

CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra -O2
SRC_LIB   = Grafo.cpp ComponentesConexas.cpp
HDR       = Grafo.h ComponentesConexas.h

# Alvos principais
all: programa test_grafo

# Programa interativo com menu
programa: main.cpp $(SRC_LIB) $(HDR)
	$(CXX) $(CXXFLAGS) -o programa main.cpp $(SRC_LIB)

# Bateria de testes obrigatoria
test_grafo: testall.cpp Grafo.cpp Grafo.h
	$(CXX) $(CXXFLAGS) -o test_grafo testall.cpp Grafo.cpp

# Execucoes rapidas
run: programa
	./programa

test: test_grafo
	./test_grafo

clean:
	rm -f programa test_grafo *.o

.PHONY: all run test clean
