/**
 * ============================================================================
 *  Grafo.h
 *  DCC059 - Teoria dos Grafos - Trabalho Prático 1
 *  Tema: Componentes Conexas
 * ----------------------------------------------------------------------------
 *  Classe MeuGrafo: representa um grafo usando MATRIZ DE ADJACÊNCIA.
 *
 *  Características suportadas:
 *      - Orientado ou não-orientado (definido no construtor)
 *      - Ponderado ou não-ponderado (padrão: ponderado)
 *
 *  Como os IDs de vértices podem ser arbitrários (ex.: 0, 5, 99) e não
 *  precisam ser contíguos, mantemos um MAPEAMENTO entre o ID do vértice
 *  e o índice da linha/coluna na matriz:
 *
 *      indiceParaId[i]   -> id do vértice na linha/coluna i da matriz
 *      idParaIndice[id]  -> índice (linha/coluna) na matriz
 *
 *  Cada célula da matriz guarda dois campos:
 *      existe -> indica se há aresta entre os vértices i e j
 *      peso   -> valor do peso (válido apenas se 'existe' for true)
 * ============================================================================
 */
#ifndef GRAFO_H
#define GRAFO_H

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class MeuGrafo {
private:
    /**
     * Célula da matriz de adjacência.
     *  existe : true se há aresta entre os vértices das linhas/colunas
     *  peso   : peso da aresta (0.0 se a aresta não existe)
     */
    struct Celula {
        bool   existe;
        double peso;
        Celula() : existe(false), peso(0.0) {}
    };

    bool direcionado;   // true = grafo orientado;   false = grafo não-orientado
    bool ponderado;     // true = arestas com peso;  false = arestas com peso 1.0

    // Mapeamento bidirecional entre IDs de vértices e índices da matriz
    std::vector<int>           indiceParaId;   // índice -> id do vértice
    std::map<int, std::size_t> idParaIndice;   // id do vértice -> índice

    // Matriz de adjacência (n x n, onde n = indiceParaId.size())
    // Em grafos não-orientados a matriz é simétrica:
    //   matriz[i][j].existe == matriz[j][i].existe
    //   matriz[i][j].peso   == matriz[j][i].peso
    std::vector<std::vector<Celula>> matriz;

public:
    // ============================================================
    //                    CONSTRUTOR / DESTRUTOR
    // ============================================================

    /**
     * Cria um grafo vazio.
     * @param ehDirecionado true para grafo orientado, false caso contrário
     * @param ehPonderado   true para grafo ponderado (padrão), false caso contrário
     */
    MeuGrafo(bool ehDirecionado, bool ehPonderado = true);
    ~MeuGrafo() = default;

    // ============================================================
    //                  OPERAÇÕES DE MANIPULAÇÃO
    // ============================================================

    /** Insere um vértice no grafo. Se já existir, não faz nada. */
    void inserirVertice(int v);

    /** Remove um vértice e TODAS as arestas incidentes a ele. */
    void removerVertice(int v);

    /**
     * Insere uma aresta entre u e v com o peso informado.
     * - Cria os vértices automaticamente caso não existam.
     * - Se a aresta já existir, atualiza o peso (não duplica).
     * - Em grafo não-orientado, espelha em matriz[j][i].
     */
    void inserirAresta(int u, int v, double peso = 1.0);

    /** Remove a aresta entre u e v (ambas as direções, se não-orientado). */
    void removerAresta(int u, int v);

    /** Verifica se existe aresta entre u e v. Não lança exceção para vértices inválidos. */
    bool verificarAresta(int u, int v) const;

    /** Altera o peso de uma aresta existente. Não faz nada se a aresta não existir. */
    void alterarPesoAresta(int u, int v, double peso);

    /** Exibe o grafo em formato compatível com csacademy.com/app/graph_editor/ */
    void exibirGrafo() const;

    // ============================================================
    //                INFORMAÇÕES SOBRE VÉRTICES
    // ============================================================

    /**
     * Calcula o grau do vértice.
     *  - Grafo não-orientado: número de arestas incidentes.
     *  - Grafo orientado    : grau de saída + grau de entrada.
     * Retorna 0 se o vértice não existir.
     */
    int grauVertice(int v) const;

    /** Lista os vizinhos (vértices de destino das arestas que saem de v). */
    std::vector<int> listarVizinhos(int v) const;

    /** Verifica se existe a aresta direta u -> v (mesma semântica de verificarAresta). */
    bool saoAdjacentes(int u, int v) const;

    // ============================================================
    //                    UTILITÁRIOS / GETTERS
    // ============================================================

    bool existeVertice(int v) const;
    int  numVertices() const;
    int  numArestas() const;
    std::vector<int> obterVertices() const;     // retornado em ordem crescente
    bool ehDirecionado() const { return direcionado; }
    bool ehPonderado()  const { return ponderado;  }

    /** Retorna o peso da aresta (u,v). Retorna 0.0 se a aresta não existir. */
    double obterPeso(int u, int v) const;

    // ============================================================
    //                    ENTRADA E SAÍDA EM ARQUIVO
    // ============================================================

    /**
     * Carrega grafo a partir de arquivo no formato CS Academy.
     * Cada linha pode ser:
     *    - um único inteiro  -> vértice isolado
     *    - dois inteiros     -> aresta não ponderada u v
     *    - dois inteiros + um double -> aresta ponderada u v peso
     * Linhas em branco e linhas começando com '#' são ignoradas.
     */
    bool carregarDeArquivo(const std::string& nomeArquivo);

    /** Salva o grafo em arquivo no formato CS Academy. */
    bool salvarEmArquivo(const std::string& nomeArquivo) const;
};

#endif // GRAFO_H
