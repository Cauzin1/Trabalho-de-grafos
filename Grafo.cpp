#include "Grafo.h"

#include <algorithm>
#include <fstream>
#include <sstream>

MeuGrafo::MeuGrafo(bool ehDirecionado, bool ehPonderado)
    : direcionado(ehDirecionado), ponderado(ehPonderado) {}

void MeuGrafo::inserirVertice(int v) {
    if (idParaIndice.find(v) != idParaIndice.end()) return;

    std::size_t novoIndice = matriz.size();

    // nova coluna em cada linha existente
    for (auto& linha : matriz) {
        linha.emplace_back();
    }
    // nova linha
    matriz.emplace_back(novoIndice + 1, Celula());

    indiceParaId.push_back(v);
    idParaIndice[v] = novoIndice;
}

void MeuGrafo::removerVertice(int v) {
    auto it = idParaIndice.find(v);
    if (it == idParaIndice.end()) return;

    std::size_t i = it->second;

    matriz.erase(matriz.begin() + i);
    for (auto& linha : matriz) {
        linha.erase(linha.begin() + i);
    }
    indiceParaId.erase(indiceParaId.begin() + i);

    // reconstrói o mapa pois todos os índices > i mudaram
    idParaIndice.clear();
    for (std::size_t k = 0; k < indiceParaId.size(); ++k) {
        idParaIndice[indiceParaId[k]] = k;
    }
}

void MeuGrafo::inserirAresta(int u, int v, double peso) {
    inserirVertice(u);
    inserirVertice(v);

    double pesoFinal = ponderado ? peso : 1.0;

    std::size_t i = idParaIndice[u];
    std::size_t j = idParaIndice[v];

    matriz[i][j].existe = true;
    matriz[i][j].peso   = pesoFinal;

    if (!direcionado && i != j) {
        matriz[j][i].existe = true;
        matriz[j][i].peso   = pesoFinal;
    }
}

void MeuGrafo::removerAresta(int u, int v) {
    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return;

    std::size_t i = itU->second;
    std::size_t j = itV->second;

    matriz[i][j].existe = false;
    matriz[i][j].peso   = 0.0;

    if (!direcionado && i != j) {
        matriz[j][i].existe = false;
        matriz[j][i].peso   = 0.0;
    }
}

bool MeuGrafo::verificarAresta(int u, int v) const {
    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return false;
    return matriz[itU->second][itV->second].existe;
}

void MeuGrafo::alterarPesoAresta(int u, int v, double peso) {
    if (!ponderado) return;

    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return;

    std::size_t i = itU->second;
    std::size_t j = itV->second;

    if (matriz[i][j].existe) {
        matriz[i][j].peso = peso;
        if (!direcionado && i != j) {
            matriz[j][i].peso = peso;
        }
    }
}

void MeuGrafo::exibirGrafo() const {
    const std::size_t n = matriz.size();

    std::vector<int> idsOrdenados = indiceParaId;
    std::sort(idsOrdenados.begin(), idsOrdenados.end());

    // vértices isolados primeiro
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
        if (isolado) std::cout << id << "\n";
    }

    // arestas; em não-orientado cada aresta aparece uma vez (u <= v)
    for (int idU : idsOrdenados) {
        std::size_t i = idParaIndice.at(idU);
        for (int idV : idsOrdenados) {
            std::size_t j = idParaIndice.at(idV);
            if (!matriz[i][j].existe) continue;
            if (!direcionado && idU > idV) continue;

            std::cout << idU << " " << idV;
            if (ponderado) std::cout << " " << matriz[i][j].peso;
            std::cout << "\n";
        }
    }
}

int MeuGrafo::grauVertice(int v) const {
    auto it = idParaIndice.find(v);
    if (it == idParaIndice.end()) return 0;

    std::size_t i = it->second;
    const std::size_t n = matriz.size();
    int grau = 0;

    for (std::size_t k = 0; k < n; ++k) {
        if (matriz[i][k].existe) ++grau;
    }
    if (direcionado) {
        // soma grau de entrada
        for (std::size_t k = 0; k < n; ++k) {
            if (matriz[k][i].existe) ++grau;
        }
    }
    return grau;
}

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

bool MeuGrafo::saoAdjacentes(int u, int v) const {
    return verificarAresta(u, v);
}

bool MeuGrafo::existeVertice(int v) const {
    return idParaIndice.find(v) != idParaIndice.end();
}

int MeuGrafo::numVertices() const {
    return static_cast<int>(indiceParaId.size());
}

int MeuGrafo::numArestas() const {
    const std::size_t n = matriz.size();
    int total = 0;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (matriz[i][j].existe) ++total;
        }
    }
    // não-orientado: cada aresta aparece duas vezes na matriz simétrica
    return direcionado ? total : total / 2;
}

std::vector<int> MeuGrafo::obterVertices() const {
    std::vector<int> v = indiceParaId;
    std::sort(v.begin(), v.end());
    return v;
}

double MeuGrafo::obterPeso(int u, int v) const {
    auto itU = idParaIndice.find(u);
    auto itV = idParaIndice.find(v);
    if (itU == idParaIndice.end() || itV == idParaIndice.end()) return 0.0;
    const Celula& c = matriz[itU->second][itV->second];
    return c.existe ? c.peso : 0.0;
}

bool MeuGrafo::carregarDeArquivo(const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir o arquivo '"
                  << nomeArquivo << "'\n";
        return false;
    }

    indiceParaId.clear();
    idParaIndice.clear();
    matriz.clear();

    std::string linha;
    int numLinha = 0;
    while (std::getline(arquivo, linha)) {
        ++numLinha;
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
                inserirVertice(u);   // vértice isolado
            }
        } else {
            std::cerr << "Aviso: linha " << numLinha
                      << " com formato invalido: '" << linha << "'\n";
        }
    }

    arquivo.close();
    return true;
}

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
