# Trabalho Prático 1 — Teoria dos Grafos (DCC059)

**Tema:** Componentes Conexas
**Estrutura escolhida:** Matriz de Adjacência
**Linguagem:** C++ (padrão C++17)
**Alunos:** Thomás Causin, Caio Cunha, Nicholas Cardoso, Igor Reis

---

## 1. Visão Geral

Este trabalho implementa um TAD (Tipo Abstrato de Dados) para representação de
grafos usando **Matriz de Adjacência**, suportando grafos:

- **Orientados** ou **não-orientados** (definido no construtor);
- **Ponderados** ou **não-ponderados** (definido no construtor).

Sobre essa estrutura foi implementado o algoritmo de **Componentes Conexas**,
em duas variantes:

1. **Componentes conexas** (para grafos não-orientados) — BFS, *O(V + E)*.
   Para grafos orientados, esta função retorna **componentes fracamente
   conexos** (ignorando a direção das arestas).
2. **Componentes fortemente conexos** (para grafos orientados) — algoritmo
   de **Kosaraju**, *O(V + E)*.

---

## 2. Estrutura de Arquivos

```
trabalho_grafos/
├── Grafo.h                    # Interface da classe MeuGrafo
├── Grafo.cpp                  # Implementação (matriz de adjacência)
├── ComponentesConexas.h       # Interface dos algoritmos
├── ComponentesConexas.cpp     # Implementação (BFS + Kosaraju)
├── main.cpp                   # Programa interativo com menu
├── testall.cpp                # Bateria de testes obrigatória (fornecida)
├── Makefile                   # Script de compilação
├── INSTRUCOES.txt             # Instruções de compilação/execução
├── README.md                  # Este arquivo
└── exemplos/                  # Grafos de exemplo (formato CS Academy)
    ├── exemplo1_naoorientado.txt
    ├── exemplo2_orientado.txt
    └── exemplo3_simples.txt
```

---

## 3. Compilação e Execução

Veja `INSTRUCOES.txt` para detalhes. Em resumo:

```bash
make            # gera ambos: 'programa' (interativo) e 'test_grafo' (testes)
./test_grafo    # executa a bateria de testes obrigatórios
./programa      # abre o menu interativo
```

### Caso o terminal não reconheça `make` (ex: PowerShell no Windows)

Abra o **Git Bash** na pasta do projeto (botão direito → "Git Bash Here") e compile manualmente:

```bash
# programa interativo
g++ -std=c++17 -o grafos main.cpp Grafo.cpp ComponentesConexas.cpp
./grafos

# bateria de testes
g++ -std=c++17 -o test_grafo testall.cpp Grafo.cpp
./test_grafo
```

---

## 4. Operações Obrigatórias Implementadas

### Manipulação do Grafo
| Operação                       | Método                                              |
|--------------------------------|-----------------------------------------------------|
| Inserir vértice                | `void inserirVertice(int v)`                        |
| Remover vértice                | `void removerVertice(int v)`                        |
| Inserir aresta                 | `void inserirAresta(int u, int v, double peso=1.0)` |
| Remover aresta                 | `void removerAresta(int u, int v)`                  |
| Verificar existência de aresta | `bool verificarAresta(int u, int v) const`          |
| Alterar peso de aresta         | `void alterarPesoAresta(int u, int v, double p)`    |
| Exibir grafo                   | `void exibirGrafo() const`                          |

### Informações sobre Vértices
| Operação              | Método                                       |
|-----------------------|----------------------------------------------|
| Grau de um vértice    | `int grauVertice(int v) const`               |
| Listar vizinhos       | `std::vector<int> listarVizinhos(int v) const` |
| Verificar adjacência  | `bool saoAdjacentes(int u, int v) const`     |

---

## 5. Algoritmo: Componentes Conexas

### 5.1 Grafo Não-Orientado — BFS
Para cada vértice não visitado, executa-se uma **BFS** que coleta todos os
vértices alcançáveis. Cada execução completa de BFS gera um componente.

**Complexidade:** *O(V²)* usando matriz de adjacência — a BFS pergunta os
vizinhos de cada vértice (`listarVizinhos`), e essa consulta custa O(V) na
matriz. Em uma lista de adjacência seria O(V + E).

### 5.2 Grafo Orientado — Kosaraju
Para grafos orientados, componentes conexos têm duas definições distintas:

- **Fracamente conexos:** componente do grafo subjacente não-orientado.
  Calculado pela mesma BFS, percorrendo arestas em ambos os sentidos.
- **Fortemente conexos:** existe caminho dirigido de qualquer vértice do
  componente para qualquer outro. Calculado por **Kosaraju**:

  1. DFS em **G** empilhando vértices em ordem de finalização.
  2. Calcula-se **G^T** (transposto: todas as arestas invertidas).
  3. DFS em **G^T** retirando vértices do topo da pilha; cada árvore de
     DFS gerada é um SCC.

  **Complexidade:** *O(V²)* com matriz de adjacência (cada DFS visita
  todos os vizinhos potenciais de cada vértice).

---

## 6. Formato de Arquivo (CS Academy)

Os exemplos seguem o formato de
[csacademy.com/app/graph_editor/](https://csacademy.com/app/graph_editor/):

- Linha com **um inteiro** → vértice isolado
- Linha com **dois inteiros** → aresta não-ponderada `u v`
- Linha com **três valores** → aresta ponderada `u v peso`
- Linhas em branco ou começando com `#` são ignoradas (extensão para
  permitir comentários nos arquivos de exemplo)

---

## 7. Decisões de Implementação (Justificativas)

| Decisão | Motivo |
|---|---|
| **Matriz de adjacência com células `{existe, peso}`** | Cada célula guarda explicitamente um booleano de existência e o peso. Isso evita usar valores sentinelas (como 0 ou -1) para indicar "sem aresta", permitindo que pesos 0 ou negativos sejam armazenados corretamente. |
| **Mapeamento `ID <-> índice` (vector + map)** | A matriz é indexada por inteiros 0..n-1, mas os IDs de vértices podem ser arbitrários (ex.: 0, 5, 99). Mantemos `indiceParaId` (vetor) e `idParaIndice` (map) para traduzir entre os dois espaços em O(log n). |
| **Remoção real de linha/coluna ao remover vértice** | Quando um vértice é removido, as linhas/colunas correspondentes são fisicamente apagadas e os índices subsequentes são decrementados. Custo O(n²), mas mantém a matriz sempre compacta e simétrica. A alternativa de "marcar como removido" complicaria todas as outras operações sem ganho claro para o porte de grafos didáticos. |
| **Tolerância a vértices inválidos** | Todas as consultas (`verificarAresta`, `grauVertice`, etc.) tratam IDs inexistentes retornando valor neutro (false/0/lista vazia) **sem lançar exceção**, como exige a bateria de testes. |
| **`inserirAresta` idempotente** | Se a aresta já existe, atualizamos o peso em vez de duplicar (na matriz isso é trivial: basta sobrescrever a célula). |
| **Simetria automática em grafos não-orientados** | Inserir aresta `{u,v}` escreve em `matriz[i][j]` e em `matriz[j][i]`. Isso simplifica `listarVizinhos`, `grauVertice` e operações de BFS, que podem assumir que basta percorrer uma das direções. |
| **BFS em vez de DFS recursivo para componentes** | Implementação iterativa evita estouro de pilha em grafos grandes/profundos. |
| **DFS iterativa em Kosaraju** | Mesma razão: robustez para entradas grandes. |

### 7.1 Comparação Matriz vs Lista de Adjacência

| Operação | Matriz (esta impl.) | Lista de Adjacência |
|---|---|---|
| `verificarAresta(u,v)` | **O(log n)** (busca do índice) | O(grau(u)) |
| `inserirAresta` | O(log n) | O(grau(u)) |
| `removerAresta` | O(log n) | O(grau(u)) |
| `listarVizinhos(v)` | O(n) | **O(grau(v))** |
| `grauVertice(v)` | O(n) | **O(1)** (ou O(grau)) |
| `removerVertice(v)` | O(n²) | O(V + E) |
| Espaço total | **O(V²)** | O(V + E) |

A matriz é **mais rápida para consultas pontuais de adjacência** mas **gasta
mais memória** e é mais lenta para listar vizinhos. Para o algoritmo de
componentes conexas via BFS, a complexidade total fica **O(V²)** porque a
BFS pergunta os vizinhos de cada vértice, e essa operação custa O(V) na
matriz. Em uma lista de adjacência seria O(V + E).

---

## 8. Resultados da Bateria de Testes

A bateria fornecida em `testall.cpp` (23 testes) é executada integralmente:

```
==================================================
         RESUMO FINAL DA BATERIA DE TESTES        
==================================================
  TESTES QUE PASSARAM: 23
  TESTES QUE FALHARAM: 0
  EXCECOES CAPTURADAS: 0
  TOTAL DE CASOS TESTADOS: 23
==================================================
```
---

## 9. Exemplo de Utilização

Abaixo está um exemplo simples de criação de um grafo não orientado e não ponderado:

```cpp
MeuGrafo g(false, false);

g.inserirVertice(1);
g.inserirVertice(2);
g.inserirVertice(3);

g.inserirAresta(1, 2);
g.inserirAresta(2, 3);

g.exibirGrafo();
```

Esse exemplo cria três vértices e duas arestas, formando um único componente conexo.

---

## 10. Autores

Projeto desenvolvido para a disciplina DCC059 - Teoria dos Grafos.

- Thomás Causin
- Caio Cunha
- Nicholas Cardoso
- Igor Reis
