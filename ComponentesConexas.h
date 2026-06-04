// ComponentesConexas.h - DCC059 Trabalho Prático 1
//
// encontrar()                -> componentes conexas via BFS
//   - não-orientado: componentes conexas clássicas
//   - orientado: componentes fracamente conexas (ignora direção)
//
// encontrarFortementeConexos() -> Kosaraju (só faz sentido em orientados)
//
// imprimir() -> exibe o resultado formatado

#ifndef COMPONENTES_CONEXAS_H
#define COMPONENTES_CONEXAS_H

#include "Grafo.h"
#include <string>
#include <vector>

class ComponentesConexas {
public:
    static std::vector<std::vector<int>> encontrar(const MeuGrafo& grafo);
    static std::vector<std::vector<int>> encontrarFortementeConexos(const MeuGrafo& grafo);
    static void imprimir(const std::vector<std::vector<int>>& componentes,
                         const std::string& tipo = "Componentes Conexas");
};

#endif // COMPONENTES_CONEXAS_H
