// Grafo.h - DCC059 Trabalho Prático 1
// Representa um grafo com matriz de adjacência.
// Suporta grafos orientados/não-orientados e ponderados/não-ponderados.
//
// IDs de vértices podem ser arbitrários (não precisam ser contíguos).
// Por isso usamos um mapeamento bidirecional entre ID e índice da matriz:
//   indiceParaId[i]  -> id do vértice na posição i
//   idParaIndice[id] -> índice na matriz

#ifndef GRAFO_H
#define GRAFO_H

#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class MeuGrafo {
private:
    // célula da matriz: guarda se a aresta existe e seu peso
    struct Celula {
        bool   existe;
        double peso;
        Celula() : existe(false), peso(0.0) {}
    };

    bool direcionado;
    bool ponderado;

    std::vector<int>           indiceParaId;
    std::map<int, std::size_t> idParaIndice;

    // matriz n x n; em grafos não-orientados é simétrica
    std::vector<std::vector<Celula>> matriz;

public:
    MeuGrafo(bool ehDirecionado, bool ehPonderado = true);
    ~MeuGrafo() = default;

    void inserirVertice(int v);
    void removerVertice(int v);   // remove também todas as arestas incidentes
    void inserirAresta(int u, int v, double peso = 1.0);
    void removerAresta(int u, int v);
    bool verificarAresta(int u, int v) const;
    void alterarPesoAresta(int u, int v, double peso);
    void exibirGrafo() const;   // formato CS Academy

    // grau de saída + entrada em orientado; total de incidentes em não-orientado
    int grauVertice(int v) const;
    std::vector<int> listarVizinhos(int v) const;
    bool saoAdjacentes(int u, int v) const;

    bool existeVertice(int v) const;
    int  numVertices() const;
    int  numArestas() const;
    std::vector<int> obterVertices() const;   // ordem crescente
    bool ehDirecionado() const { return direcionado; }
    bool ehPonderado()   const { return ponderado;  }
    double obterPeso(int u, int v) const;

    // formato CS Academy: inteiro isolado = vértice, "u v [peso]" = aresta
    bool carregarDeArquivo(const std::string& nomeArquivo);
    bool salvarEmArquivo(const std::string& nomeArquivo) const;
};

#endif // GRAFO_H
