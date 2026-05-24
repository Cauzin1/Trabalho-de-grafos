/**
 * ============================================================================
 *  Grafo.cpp
 *  Implementação da classe MeuGrafo usando MATRIZ DE ADJACÊNCIA.
 *
 *  Decisões de implementação:
 *
 *  1. Mapeamento ID <-> índice:
 *     - std::vector<int> indiceParaId : índice da matriz -> ID do vértice
 *     - std::map<int, size_t> idParaIndice : ID do vértice -> índice
 *     Esse duplo mapeamento permite suportar IDs arbitrários (não contíguos,
 *     ex.: 0, 5, 99) sem desperdiçar memória.
 *
 *  2. Matriz como std::vector<std::vector<Celula>>:
 *     - Cada Celula guarda {existe, peso}; assim suportamos pesos 0 ou
 *       negativos sem ambiguidade (ao contrário de usar 0 como "sem aresta").
 *
 *  3. Remoção real de linha/coluna:
 *     - Quando um vértice é removido, sua linha e coluna são DE FATO
 *       eliminadas (não marcadas como "removidas"). Isso mantém a matriz
 *       sempre compacta e simétrica em ordem com os índices.
 *     - Custo: O(n^2) por remoção, aceitável para o porte de grafos
 *       didáticos. Em compensação, simplifica todas as outras operações.
 *
 *  4. Tolerância a entradas inválidas:
 *     - Todas as consultas (verificarAresta, grauVertice, listarVizinhos,
 *       saoAdjacentes) tratam vértices inexistentes sem lançar exceção.
 * ============================================================================
 */
#include "Grafo.h"

#include <algorithm>
#include <fstream>
#include <sstream>

// ----------------------------------------------------------------------------
// CONSTRUTOR
// ----------------------------------------------------------------------------
MeuGrafo::MeuGrafo(bool ehDirecionado, bool ehPonderado)
    : direcionado(ehDirecionado), ponderado(ehPonderado) {}

// ----------------------------------------------------------------------------
// inserirVertice
//   Adiciona uma nova linha (vazia) ao final da matriz e uma nova coluna
//   (vazia) em todas as linhas existentes (incluindo a nova).
//   Complexidade: O(n) — n = número de vértices anteriores.
// ----------------------------------------------------------------------------
void MeuGrafo::inserirVertice(int v) {
    // Se o vértice já existir, não faz nada (idempotente)
    if (idParaIndice.find(v) != idParaIndice.end()) return;

    std::size_t novoIndice = matriz.size();

    // Adiciona uma nova coluna em cada linha existente
    for (auto& linha : matriz) {
        linha.emplace_back();   // Celula() = {false, 0.0}
    }
    // Adiciona uma nova linha de tamanho (novoIndice + 1)
    matriz.emplace_back(novoIndice + 1, Celula());

    indiceParaId.push_back(v);
    idParaIndice[v] = novoIndice;
}

// ----------------------------------------------------------------------------
// removerVertice
//   Remove a linha i e a coluna i da matriz, depois reconstrói o mapeamento
//   idParaIndice (todos os índices > i decrementam em 1).
//   Complexidade: O(n^2).
// ----------------------------------------------------------------------------
void MeuGrafo::removerVertice(int v) {
    auto it = idParaIndice.find(v);
    if (it == idParaIndice.end()) return;   // vértice inexistente

    std::size_t i = it->second;

    // 1) Remove a linha i
    matriz.erase(matriz.begin() + i);
    // 2) Remove a coluna i em cada linha restante
    for (auto& linha : matriz) {
        linha.erase(linha.begin() + i);
    }
    // 3) Remove do vetor de IDs
    indiceParaId.erase(indiceParaId.begin() + i);

    // 4) Reconstrói o mapa de índices
    idParaIndice.clear();
    for (std::size_t k = 0; k < indiceParaId.size(); ++k) {
        idParaIndice[indiceParaId[k]] = k;
    }
}

// ----------------------------------------------------------------------------
// inserirAresta
//   Cria os vértices se não existirem; se a aresta já existir, atualiza
//   o peso (evita duplicação). Em grafos não-orientados, espelha em [j][i].
// ----------------------------------------------------------------------------
void MeuGrafo::inserirAresta(int u, int v, double peso) {
    inserirVertice(u);
    inserirVertice(v);

    double pesoFinal = ponderado ? peso : 1.0;

    std::size_t i = idParaIndice[u];
    std::size_t j = idParaIndice[v];

    matriz[i][j].existe = true;
    matriz[i][j].peso   = pesoFinal;

    // Em grafo não-orientado a matriz é simétrica
    if (!direcionado && i != j) {
        matriz[j][i].existe = true;
        matriz[j][i].peso   = pesoFinal;
    }
}

// ----------------------------------------------------------------------------
// removerAresta
// ----------------------------------------------------------------------------
void MeuGrafo::removerAresta(int u, int v) {
    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return;

    std::size_t i = itU->second;
    std::size_t j = itV->second;

    matriz[i][j].existe = false;
    matriz[i][j].peso   = 0.0;

    // Em grafo não-orientado também remove o sentido contrário
    if (!direcionado && i != j) {
        matriz[j][i].existe = false;
        matriz[j][i].peso   = 0.0;
    }
}

// ----------------------------------------------------------------------------
// verificarAresta
//   Em matriz de adjacência, esta é a operação MAIS RÁPIDA: O(1) após
//   resolver os índices.
// ----------------------------------------------------------------------------
bool MeuGrafo::verificarAresta(int u, int v) const {
    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return false;
    return matriz[itU->second][itV->second].existe;
}

// ----------------------------------------------------------------------------
// alterarPesoAresta
// ----------------------------------------------------------------------------
void MeuGrafo::alterarPesoAresta(int u, int v, double peso) {
    if (!ponderado) return;     // não faz sentido em grafo não-ponderado

    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return;

    std::size_t i = itU->second;
    std::size_t j = itV->second;

    // Só altera se a aresta de fato existe
    if (matriz[i][j].existe) {
        matriz[i][j].peso = peso;
        if (!direcionado && i != j) {
            matriz[j][i].peso = peso;
        }
    }
}

// ----------------------------------------------------------------------------
// exibirGrafo
//   Formato compatível com https://csacademy.com/app/graph_editor/
//     - vértices isolados aparecem sozinhos em uma linha
//     - arestas aparecem como "u v" ou "u v peso"
//     - em grafos não-orientados cada aresta é listada UMA vez (u <= v)
//
//   Para impressão determinística, processamos os vértices em ordem
//   crescente de ID (não na ordem de inserção da matriz).
// ----------------------------------------------------------------------------
void MeuGrafo::exibirGrafo() const {
    const std::size_t n = matriz.size();

    // Constrói lista de IDs em ordem crescente
    std::vector<int> idsOrdenados = indiceParaId;
    std::sort(idsOrdenados.begin(), idsOrdenados.end());

    // 1) Vértices isolados (sem aresta de saída e, se orientado, sem entrada)
    for (int id : idsOrdenados) {
        std::size_t i = idParaIndice.at(id);
        bool isolado = true;

        // Verifica arestas de saída
        for (std::size_t k = 0; k < n; ++k) {
            if (matriz[i][k].existe) { isolado = false; break; }
        }
        // Em grafo orientado, ainda pode haver arestas de entrada
        if (isolado && direcionado) {
            for (std::size_t k = 0; k < n; ++k) {
                if (matriz[k][i].existe) { isolado = false; break; }
            }
        }
        if (isolado) std::cout << id << "\n";
    }

    // 2) Arestas — percorre em ordem crescente de ID
    for (int idU : idsOrdenados) {
        std::size_t i = idParaIndice.at(idU);
        for (int idV : idsOrdenados) {
            std::size_t j = idParaIndice.at(idV);
            if (!matriz[i][j].existe) continue;

            // Em grafo não-orientado, imprime cada aresta uma única vez
            if (!direcionado && idU > idV) continue;

            std::cout << idU << " " << idV;
            if (ponderado) std::cout << " " << matriz[i][j].peso;
            std::cout << "\n";
        }
    }
}

// ----------------------------------------------------------------------------
// grauVertice
//   Não-orientado: conta a linha i (cada aresta incidente aparece uma vez)
//   Orientado    : grau de saída (linha i) + grau de entrada (coluna i)
//   Retorna 0 se o vértice não existir.
// ----------------------------------------------------------------------------
int MeuGrafo::grauVertice(int v) const {
    auto it = idParaIndice.find(v);
    if (it == idParaIndice.end()) return 0;

    std::size_t i = it->second;
    const std::size_t n = matriz.size();
    int grau = 0;

    // Grau de saída (linha i)
    for (std::size_t k = 0; k < n; ++k) {
        if (matriz[i][k].existe) ++grau;
    }
    if (direcionado) {
        // Soma o grau de entrada (coluna i)
        for (std::size_t k = 0; k < n; ++k) {
            if (matriz[k][i].existe) ++grau;
        }
    }
    return grau;
}

// ----------------------------------------------------------------------------
// listarVizinhos
//   Retorna os vértices alcançáveis a partir de v em UM passo (vizinhos
//   de saída). Em grafos não-orientados isso equivale ao conjunto completo
//   de vizinhos, já que a matriz é simétrica.
// ----------------------------------------------------------------------------
std::vector<int> MeuGrafo::listarVizinhos(int v) const {
    std::vector<int> vizinhos;
    auto it = idParaIndice.find(v);
    if (it == idParaIndice.end()) return vizinhos;

    std::size_t i = it->second;
    const std::size_t n = matriz.size();
    for (std::size_t k = 0; k < n; ++k) {
        if (matriz[i][k].existe) {
            vizinhos.push_back(indiceParaId[k]);
        }
    }
    return vizinhos;
}

// ----------------------------------------------------------------------------
// saoAdjacentes
// ----------------------------------------------------------------------------
bool MeuGrafo::saoAdjacentes(int u, int v) const {
    return verificarAresta(u, v);
}

// ----------------------------------------------------------------------------
// existeVertice
// ----------------------------------------------------------------------------
bool MeuGrafo::existeVertice(int v) const {
    return idParaIndice.find(v) != idParaIndice.end();
}

// ----------------------------------------------------------------------------
// numVertices
// ----------------------------------------------------------------------------
int MeuGrafo::numVertices() const {
    return static_cast<int>(indiceParaId.size());
}

// ----------------------------------------------------------------------------
// numArestas
//   Em grafo não-orientado, cada aresta é contada duas vezes na matriz
//   (simétrica), por isso dividimos por 2 ao final.
// ----------------------------------------------------------------------------
int MeuGrafo::numArestas() const {
    const std::size_t n = matriz.size();
    int total = 0;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (matriz[i][j].existe) ++total;
        }
    }
    return direcionado ? total : total / 2;
}

// ----------------------------------------------------------------------------
// obterVertices
//   Retorna os IDs dos vértices em ordem crescente (para iteração
//   determinística por parte do algoritmo de componentes conexas).
// ----------------------------------------------------------------------------
std::vector<int> MeuGrafo::obterVertices() const {
    std::vector<int> v = indiceParaId;
    std::sort(v.begin(), v.end());
    return v;
}

// ----------------------------------------------------------------------------
// obterPeso
// ----------------------------------------------------------------------------
double MeuGrafo::obterPeso(int u, int v) const {
    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return 0.0;
    const Celula& c = matriz[itU->second][itV->second];
    return c.existe ? c.peso : 0.0;
}

// ----------------------------------------------------------------------------
// carregarDeArquivo
// ----------------------------------------------------------------------------
bool MeuGrafo::carregarDeArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo '"
                  << nomeArquivo << "'\n";
        return false;
    }

    // Reseta o grafo
    indiceParaId.clear();
    idParaIndice.clear();
    matriz.clear();

    std::string linha;
    int numLinha = 0;
    while (std::getline(arquivo, linha)) {
        ++numLinha;
        // Ignora linhas em branco e comentários (começando com '#')
        if (linha.empty()) continue;
        std::size_t inicio = linha.find_first_not_of(" \t\r\n");
        if (inicio == std::string::npos) continue;
        if (linha[inicio] == '#') continue;

        std::istringstream iss(linha);
        int u, v;
        double peso;

        if (iss >> u) {
            if (iss >> v) {
                if (iss >> peso) {
                    inserirAresta(u, v, peso);
                } else {
                    inserirAresta(u, v, 1.0);
                }
            } else {
                // Apenas um número na linha -> vértice isolado
                inserirVertice(u);
            }
        } else {
            std::cerr << "Aviso: linha " << numLinha
                      << " com formato invalido: '" << linha << "'\n";
        }
    }

    arquivo.close();
    return true;
}

// ----------------------------------------------------------------------------
// salvarEmArquivo
// ----------------------------------------------------------------------------
bool MeuGrafo::salvarEmArquivo(const std::string& nomeArquivo) const {
    std::ofstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel criar o arquivo '"
                  << nomeArquivo << "'\n";
        return false;
    }

    const std::size_t n = matriz.size();
    std::vector<int> idsOrdenados = indiceParaId;
    std::sort(idsOrdenados.begin(), idsOrdenados.end());

    // Vértices isolados primeiro
    for (int id : idsOrdenados) {
        std::size_t i = idParaIndice.at(id);
        bool isolado = true;
        for (std::size_t k = 0; k < n; ++k) {
            if (matriz[i][k].existe) { isolado = false; break; }
        }
        if (isolado && direcionado) {
            for (std::size_t k = 0; k < n; ++k) {
                if (matriz[k][i].existe) { isolado = false; break; }
            }
        }
        if (isolado) arquivo << id << "\n";
    }

    // Arestas
    for (int idU : idsOrdenados) {
        std::size_t i = idParaIndice.at(idU);
        for (int idV : idsOrdenados) {
            std::size_t j = idParaIndice.at(idV);
            if (!matriz[i][j].existe) continue;
            if (!direcionado && idU > idV) continue;
            arquivo << idU << " " << idV;
            if (ponderado) arquivo << " " << matriz[i][j].peso;
            arquivo << "\n";
        }
    }

    arquivo.close();
    return true;
}
