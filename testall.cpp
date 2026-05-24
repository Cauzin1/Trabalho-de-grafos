/**
 * ============================================================================
 * ARQUIVO DE TESTES OBRIGATÓRIOS - TRABALHO PRÁTICO 1 (TEORIA DOS GRAFOS)
 * ============================================================================
 * * INSTRUÇÕES PARA OS ALUNOS:
 * Este arquivo contém a bateria de testes para avaliar a implementação
 * do seu grafo. Para que ele funcione com o seu código, você tem duas opções:
 * * OPÇÃO 1:
 * Mude os nomes dos métodos na sua classe original para que fiquem exatamente
 * iguais aos chamados na função testAll() (ex: inserirVertice, inserirAresta...).
 *
 * * OPÇÃO 2 (Usando um Wrapper / Adaptador):
 * Se você não quiser mudar os nomes das funções na sua classe, você pode criar
 * uma classe "casca" (Wrapper) que apenas traduz os nomes esperados pelo teste 
 * para as funções reais que você criou. O exemplo abaixo mostra como fazer isso.
 *
 * * ATENÇÃO: NÃO altere a lógica da função testAll(). Apenas substitua a 
 * instanciação nas primeiras linhas da função pelas suas próprias classes.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include "Grafo.h"   // <-- Inclusão necessária para reconhecer a classe MeuGrafo

// ============================================================================
// EXEMPLO DE OPÇÃO 2: COMO USAR UM WRAPPER
// ============================================================================

/*
// 1. Imagine que esta é a classe original que o seu grupo implementou.
class MeuGrafoOriginal {
public:
    MeuGrafoOriginal(bool ehDirecionado) { ... }
    void adicionarNo(int id) { ... }
    void ligarNos(int a, int b, double peso) { ... }
    bool existeCaminho(int a, int b) { ... }
    void mudarValor(int a, int b, double p) { ... }
    int pegarGrau(int v) { ... }
    std::vector<int> pegarVizinhos(int v) { ... }
    bool checarAdjacencia(int a, int b) { ... }
    void deletarLink(int a, int b) { ... }
    void apagarNo(int v) { ... }
    void print() { ... }
};

// 2. Este é o Wrapper. A função dele é apenas chamar as funções da sua classe, mas definindo os nomes de funções que o testAll entende.
class GrafoWrapper {
private:
    MeuGrafoOriginal* grafoReal;

public:
    // Construtor repassa a configuração de orientação para a sua classe
    GrafoWrapper(bool orientado) {
        grafoReal = new MeuGrafoOriginal(orientado);
    }
    
    ~GrafoWrapper() {
        delete grafoReal;
    }

    // Tradução dos métodos exigidos pelo testAll() para os seus métodos:
    void inserirVertice(int v) { grafoReal->adicionarNo(v); }
    void inserirAresta(int u, int v, double peso = 1.0) { grafoReal->ligarNos(u, v, peso); }
    bool verificarAresta(int u, int v) { return grafoReal->existeCaminho(u, v); }
    void alterarPesoAresta(int u, int v, double peso) { grafoReal->mudarValor(u, v, peso); }
    int grauVertice(int v) { return grafoReal->pegarGrau(v); }
    std::vector<int> listarVizinhos(int v) { return grafoReal->pegarVizinhos(v); }
    bool saoAdjacentes(int u, int v) { return grafoReal->checarAdjacencia(u, v); }
    void removerAresta(int u, int v) { grafoReal->deletarLink(u, v); }
    void removerVertice(int v) { grafoReal->apagarNo(v); }
    void exibirGrafo() { grafoReal->print(); }
};
*/

void testAll() {
    // SUBSTITUA AS LINHAS ABAIXO PELAS SUAS CLASSES
    // Exemplo: Se usou o Wrapper acima, mude para:
    // GrafoWrapper gNaoOrientado(false);
    // GrafoWrapper gOrientado(true);
    MeuGrafo gNaoOrientado(false); // Instancia de Grafo NAO Orientado
    MeuGrafo gOrientado(true);     // Instancia de Grafo ORIENTADO

    int passou = 0;
    int falhou = 0;
    int excecoes = 0;

    std::cout << "==================================================\n";
    std::cout << "              BATERIA DE TESTES \n";
    std::cout << "==================================================\n\n";

    // -------------------------------------------------------------------------
    // PARTE A: TESTES EM GRAFO NÃO ORIENTADO
    // -------------------------------------------------------------------------
    std::cout << "--------------------------------------------------\n";
    std::cout << "      Fase 1: Testando Grafo Nao Orientado        \n";
    std::cout << "--------------------------------------------------\n\n";

    std::cout << "[TESTE] Inserindo vertices iniciais (0 a 5)...\n";
    try {
        for (int i = 0; i <= 5; i++) gNaoOrientado.inserirVertice(i);
        std::cout << "  STATUS: PASSOU\n\n";
        passou++;
    } catch (...) {
        std::cout << "  STATUS: FALHOU (Lancou excecao ao inserir vertices)\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Inserindo malha de arestas...\n";
    try {
        gNaoOrientado.inserirAresta(0, 1, 1.5);
        gNaoOrientado.inserirAresta(0, 2, 2.0);
        gNaoOrientado.inserirAresta(1, 2, 1.0);
        gNaoOrientado.inserirAresta(2, 3, 3.5);
        gNaoOrientado.inserirAresta(3, 4, 4.0);
        gNaoOrientado.inserirAresta(4, 5, 2.5);
        gNaoOrientado.inserirAresta(3, 5, 5.0);
        std::cout << "  STATUS: PASSOU\n\n";
        passou++;
    } catch (...) {
        std::cout << "  STATUS: FALHOU (Lancou excecao ao inserir arestas)\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar existencia da aresta existente (0, 1):\n";
    std::cout << "  Saida Esperada: VERDADEIRO\n";
    try {
        bool obtido = gNaoOrientado.verificarAresta(0, 1);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar existencia na direcao inversa (1, 0):\n";
    std::cout << "  Saida Esperada: VERDADEIRO\n";
    try {
        bool obtido = gNaoOrientado.verificarAresta(1, 0);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar aresta inexistente entre vertices existentes (0, 5):\n";
    std::cout << "  Saida Esperada: FALSO\n";
    try {
        bool obtido = gNaoOrientado.verificarAresta(0, 5);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (!obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar vertices invalidos/inexistentes (99, 100):\n";
    std::cout << "  Saida Esperada: FALSO \n";
    try {
        bool obtido = gNaoOrientado.verificarAresta(99, 100);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (!obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Alterar peso da aresta (2, 3) para 9.9:\n";
    std::cout << "  Saida Esperada: Modificacao executada com sucesso\n";
    try {
        gNaoOrientado.alterarPesoAresta(2, 3, 9.9);
        std::cout << "  Saida Obtida:   Modificacao executada com sucesso\n  STATUS:         PASSOU\n\n";
        passou++;
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Calcular grau do vertice 2 (Conectado a 0, 1 e 3):\n";
    std::cout << "  Saida Esperada: 3\n";
    try {
        int obtido = gNaoOrientado.grauVertice(2);
        std::cout << "  Saida Obtida:   " << obtido << "\n";
        if (obtido == 3) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Calcular grau de vertice inexistente (99):\n";
    std::cout << "  Saida Esperada: 0 (Tratado sem travar)\n";
    try {
        int obtido = gNaoOrientado.grauVertice(99);
        std::cout << "  Saida Obtida:   " << obtido << "\n";
        if (obtido == 0) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Listar vizinhos do vertice 3:\n";
    std::cout << "  Saida Esperada: [2, 4, 5]\n";
    try {
        std::vector<int> obtido = gNaoOrientado.listarVizinhos(3);
        std::sort(obtido.begin(), obtido.end()); 
        std::cout << "  Saida Obtida:   [";
        for (size_t i = 0; i < obtido.size(); ++i) {
            std::cout << obtido[i] << (i == obtido.size() - 1 ? "" : ", ");
        }
        std::cout << "]\n";
        bool correto = (obtido.size() == 3 && obtido[0] == 2 && obtido[1] == 4 && obtido[2] == 5);
        if (correto) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar se 4 e 5 de forma bidirecional sao adjacentes:\n";
    std::cout << "  Saida Esperada: (4,5)=VERDADEIRO e (5,4)=VERDADEIRO\n";
    try {
        bool dir1 = gNaoOrientado.saoAdjacentes(4, 5);
        bool dir2 = gNaoOrientado.saoAdjacentes(5, 4);
        std::cout << "  Saida Obtida:   (4,5): " << (dir1 ? "VERDADEIRO" : "FALSO") << " | (5,4): " << (dir2 ? "VERDADEIRO" : "FALSO") << "\n";
        if (dir1 && dir2) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Remover aresta (3, 4):\n";
    std::cout << "  Saida Esperada: FALSO para ambas as direcoes\n";
    try {
        gNaoOrientado.removerAresta(3, 4);
        bool aindaExisteDireto = gNaoOrientado.verificarAresta(3, 4);
        bool aindaExisteInverso = gNaoOrientado.verificarAresta(4, 3);
        std::cout << "  Saida Obtida:   Aresta (3,4) existe? " << (aindaExisteDireto ? "VERDADEIRO" : "FALSO") << "\n";
        std::cout << "                  Aresta (4,3) existe? " << (aindaExisteInverso ? "VERDADEIRO" : "FALSO") << "\n";
        if (!aindaExisteDireto && !aindaExisteInverso) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Remover vertice 2 (Remocao de No Central):\n";
    std::cout << "  Saida Esperada: FALSO para adjacencias com vizinhos antigo e reducao de graus\n";
    try {
        gNaoOrientado.removerVertice(2);
        bool adj02 = gNaoOrientado.saoAdjacentes(0, 2);
        bool adj12 = gNaoOrientado.saoAdjacentes(1, 2);
        int grauRestante0 = gNaoOrientado.grauVertice(0);
        std::cout << "  Saida Obtida:   Adjacente (0,2)? " << (adj02 ? "VERDADEIRO" : "FALSO") << " | Adjacente (1,2)? " << (adj12 ? "VERDADEIRO" : "FALSO") << "\n";
        std::cout << "                  Grau restante do vertice 0: " << grauRestante0 << " (Esperado: 1, conectado apenas ao 1)\n";
        if (!adj02 && !adj12 && grauRestante0 == 1) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU (Deixou arestas orfas estruturais)\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA / CRASH\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }


    // -------------------------------------------------------------------------
    // PARTE B: TESTES EM GRAFO ORIENTADO
    // -------------------------------------------------------------------------
    std::cout << "--------------------------------------------------\n";
    std::cout << "        Fase 2: Testando Grafo Orientado          \n";
    std::cout << "--------------------------------------------------\n\n";

    std::cout << "[TESTE] Inserindo vertices iniciais (0 a 5)...\n";
    try {
        for (int i = 0; i <= 5; i++) gOrientado.inserirVertice(i);
        std::cout << "  STATUS: PASSOU\n\n";
        passou++;
    } catch (...) {
        std::cout << "  STATUS: FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Inserindo malha direcionada de arcos (Ciclos: 0->1->2->0 e 3->4->5->3)...\n";
    try {
        gOrientado.inserirAresta(0, 1, 1.0);
        gOrientado.inserirAresta(1, 2, 2.0);
        gOrientado.inserirAresta(2, 0, 3.0);
        gOrientado.inserirAresta(2, 3, 1.5);
        gOrientado.inserirAresta(3, 4, 2.5);
        gOrientado.inserirAresta(4, 5, 3.5);
        gOrientado.inserirAresta(5, 3, 4.5);
        
        std::cout << "  STATUS: PASSOU\n\n";
        passou++;
    } catch (...) {
        std::cout << "  STATUS: FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar existencia da aresta direcionada existente (0, 1):\n";
    std::cout << "  Saida Esperada: VERDADEIRO\n";
    try {
        bool obtido = gOrientado.verificarAresta(0, 1);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar existencia na direcao inversa (1, 0):\n";
    std::cout << "  Saida Esperada: FALSO\n";
    try {
        bool obtido = gOrientado.verificarAresta(1, 0);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (!obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar arco inexistente entre vertices existentes (0, 5):\n";
    std::cout << "  Saida Esperada: FALSO\n";
    try {
        bool obtido = gOrientado.verificarAresta(0, 5);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (!obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar vertices invalidos/inexistentes no Orientado (99, 100):\n";
    std::cout << "  Saida Esperada: FALSO\n";
    try {
        bool obtido = gOrientado.verificarAresta(99, 100);
        std::cout << "  Saida Obtida:   " << (obtido ? "VERDADEIRO" : "FALSO") << "\n";
        if (!obtido) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Verificar Adjacencia Unidirecional (2, 3) vs (3, 2):\n";
    std::cout << "  Saida Esperada: (2,3)=VERDADEIRO e (3,2)=FALSO\n";
    try {
        bool dir1 = gOrientado.saoAdjacentes(2, 3);
        bool dir2 = gOrientado.saoAdjacentes(3, 2);
        std::cout << "  Saida Obtida:   (2,3): " << (dir1 ? "VERDADEIRO" : "FALSO") << " | (3,2): " << (dir2 ? "VERDADEIRO" : "FALSO") << "\n";
        if (dir1 && !dir2) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Remover arco direcionado (4, 5):\n";
    std::cout << "  Saida Esperada: FALSO para (4,5)\n";
    try {
        gOrientado.removerAresta(4, 5);
        bool aindaExiste = gOrientado.verificarAresta(4, 5);
        std::cout << "  Saida Obtida:   Arco (4,5) ainda existe? " << (aindaExiste ? "VERDADEIRO" : "FALSO") << "\n";
        if (!aindaExiste) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Remover vertice 3 (Ponto de articulacao dos ciclos):\n";
    std::cout << "  Saida Esperada: FALSO para arcos de entrada (2,3) e de saida (3,4)\n";
    try {
        gOrientado.removerVertice(3);
        bool adj23 = gOrientado.saoAdjacentes(2, 3);
        bool adj53 = gOrientado.saoAdjacentes(5, 3);
        std::cout << "  Saida Obtida:   Adjacencia (2,3)? " << (adj23 ? "VERDADEIRO" : "FALSO") << "\n";
        std::cout << "                  Adjacencia (5,3)? " << (adj53 ? "VERDADEIRO" : "FALSO") << "\n";
        if (!adj23 && !adj53) { std::cout << "  STATUS:         PASSOU\n\n"; passou++; }
        else { std::cout << "  STATUS:         FALHOU\n\n"; falhou++; }
    } catch (...) {
        std::cout << "  Saida Obtida:   EXCECAO LANCADA\n  STATUS:         FALHOU\n\n";
        excecoes++;
    }

    std::cout << "[TESTE] Exibir estado impresso dos grafos finais:\n";
    std::cout << "--------------------------------------------------\n";
    try {
        std::cout << ">> GRAFO NAO ORIENTADO RESULTANTE:\n";
        gNaoOrientado.exibirGrafo(); 
        
        std::cout << "\n>> GABARITO ESPERADO PARA O NAO ORIENTADO (FORMATO CS ACADEMY):\n";
        std::cout << "5\n0 1 1.5\n4 5 2.5\n3 5 5.0\n";

        std::cout << "\n==================================================\n";

        std::cout << ">> GRAFO ORIENTADO RESULTANTE:\n";
        gOrientado.exibirGrafo();
        
        std::cout << "\n>> GABARITO ESPERADO PARA O ORIENTADO (FORMATO CS ACADEMY):\n";
        std::cout << "5\n0 1 1\n1 2 2\n2 0 3\n";
        
        std::cout << "--------------------------------------------------\n";
        std::cout << "  STATUS:         PASSOU\n\n";
        passou++;
    } catch (...) {
        std::cout << "--------------------------------------------------\n";
        std::cout << "  STATUS:         FALHOU \n\n";
        excecoes++;
    }

    std::cout << "==================================================\n";
    std::cout << "         RESUMO FINAL DA BATERIA DE TESTES        \n";
    std::cout << "==================================================\n";
    std::cout << "  TESTES QUE PASSARAM: " << passou << "\n";
    std::cout << "  TESTES QUE FALHARAM: " << falhou << "\n";
    std::cout << "  EXCECOES CAPTURADAS: " << excecoes << "\n";
    std::cout << "  TOTAL DE CASOS TESTADOS: " << (passou + falhou + excecoes) << "\n";
    std::cout << "==================================================\n";
}

int main() {
    testAll();
    return 0;
}
