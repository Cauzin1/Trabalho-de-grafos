/**
 * ============================================================================
 *  ComponentesConexas.h
 *  Algoritmo de Componentes Conexas para a classe MeuGrafo.
 *
 *  Implementa três variantes para cobrir todos os tipos de grafo:
 *
 *    1. encontrar()              -> Componentes conexas (BFS)
 *         Para grafos NÃO-ORIENTADOS: componentes conexas clássicos.
 *         Para grafos ORIENTADOS    : componentes FRACAMENTE conexos
 *                                     (o algoritmo trata o grafo como se
 *                                     não-orientado).
 *
 *    2. encontrarFortementeConexos() -> Algoritmo de Kosaraju
 *         Aplicável apenas a grafos orientados.
 *
 *    3. imprimir() -> exibe os componentes encontrados de forma organizada.
 * ============================================================================
 */
#ifndef COMPONENTES_CONEXAS_H
#define COMPONENTES_CONEXAS_H

#include "Grafo.h"
#include <string>
#include <vector>

class ComponentesConexas {
public:
    /**
     * Encontra as componentes conexas do grafo usando BFS.
     *  - Grafo não-orientado: componentes conexas (definição clássica).
     *  - Grafo orientado    : componentes fracamente conexos
     *                         (ignora a direção das arestas).
     *
     * Complexidade: O(V + E)
     *
     * @return Vetor de componentes, cada componente é um vetor com os ids dos
     *         vértices que pertencem àquele componente.
     */
    static std::vector<std::vector<int>> encontrar(const MeuGrafo& grafo);

    /**
     * Encontra componentes FORTEMENTE conexos de um grafo orientado
     * usando o algoritmo de Kosaraju:
     *    1. DFS no grafo G empilhando os vértices em ordem de finalização.
     *    2. Constrói o grafo transposto G^T (todas as arestas invertidas).
     *    3. DFS em G^T retirando vértices da pilha. Cada árvore de DFS
     *       gerada é uma componente fortemente conexa.
     *
     * Complexidade: O(V + E)
     *
     * Para grafos não-orientados, retorna o mesmo que encontrar().
     */
    static std::vector<std::vector<int>> encontrarFortementeConexos(const MeuGrafo& grafo);

    /**
     * Imprime os componentes de forma organizada.
     * @param componentes Vetor de componentes (vetor de vetores de ids).
     * @param tipo Rótulo descritivo (ex.: "Componentes Conexas").
     */
    static void imprimir(const std::vector<std::vector<int>>& componentes,
                         const std::string& tipo = "Componentes Conexas");
};

#endif // COMPONENTES_CONEXAS_H
