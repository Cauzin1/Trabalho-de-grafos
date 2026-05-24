/**
 * ============================================================================
 *  ComponentesConexas.cpp
 *  Implementação do algoritmo de Componentes Conexas.
 *
 *  Abordagem:
 *   - Mantemos um conjunto (std::set) de vértices já visitados.
 *   - Para cada vértice ainda não visitado, executamos BFS e
 *     coletamos todos os vértices alcançáveis -> um componente.
 *   - Em grafos orientados, para componentes FRACAMENTE conexos,
 *     percorremos as arestas em ambos os sentidos (vizinhos de saída
 *     e vizinhos de entrada).
 *
 *  Para componentes FORTEMENTE conexos usamos Kosaraju, que faz duas
 *  passagens de DFS – uma no grafo original e outra no transposto.
 * ============================================================================
 */
#include "ComponentesConexas.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>

// ----------------------------------------------------------------------------
// Função auxiliar: para um grafo ORIENTADO, retorna o "conjunto de vizinhos
// não-direcionais" do vértice v – ou seja, todos os vértices conectados a v
// independentemente da direção da aresta. Usado para componentes fracamente
// conexos.
// ----------------------------------------------------------------------------
static std::vector<int> vizinhosBidirecionais(const MeuGrafo& grafo, int v) {
    std::vector<int> resultado = grafo.listarVizinhos(v);
    if (!grafo.ehDirecionado()) return resultado;

    // Em grafos orientados, também precisamos dos vértices que apontam para v
    for (int u : grafo.obterVertices()) {
        if (u == v) continue;
        if (grafo.verificarAresta(u, v)) {
            // evita duplicação caso v->u também exista
            if (std::find(resultado.begin(), resultado.end(), u) == resultado.end()) {
                resultado.push_back(u);
            }
        }
    }
    return resultado;
}

// ----------------------------------------------------------------------------
// encontrar()
//   - Não-orientado: componentes conexos clássicos (BFS).
//   - Orientado    : componentes fracamente conexos.
// ----------------------------------------------------------------------------
std::vector<std::vector<int>> ComponentesConexas::encontrar(const MeuGrafo& grafo) {
    std::vector<std::vector<int>> componentes;
    std::set<int> visitados;
    std::vector<int> vertices = grafo.obterVertices();

    for (int v : vertices) {
        if (visitados.count(v)) continue;

        // -------- BFS a partir de v --------
        std::vector<int> componente;
        std::queue<int> fila;

        fila.push(v);
        visitados.insert(v);

        while (!fila.empty()) {
            int atual = fila.front();
            fila.pop();
            componente.push_back(atual);

            // Vizinhos (tratando arestas como não-direcionais se preciso)
            std::vector<int> vizinhos = vizinhosBidirecionais(grafo, atual);
            for (int viz : vizinhos) {
                if (!visitados.count(viz)) {
                    visitados.insert(viz);
                    fila.push(viz);
                }
            }
        }

        // Ordena para apresentação consistente
        std::sort(componente.begin(), componente.end());
        componentes.push_back(componente);
    }
    return componentes;
}

// ----------------------------------------------------------------------------
// Helpers para Kosaraju
// ----------------------------------------------------------------------------

/** DFS iterativa no grafo original, empilhando em ordem de finalização. */
static void dfsFinalizacao(const MeuGrafo& grafo, int v,
                           std::unordered_set<int>& visitados,
                           std::stack<int>& pilhaFinalizacao) {
    // DFS iterativa com pilha de estados (vértice, índice do próximo vizinho)
    std::stack<std::pair<int, size_t>> pilha;
    std::unordered_map<int, std::vector<int>> cacheVizinhos;

    pilha.push({v, 0});
    visitados.insert(v);

    while (!pilha.empty()) {
        auto& topo = pilha.top();
        int u = topo.first;

        // Carrega vizinhos do vértice apenas uma vez
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
            pilhaFinalizacao.push(u);   // finaliza vértice
            pilha.pop();
        }
    }
}

/** DFS no grafo transposto (usando arestas de entrada do grafo original). */
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

        // No grafo transposto, vizinhos de u são os vértices w com w->u no original
        for (int w : grafo.obterVertices()) {
            if (visitados.count(w)) continue;
            if (grafo.verificarAresta(w, u)) {
                visitados.insert(w);
                pilha.push(w);
            }
        }
    }
}

// ----------------------------------------------------------------------------
// encontrarFortementeConexos() - Algoritmo de Kosaraju
// ----------------------------------------------------------------------------
std::vector<std::vector<int>> ComponentesConexas::encontrarFortementeConexos(const MeuGrafo& grafo) {
    // Para grafo não-orientado, a noção de fortemente conexo coincide com a de conexo.
    if (!grafo.ehDirecionado()) return encontrar(grafo);

    std::vector<std::vector<int>> componentes;
    std::vector<int> vertices = grafo.obterVertices();

    // ---- Etapa 1: DFS no grafo original, empilhando por ordem de finalização ----
    std::stack<int> pilhaFinalizacao;
    std::unordered_set<int> visitados;
    for (int v : vertices) {
        if (!visitados.count(v)) {
            dfsFinalizacao(grafo, v, visitados, pilhaFinalizacao);
        }
    }

    // ---- Etapa 2: DFS no grafo transposto, em ordem decrescente de finalização ----
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

// ----------------------------------------------------------------------------
// imprimir()
// ----------------------------------------------------------------------------
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
