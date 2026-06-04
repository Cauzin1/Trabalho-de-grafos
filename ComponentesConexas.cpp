#include "ComponentesConexas.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>

// retorna vizinhos de v ignorando direção das arestas (para componentes fracamente conexas)
static std::vector<int> vizinhosBidirecionais(const MeuGrafo& grafo, int v) {
    std::vector<int> resultado = grafo.listarVizinhos(v);
    if (!grafo.ehDirecionado()) return resultado;

    for (int u : grafo.obterVertices()) {
        if (u == v) continue;
        if (grafo.verificarAresta(u, v)) {
            if (std::find(resultado.begin(), resultado.end(), u) == resultado.end()) {
                resultado.push_back(u);
            }
        }
    }
    return resultado;
}

std::vector<std::vector<int>> ComponentesConexas::encontrar(const MeuGrafo& grafo) {
    std::vector<std::vector<int>> componentes;
    std::set<int> visitados;
    std::vector<int> vertices = grafo.obterVertices();

    for (int v : vertices) {
        if (visitados.count(v)) continue;

        // BFS a partir de v
        std::vector<int> componente;
        std::queue<int> fila;

        fila.push(v);
        visitados.insert(v);

        while (!fila.empty()) {
            int atual = fila.front();
            fila.pop();
            componente.push_back(atual);

            for (int viz : vizinhosBidirecionais(grafo, atual)) {
                if (!visitados.count(viz)) {
                    visitados.insert(viz);
                    fila.push(viz);
                }
            }
        }

        std::sort(componente.begin(), componente.end());
        componentes.push_back(componente);
    }
    return componentes;
}

// DFS iterativa no grafo original; empilha vértices em ordem de finalização
static void dfsFinalizacao(const MeuGrafo& grafo, int v,
                           std::unordered_set<int>& visitados,
                           std::stack<int>& pilhaFinalizacao) {
    std::stack<std::pair<int, size_t>> pilha;
    std::unordered_map<int, std::vector<int>> cacheVizinhos;

    pilha.push({v, 0});
    visitados.insert(v);

    while (!pilha.empty()) {
        auto& topo = pilha.top();
        int u = topo.first;

        if (cacheVizinhos.find(u) == cacheVizinhos.end()) {
            cacheVizinhos[u] = grafo.listarVizinhos(u);
        }
        auto& vizinhos = cacheVizinhos[u];

        if (topo.second < vizinhos.size()) {
            int w = vizinhos[topo.second++];
            if (!visitados.count(w)) {
                visitados.insert(w);
                pilha.push({w, 0});
            }
        } else {
            pilhaFinalizacao.push(u);
            pilha.pop();
        }
    }
}

// DFS no grafo transposto (percorre arestas de entrada do grafo original)
static void dfsTransposto(const MeuGrafo& grafo, int v,
                          std::unordered_set<int>& visitados,
                          std::vector<int>& componente) {
    std::stack<int> pilha;
    pilha.push(v);
    visitados.insert(v);

    while (!pilha.empty()) {
        int u = pilha.top();
        pilha.pop();
        componente.push_back(u);

        for (int w : grafo.obterVertices()) {
            if (visitados.count(w)) continue;
            if (grafo.verificarAresta(w, u)) {   // w->u no original = u->w no transposto
                visitados.insert(w);
                pilha.push(w);
            }
        }
    }
}

// Kosaraju:
//   1. DFS no grafo original, empilhando por ordem de finalização
//   2. DFS no transposto na ordem inversa de finalização
std::vector<std::vector<int>> ComponentesConexas::encontrarFortementeConexos(const MeuGrafo& grafo) {
    if (!grafo.ehDirecionado()) return encontrar(grafo);

    std::vector<std::vector<int>> componentes;
    std::vector<int> vertices = grafo.obterVertices();

    std::stack<int> pilhaFinalizacao;
    std::unordered_set<int> visitados;
    for (int v : vertices) {
        if (!visitados.count(v)) {
            dfsFinalizacao(grafo, v, visitados, pilhaFinalizacao);
        }
    }

    visitados.clear();
    while (!pilhaFinalizacao.empty()) {
        int v = pilhaFinalizacao.top();
        pilhaFinalizacao.pop();

        if (visitados.count(v)) continue;

        std::vector<int> componente;
        dfsTransposto(grafo, v, visitados, componente);
        std::sort(componente.begin(), componente.end());
        componentes.push_back(componente);
    }
    return componentes;
}

void ComponentesConexas::imprimir(const std::vector<std::vector<int>>& componentes,
                                  const std::string& tipo) {
    std::cout << "\n+-----------------------------------------------------+\n";
    std::cout << "  " << tipo << "\n";
    std::cout << "  Total de componentes: " << componentes.size() << "\n";
    std::cout << "+-----------------------------------------------------+\n";

    for (size_t i = 0; i < componentes.size(); ++i) {
        std::cout << "  Componente " << (i + 1)
                  << " (" << componentes[i].size() << " vertices): { ";
        for (size_t j = 0; j < componentes[i].size(); ++j) {
            std::cout << componentes[i][j];
            if (j + 1 < componentes[i].size()) std::cout << ", ";
        }
        std::cout << " }\n";
    }
    std::cout << "+-----------------------------------------------------+\n";
}
