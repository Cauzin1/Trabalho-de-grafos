/**
 * Stress tests para verificar robustez do TAD em casos atipicos
 * (NAO faz parte da entrega; é apenas verificacao).
 */
#include "Grafo.h"
#include "ComponentesConexas.h"
#include <cassert>
#include <iostream>

#define CHECK(cond, msg) do { \
    if (!(cond)) { std::cout << "  FALHOU: " << msg << "\n"; falhas++; } \
    else { std::cout << "  ok: " << msg << "\n"; } \
} while(0)

int main() {
    int falhas = 0;

    std::cout << "\n=== 1. Grafo vazio ===\n";
    {
        MeuGrafo g(false);
        CHECK(g.numVertices() == 0, "numVertices == 0");
        CHECK(g.numArestas() == 0, "numArestas == 0");
        CHECK(g.grauVertice(0) == 0, "grauVertice em vazio");
        CHECK(g.listarVizinhos(42).empty(), "listarVizinhos em vazio");
        CHECK(!g.verificarAresta(0, 1), "verificarAresta em vazio");
        g.removerVertice(99);     // nao deve crashar
        g.removerAresta(0, 1);    // nao deve crashar
        g.alterarPesoAresta(0, 1, 3.14);  // nao deve crashar
        auto comp = ComponentesConexas::encontrar(g);
        CHECK(comp.empty(), "componentes em grafo vazio");
    }

    std::cout << "\n=== 2. Self-loop ===\n";
    {
        MeuGrafo g(false);
        g.inserirAresta(5, 5, 2.5);
        CHECK(g.verificarAresta(5, 5), "self-loop existe");
        CHECK(g.obterPeso(5, 5) == 2.5, "peso do self-loop");
        g.removerAresta(5, 5);
        CHECK(!g.verificarAresta(5, 5), "self-loop removido");
    }

    std::cout << "\n=== 3. Pesos negativos e zero ===\n";
    {
        MeuGrafo g(true);
        g.inserirAresta(0, 1, -5.0);
        g.inserirAresta(1, 2, 0.0);
        CHECK(g.obterPeso(0, 1) == -5.0, "peso negativo preservado");
        CHECK(g.obterPeso(1, 2) == 0.0, "peso 0 e diferente de 'sem aresta'");
        CHECK(g.verificarAresta(1, 2), "aresta de peso 0 existe");
        CHECK(!g.verificarAresta(2, 1), "direcao inversa nao existe (dir.)");
    }

    std::cout << "\n=== 4. IDs negativos e grandes ===\n";
    {
        MeuGrafo g(false);
        g.inserirVertice(-10);
        g.inserirVertice(1000000);
        g.inserirAresta(-10, 1000000, 1.0);
        CHECK(g.numVertices() == 2, "2 vertices: -10 e 1000000");
        CHECK(g.verificarAresta(-10, 1000000), "aresta entre IDs extremos");
        CHECK(g.verificarAresta(1000000, -10), "aresta simetrica");
        CHECK(g.grauVertice(-10) == 1, "grau de -10");
    }

    std::cout << "\n=== 5. Insercao duplicada de aresta atualiza peso ===\n";
    {
        MeuGrafo g(false);
        g.inserirAresta(0, 1, 1.0);
        g.inserirAresta(0, 1, 9.9);
        CHECK(g.numArestas() == 1, "ainda so 1 aresta apos insercao dupla");
        CHECK(g.obterPeso(0, 1) == 9.9, "peso atualizado");
        CHECK(g.obterPeso(1, 0) == 9.9, "peso simetrico tambem");
    }

    std::cout << "\n=== 6. Insercao de aresta cria vertices automaticamente ===\n";
    {
        MeuGrafo g(true);
        g.inserirAresta(100, 200, 7.0);
        CHECK(g.existeVertice(100) && g.existeVertice(200),
              "vertices 100 e 200 criados automaticamente");
    }

    std::cout << "\n=== 7. Grafo nao-ponderado ignora pesos ===\n";
    {
        MeuGrafo g(false, /*ponderado=*/false);
        g.inserirAresta(0, 1, 42.0);
        CHECK(g.obterPeso(0, 1) == 1.0, "grafo nao-ponderado guarda peso 1.0");
        g.alterarPesoAresta(0, 1, 99.0);
        CHECK(g.obterPeso(0, 1) == 1.0, "alterarPesoAresta ignorado em nao-pond.");
    }

    std::cout << "\n=== 8. Remover vertice central (varia teste do testAll) ===\n";
    {
        MeuGrafo g(false);
        // Estrela centrada em 0
        for (int i = 1; i <= 5; ++i) g.inserirAresta(0, i, 1.0);
        CHECK(g.grauVertice(0) == 5, "grau central = 5");
        g.removerVertice(0);
        CHECK(g.numArestas() == 0, "todas arestas removidas com central");
        for (int i = 1; i <= 5; ++i) {
            CHECK(g.grauVertice(i) == 0, "grau de vertice folha apos remover central");
        }
    }

    std::cout << "\n=== 9. Componentes em grafo so com vertices isolados ===\n";
    {
        MeuGrafo g(false);
        for (int i = 0; i < 4; ++i) g.inserirVertice(i);
        auto comp = ComponentesConexas::encontrar(g);
        CHECK(comp.size() == 4, "4 componentes (1 por vertice isolado)");
    }

    std::cout << "\n=== 10. Componentes em grafo completamente conexo ===\n";
    {
        MeuGrafo g(false);
        for (int i = 0; i < 5; ++i)
            for (int j = i+1; j < 5; ++j)
                g.inserirAresta(i, j, 1.0);
        auto comp = ComponentesConexas::encontrar(g);
        CHECK(comp.size() == 1, "1 componente unico");
        CHECK(comp[0].size() == 5, "componente com 5 vertices");
    }

    std::cout << "\n=== 11. Kosaraju em grafo sem arestas ===\n";
    {
        MeuGrafo g(true);
        for (int i = 0; i < 3; ++i) g.inserirVertice(i);
        auto comp = ComponentesConexas::encontrarFortementeConexos(g);
        CHECK(comp.size() == 3, "3 SCCs (cada vertice e seu proprio SCC)");
    }

    std::cout << "\n=== 12. Kosaraju em DAG (sem ciclos) ===\n";
    {
        MeuGrafo g(true);
        g.inserirAresta(0, 1, 1.0);
        g.inserirAresta(1, 2, 1.0);
        g.inserirAresta(0, 2, 1.0);
        auto comp = ComponentesConexas::encontrarFortementeConexos(g);
        CHECK(comp.size() == 3, "DAG -> cada vertice e seu proprio SCC");
    }

    std::cout << "\n=== 13. Roundtrip salvar/carregar ===\n";
    {
        MeuGrafo g1(false);
        g1.inserirAresta(0, 1, 1.5);
        g1.inserirAresta(1, 2, 2.5);
        g1.inserirVertice(99);  // isolado
        g1.salvarEmArquivo("/tmp/_grafo_test.txt");

        MeuGrafo g2(false);
        g2.carregarDeArquivo("/tmp/_grafo_test.txt");
        CHECK(g2.numVertices() == 4, "roundtrip preserva n. vertices");
        CHECK(g2.numArestas() == 2, "roundtrip preserva n. arestas");
        CHECK(g2.verificarAresta(0, 1), "aresta 0-1 preservada");
        CHECK(g2.obterPeso(1, 2) == 2.5, "peso preservado");
        CHECK(g2.existeVertice(99), "vertice isolado preservado");
    }

    std::cout << "\n=== 14. Carregar arquivo inexistente nao crasha ===\n";
    {
        MeuGrafo g(false);
        bool r = g.carregarDeArquivo("/tmp/_inexistente_xyz.txt");
        CHECK(!r, "carregarDeArquivo retorna false");
        CHECK(g.numVertices() == 0, "grafo permanece vazio");
    }

    std::cout << "\n=== 15. Sequencia agressiva de insercao/remocao ===\n";
    {
        MeuGrafo g(false);
        for (int i = 0; i < 50; ++i) g.inserirVertice(i);
        for (int i = 0; i < 49; ++i) g.inserirAresta(i, i+1, 1.0);
        for (int i = 0; i < 25; ++i) g.removerVertice(i*2);  // remove os pares
        CHECK(g.numVertices() == 25, "25 vertices restantes (impares)");
        // os vertices impares restantes formam isolados (1, 3, 5, ...)
        auto comp = ComponentesConexas::encontrar(g);
        CHECK(comp.size() == 25, "25 componentes (cada um sozinho)");
    }

    std::cout << "\n=========================================\n";
    if (falhas == 0)
        std::cout << "TODOS OS STRESS TESTS PASSARAM\n";
    else
        std::cout << falhas << " FALHA(S) DETECTADA(S)\n";
    std::cout << "=========================================\n";
    return falhas;
}
