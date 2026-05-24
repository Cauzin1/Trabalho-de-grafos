/**
 * ============================================================================
 *  main.cpp
 *  Programa interativo para uso da biblioteca de grafos.
 *
 *  DCC059 - Teoria dos Grafos - Trabalho Prático 1
 *  Tema: Componentes Conexas
 *
 *  Permite:
 *    - Criar grafo (orientado ou não, ponderado ou não)
 *    - Carregar / salvar grafos em arquivo (formato CS Academy)
 *    - Manipular vértices e arestas
 *    - Consultar informações do grafo (grau, vizinhos, adjacência)
 *    - Executar o algoritmo de Componentes Conexas
 *    - Executar o algoritmo de Componentes Fortemente Conexas (Kosaraju)
 * ============================================================================
 */
#include "ComponentesConexas.h"
#include "Grafo.h"

#include <iostream>
#include <limits>
#include <memory>
#include <string>

// Limpa o buffer de entrada após leitura numérica
static void limparBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Lê um inteiro do usuário com validação simples
static int lerInt(const std::string& prompt) {
    int valor;
    while (true) {
        std::cout << prompt;
        if (std::cin >> valor) {
            limparBuffer();
            return valor;
        }
        std::cout << "  Entrada invalida. Tente novamente.\n";
        std::cin.clear();
        limparBuffer();
    }
}

// Lê um double do usuário com validação simples
static double lerDouble(const std::string& prompt) {
    double valor;
    while (true) {
        std::cout << prompt;
        if (std::cin >> valor) {
            limparBuffer();
            return valor;
        }
        std::cout << "  Entrada invalida. Tente novamente.\n";
        std::cin.clear();
        limparBuffer();
    }
}

// Lê uma linha de string
static std::string lerString(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}

// Lê uma resposta s/n (case-insensitive)
static bool lerSimNao(const std::string& prompt) {
    std::string r = lerString(prompt);
    if (r.empty()) return false;
    return (r[0] == 's' || r[0] == 'S' || r[0] == 'y' || r[0] == 'Y');
}

// Exibe o cabeçalho com as configurações atuais do grafo
static void exibirCabecalho(const MeuGrafo& g) {
    std::cout << "\n+============================================================+\n";
    std::cout << "  Grafo atual: "
              << (g.ehDirecionado() ? "ORIENTADO" : "NAO-ORIENTADO")
              << " | "
              << (g.ehPonderado() ? "PONDERADO" : "NAO-PONDERADO")
              << "\n";
    std::cout << "  Vertices: " << g.numVertices()
              << " | Arestas: " << g.numArestas() << "\n";
    std::cout << "+============================================================+\n";
}

// Exibe o menu principal
static void exibirMenu() {
    std::cout << "\n+--------------- MENU ---------------+\n";
    std::cout << "| -- Manipulacao do Grafo --        |\n";
    std::cout << "|  1) Inserir vertice                |\n";
    std::cout << "|  2) Remover vertice                |\n";
    std::cout << "|  3) Inserir aresta                 |\n";
    std::cout << "|  4) Remover aresta                 |\n";
    std::cout << "|  5) Alterar peso de aresta         |\n";
    std::cout << "|  6) Exibir grafo                   |\n";
    std::cout << "| -- Consultas --                    |\n";
    std::cout << "|  7) Verificar existencia de aresta |\n";
    std::cout << "|  8) Calcular grau de um vertice    |\n";
    std::cout << "|  9) Listar vizinhos de um vertice  |\n";
    std::cout << "| 10) Verificar adjacencia           |\n";
    std::cout << "| -- Algoritmo: Componentes Conexas -|\n";
    std::cout << "| 11) Encontrar componentes conexas  |\n";
    std::cout << "| 12) Componentes fortemente conexas |\n";
    std::cout << "| -- Arquivo --                      |\n";
    std::cout << "| 13) Carregar grafo de arquivo      |\n";
    std::cout << "| 14) Salvar grafo em arquivo        |\n";
    std::cout << "| -- Outros --                       |\n";
    std::cout << "| 15) Criar novo grafo (reset)       |\n";
    std::cout << "|  0) Sair                           |\n";
    std::cout << "+------------------------------------+\n";
}

// Cria um novo grafo perguntando ao usuário as configurações
static std::unique_ptr<MeuGrafo> criarNovoGrafo() {
    std::cout << "\n--- Criacao de Novo Grafo ---\n";
    bool dir = lerSimNao("O grafo eh orientado? (s/n): ");
    bool pond = lerSimNao("O grafo eh ponderado? (s/n): ");
    std::cout << "Grafo criado: "
              << (dir ? "Orientado" : "Nao-orientado") << " e "
              << (pond ? "ponderado" : "nao-ponderado") << ".\n";
    return std::make_unique<MeuGrafo>(dir, pond);
}

// ============================================================================
// MAIN
// ============================================================================
int main() {
    std::cout << "============================================================\n";
    std::cout << "   TRABALHO PRATICO 1 - DCC059 (Teoria dos Grafos)\n";
    std::cout << "   Tema: COMPONENTES CONEXAS\n";
    std::cout << "   Estrutura: Matriz de Adjacencia\n";
    std::cout << "============================================================\n";

    std::unique_ptr<MeuGrafo> grafo = criarNovoGrafo();

    int opcao = -1;
    while (opcao != 0) {
        exibirCabecalho(*grafo);
        exibirMenu();
        opcao = lerInt("Escolha uma opcao: ");

        switch (opcao) {
            case 1: { // Inserir vertice
                int v = lerInt("  Id do vertice a inserir: ");
                grafo->inserirVertice(v);
                std::cout << "  Vertice " << v << " inserido.\n";
                break;
            }
            case 2: { // Remover vertice
                int v = lerInt("  Id do vertice a remover: ");
                if (!grafo->existeVertice(v)) {
                    std::cout << "  Vertice " << v << " nao existe.\n";
                } else {
                    grafo->removerVertice(v);
                    std::cout << "  Vertice " << v << " removido.\n";
                }
                break;
            }
            case 3: { // Inserir aresta
                int u = lerInt("  Vertice origem (u): ");
                int v = lerInt("  Vertice destino (v): ");
                double p = 1.0;
                if (grafo->ehPonderado()) {
                    p = lerDouble("  Peso da aresta: ");
                }
                grafo->inserirAresta(u, v, p);
                std::cout << "  Aresta (" << u << " -> " << v << ") inserida"
                          << (grafo->ehPonderado() ? (" com peso " + std::to_string(p)) : "")
                          << ".\n";
                break;
            }
            case 4: { // Remover aresta
                int u = lerInt("  Vertice origem (u): ");
                int v = lerInt("  Vertice destino (v): ");
                grafo->removerAresta(u, v);
                std::cout << "  Aresta (" << u << ", " << v << ") removida (se existia).\n";
                break;
            }
            case 5: { // Alterar peso
                if (!grafo->ehPonderado()) {
                    std::cout << "  Grafo nao-ponderado: operacao nao aplicavel.\n";
                    break;
                }
                int u = lerInt("  Vertice origem (u): ");
                int v = lerInt("  Vertice destino (v): ");
                if (!grafo->verificarAresta(u, v)) {
                    std::cout << "  Aresta (" << u << ", " << v << ") nao existe.\n";
                    break;
                }
                double p = lerDouble("  Novo peso: ");
                grafo->alterarPesoAresta(u, v, p);
                std::cout << "  Peso atualizado.\n";
                break;
            }
            case 6: { // Exibir grafo
                std::cout << "\n--- Grafo (formato CS Academy) ---\n";
                grafo->exibirGrafo();
                std::cout << "----------------------------------\n";
                break;
            }
            case 7: { // Verificar aresta
                int u = lerInt("  Vertice origem (u): ");
                int v = lerInt("  Vertice destino (v): ");
                bool existe = grafo->verificarAresta(u, v);
                std::cout << "  Aresta (" << u << " -> " << v << ") existe? "
                          << (existe ? "SIM" : "NAO") << "\n";
                if (existe && grafo->ehPonderado()) {
                    std::cout << "  Peso: " << grafo->obterPeso(u, v) << "\n";
                }
                break;
            }
            case 8: { // Grau
                int v = lerInt("  Id do vertice: ");
                std::cout << "  Grau do vertice " << v << ": "
                          << grafo->grauVertice(v) << "\n";
                break;
            }
            case 9: { // Listar vizinhos
                int v = lerInt("  Id do vertice: ");
                std::vector<int> vz = grafo->listarVizinhos(v);
                std::cout << "  Vizinhos de " << v << ": [";
                for (size_t i = 0; i < vz.size(); ++i) {
                    std::cout << vz[i] << (i + 1 < vz.size() ? ", " : "");
                }
                std::cout << "]\n";
                break;
            }
            case 10: { // Adjacencia
                int u = lerInt("  Vertice u: ");
                int v = lerInt("  Vertice v: ");
                std::cout << "  saoAdjacentes(" << u << ", " << v << ")? "
                          << (grafo->saoAdjacentes(u, v) ? "SIM" : "NAO") << "\n";
                break;
            }
            case 11: { // Componentes conexas
                auto comp = ComponentesConexas::encontrar(*grafo);
                std::string titulo = grafo->ehDirecionado()
                    ? "Componentes Fracamente Conexos"
                    : "Componentes Conexas";
                ComponentesConexas::imprimir(comp, titulo);
                break;
            }
            case 12: { // Componentes fortemente conexas
                if (!grafo->ehDirecionado()) {
                    std::cout << "  Para grafos nao-orientados, componentes fortemente\n"
                              << "  conexos coincidem com componentes conexos.\n";
                }
                auto comp = ComponentesConexas::encontrarFortementeConexos(*grafo);
                ComponentesConexas::imprimir(comp, "Componentes Fortemente Conexos (Kosaraju)");
                break;
            }
            case 13: { // Carregar de arquivo
                std::string caminho = lerString("  Caminho do arquivo: ");
                if (grafo->carregarDeArquivo(caminho)) {
                    std::cout << "  Grafo carregado com sucesso.\n";
                }
                break;
            }
            case 14: { // Salvar em arquivo
                std::string caminho = lerString("  Caminho do arquivo: ");
                if (grafo->salvarEmArquivo(caminho)) {
                    std::cout << "  Grafo salvo em '" << caminho << "'.\n";
                }
                break;
            }
            case 15: { // Reset
                grafo = criarNovoGrafo();
                break;
            }
            case 0: {
                std::cout << "Encerrando...\n";
                break;
            }
            default:
                std::cout << "  Opcao invalida.\n";
                break;
        }
    }
    return 0;
}
