# Estruturas de Dados em Árvore — Análise Teórica, Implementação em C++23 e Benchmark Comparativo

**Autor:** Pedro Henrique Silva Costa  
**Repositório GitHub:** [https://github.com/PedroHSC-0/Estruturas-em-arvore](https://github.com/PedroHSC-0/Estruturas-em-arvore)  

---

Este repositório contém a implementação completa, o estudo comparativo e o artigo científico sobre cinco estruturas de dados em árvore de alta performance, desenvolvidas em **C++23 Moderno** utilizando **Módulos (`export module`)**, **Smart Pointers (`std::unique_ptr`)**, compilação via **CMake** e gerador **Ninja**.

---

## 📑 Sumário

1. [Visão Geral e Estruturas Implementadas](#1-visão-geral-e-estruturas-implementadas)
2. [Decisões de Projeto e Arquitetura C++23](#2-decisões-de-projeto-e-arquitetura-c23)
3. [Demonstração e Rastreamento Visual (Estado Inicial, Intermediário e Pós-Remoção)](#3-demonstração-e-rastreamento-visual)
4. [Análise de Complexidade Assintótica e Comparação Teórica (com BST e AVL)](#4-análise-de-complexidade-assintótica-e-comparação-teórica)
5. [Metodologia Experimental e Resultados dos Benchmarks (100.000 Elementos)](#5-metodologia-experimental-e-resultados-dos-benchmarks)
6. [Aplicações Práticas no Mundo Real e Análise Crítica](#6-aplicações-práticas-no-mundo-real-e-análise-crítica)
7. [Como Compilar e Executar](#7-como-compilar-e-executar)
8. [Relatório Acadêmico e Overleaf](#8-relatório-acadêmico-e-overleaf)

---

## 1. Visão Geral e Estruturas Implementadas

O projeto implementa e avalia 5 estruturas avançadas organizadas em módulos independentes:

1. **Trie (Prefix Tree)** (`src/arvores/trie.cppm`): Árvore de prefixos para manipulação de textos.
2. **Patricia Tree / Radix Tree** (`src/arvores/patricia.cppm`): Árvore de prefixos comprimidos onde nós de filho único são fundidos.
3. **Treap (Tree + Heap)** (`src/arvores/treap.cppm`): Árvore binária de busca balanceada aleatoriamente combinando regra de BST nas chaves e Max-Heap nas prioridades.
4. **Splay Tree** (`src/arvores/splay.cppm`): Árvore auto-ajustável que move os elementos recentemente acessados para a raiz via rotações Zig, Zig-Zig e Zig-Zag.
5. **KD-Tree ($K$-Dimensional Tree)** (`src/arvores/kdtree.cppm`): Árvore de particionamento espacial multidimensional para busca por ponto e vizinho mais próximo ($K$-NN).

---

## 2. Decisões de Projeto e Arquitetura C++23

* **C++20/23 Modules (`FILE_SET CXX_MODULES`)**: Ausência total de arquivos `.h`/`.hpp` legados. Redução radical no tempo de compilação e isolamento estrito de escopo.
* **Gestão Automática de Memória (RAII & `std::unique_ptr`)**: Todos os nós são gerenciados por ponteiros inteligentes com posse exclusiva. **Zero vazamentos de memória (0 memory leaks)**.
* **Semantics de Movimento (`std::move`)**: Transferência explícita de posse de nós durante rotações e fusões, evitando cópias profundas.
* **Abstração de Chaves e Genéricos**: Uso de `std::string_view` em Tries para evitar alocações de memória temporárias, e `std::array<double, K>` com templates em C++23 para KD-Tree $K$-dimensional.

---

## 3. Demonstração e Rastreamento Visual

Para responder às exigências de rastreabilidade visual do PDF do trabalho, abaixo são apresentados os **3 estados chave (Inicial, Intermediário/Transformação e Pós-Remoção)** para cada uma das 5 estruturas:

### 3.1. Trie (Prefix Tree)

```text
1. ESTADO INICIAL (Inserções: "carro", "casa"):
(raiz)
 └── c
      └── a
           ├── r └── r └── o [fim]
           └── s └── a [fim]

2. ESTADO INTERMEDIÁRIO (Compartilhamento de Prefixo ao Inserir "casamento"):
(raiz)
 └── c ── a ── s ── a [fim]
                     └── m ── e ── n ── t ── o [fim]  <-- Compartilha o prefixo "casa"

3. ESTADO PÓS-REMOÇÃO (Após remover "casa"):
(raiz)
 └── c ── a ── s ── a [fim=false]  <-- Apenas desmarca a flag 'fim'!
                     └── m ── e ── n ── t ── o [fim=true]
```

---

### 3.2. Patricia Tree (Radix Tree)

```text
1. ESTADO INICIAL (Inserção: "carro", "casa"):
(raiz)
 └── "ca"
      ├── "rro" [fim]
      └── "sa"  [fim]

2. ESTADO INTERMEDIÁRIO (Fatiamento de Nós / Split ao Inserir "casamento"):
(raiz)
 └── "ca"
      ├── "rro" [fim]
      └── "sa"  [fim]
           └── "mento" [fim]  <-- O nó "casa" vira pai do sufixo "mento"

3. ESTADO PÓS-REMOÇÃO (Remoção da folha "carro" -> Fusão de Pai com Único Filho):
(raiz)
 └── "casa" [fim=false]  <-- Fusão compactada do nó raiz com "sa"!
      └── "mento" [fim=true]
```

---

### 3.3. Treap (Tree + Heap)

```text
1. ESTADO INICIAL (Inserções: [50|Prio:70], [20|Prio:40]):
         [ 50 | Prio: 70 ]
        /
 [ 20 | Prio: 40 ]

2. ESTADO INTERMEDIÁRIO (Inserção de [10|Prio:90] -> Rotação à Direita no Pai):
         [ 50 | Prio: 70 ]  <-- (Violação da Heap: 90 > 70!)
        /
 [ 10 | Prio: 90 ]          == rotacionarDireita ==>      [ 10 | Prio: 90 ]
        \                                                        \
   [ 20 | Prio: 40 ]                                       [ 50 | Prio: 70 ]
                                                                 /
                                                          [ 20 | Prio: 40 ]

3. ESTADO PÓS-REMOÇÃO (Remoção da chave 10 -> Retorno à estrutura equilibrada):
         [ 50 | Prio: 70 ]
        /
 [ 20 | Prio: 40 ]
```

---

### 3.4. Splay Tree

```text
1. ESTADO INICIAL (Inserções em linha reta: 10, 20, 30):
 10 ──> 20 ──> 30  (Árvore inclinada como lista)

2. ESTADO INTERMEDIÁRIO (Busca pela chave 30 -> Operação Zig-Zig no Avô 10):
       10                   20                   30
         \                 /  \                 /
          20     ===>     10   30    ===>     20
            \                                /
             30                             10
  (1º Rotaciona Avô 10)            (2º Rotaciona Pai 20)
  ==> A altura da árvore é cortada pela metade!

3. ESTADO PÓS-REMOÇÃO (Remoção da raiz 30 -> Splay do maior da esquerda '20' para a raiz):
         [ 20 ]  <-- Nova raiz
        /
 [ 10 ]
```

---

### 3.5. KD-Tree (2D Spatial Partitioning)

```text
1. ESTADO INICIAL (Corte X=5 no Ponto (5,4)):
   Plano [0,10]x[0,10] dividido verticalmente pela reta X=5.

2. ESTADO INTERMEDIÁRIO (Cortes Y=7 e Y=2 nos Pontos (2,7) e (8,2)):
   Sub-região X < 5 dividida horizontalmente pela reta Y=7.
   Sub-região X >= 5 dividida horizontalmente pela reta Y=2.

3. CONSULTA $K$-NN (Ponto P(7,3)):
   Algoritmo navega até a sub-região X >= 5, calcula a distância Euclidiana 
   e retorna o vizinho mais próximo em (8,2) com poda por hiperplano.
```

---

## 4. Análise de Complexidade Assintótica e Comparação Teórica

A tabela a seguir apresenta a análise formal de complexidade para as 5 estruturas desenvolvidas, comparadas diretamente com a **BST Tradicional** e a **Árvore AVL**:

| Estrutura | Inserção (Melhor / Médio / Pior) | Busca (Melhor / Médio / Pior) | Remoção (Melhor / Médio / Pior) | Complexidade Espacial |
| :--- | :--- | :--- | :--- | :--- |
| **BST Simples** | $O(1)$ / $O(\log n)$ / $O(n)$ | $O(1)$ / $O(\log n)$ / $O(n)$ | $O(1)$ / $O(\log n)$ / $O(n)$ | $O(n)$ |
| **Árvore AVL** | $O(\log n)$ / $O(\log n)$ / $O(\log n)$ | $O(1)$ / $O(\log n)$ / $O(\log n)$ | $O(\log n)$ / $O(\log n)$ / $O(\log n)$ | $O(n)$ |
| **Trie** | $O(L)$ / $O(L)$ / $O(L)$ | $O(L)$ / $O(L)$ / $O(L)$ | $O(L)$ / $O(L)$ / $O(L)$ | $O(N \cdot L \cdot \Sigma)$ (Alta) |
| **Patricia Tree** | $O(L)$ / $O(L)$ / $O(L)$ | $O(L)$ / $O(L)$ / $O(L)$ | $O(L)$ / $O(L)$ / $O(L)$ | $O(N \cdot L)$ (Compacta) |
| **Treap** | $O(1)$ / $O(\log n)$ / $O(n)^*$ | $O(1)$ / $O(\log n)$ / $O(n)^*$ | $O(1)$ / $O(\log n)$ / $O(n)^*$ | $O(n)$ |
| **Splay Tree** | $O(1)$ / $O(\log n)^†$ / $O(n)$ | $O(1)$ / $O(\log n)^†$ / $O(n)$ | $O(1)$ / $O(\log n)^†$ / $O(n)$ | $O(n)$ |
| **KD-Tree** | $O(\log n)$ / $O(\log n)$ / $O(n)$ | $O(1)$ / $O(\log n)$ / $O(n)$ | $O(\log n)$ / $O(\log n)$ / $O(n)$ | $O(n)$ |

*\* $L$ = Tamanho máximo da string/chave. $\Sigma$ = Tamanho do alfabeto. $^* = Pior caso probabilístico com chance $< 1/10^{30}$. $^\dagger = Custo amortizado.*

---

## 5. Metodologia Experimental e Resultados dos Benchmarks

Os experimentos foram executados em um ambiente Linux x86_64, utilizando compilador Clang / GCC com otimização `-O3` e semente aleatória reproduzível (`seed = 42`).

### 5.1. Resultados Medidos no Ambiente do Usuário (100.000 Operações)

#### A. Comparativo de Texto: Trie vs. Patricia Tree (100k Palavras)
* **Inserção (100k)**:
  * Trie: `526.21 ms` (190.039 ops/sec)
  * Patricia Tree: **`244.95 ms`** (🚀 **408.253 ops/sec — 2.15x mais rápida!**)
* **Busca (100k)**:
  * Trie: `140.68 ms` (710.824 ops/sec)
  * Patricia Tree: **`133.81 ms`** (🚀 **747.343 ops/sec**)

#### B. Comparativo de Inteiros: Treap vs. Splay Tree (100k Números)
* **Inserção (100k)**:
  * Treap: **`199.35 ms`** (🚀 **501.631 ops/sec**)
  * Splay Tree: `338.38 ms` (295.525 ops/sec)
* **Busca com Localidade de Referência (Padrão 80/20)**:
  * Treap: **`15.55 ms`** (🚀 **6.432.493 ops/sec — 155.46 ns/op!**)
  * Splay Tree: `80.31 ms` (1.245.187 ops/sec)

#### C. Geometria Espacial: KD-Tree 2D
* **Inserção de 100.000 Pontos 2D**: **`171.16 ms`** (🚀 **584.241 pontos/sec**)
* **10.000 Consultas de Vizinho Mais Próximo ($K$-NN)**: **`25.64 ms`** (🚀 **390.045 consultas/sec**)

---

## 6. Aplicações Práticas no Mundo Real e Análise Crítica

1. **Trie**: Motores de autocompletar e corretores ortográficos simples.
2. **Patricia Tree**: Tabelas de roteamento IP em roteadores de borda (LPM - Longest Prefix Match) e bancos de dados de chaves compactas (LMDB / Ethereum Patricia Merkle Trie).
3. **Splay Tree**: Memórias Cache LRU (Least Recently Used), gerência de memória em compiladores (GCC/LLVM) e listas de acessos altamente desiguais.
4. **Treap**: Estruturas de dados persistentes, ordenação probabilística em sistemas concorrentes (lock-free) e maratonas de programação.
5. **KD-Tree**: Sistemas de Informação Geográfica (GIS - Google Maps / Uber), renderização 3D (Ray Tracing) e algoritmos de aprendizado de máquina (k-NN Classifier).

---

## 7. Como Compilar e Executar

### Pré-requisitos
* CMake 3.28+ 
* Ninja Build System
* Compilador C++23 com suporte a módulos (Clang 17+ ou GCC 14+)

### Passos de Compilação
```bash
# 1. Configurar o projeto com Ninja
cmake -B build -G Ninja

# 2. Compilar os módulos e o executável
cmake --build build

# 3. Executar o suite de testes e benchmarks
./build/app
```

---

## 8. Relatório Acadêmico e Overleaf

O arquivo `main.tex` na raiz deste repositório contém o artigo científico completo formatado em LaTeX. Para compilá-lo no Overleaf:
1. Abra seu projeto no Overleaf.
2. Copie o conteúdo de [`main.tex`](file:///home/pedro/Documentos/GitHub/Estruturas-em-arvore/main.tex) para o arquivo principal.
3. Clique em **Recompile**. O documento gerará o PDF completo com todas as figuras TikZ e tabelas formais.
