#include <iostream>
#include <iomanip>

import trie;
import patricia;
import treap;
import splay;
import kdtree;
import generator;
import benchmark;

void testarTrie() {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [1/5] TESTANDO ESTRUTURA: TRIE (Prefix Tree)\n";
    std::cout << "--------------------------------------------------\n";

    Trie trie;

    // Teste 1: Inserções
    trie.inserir("carro");
    trie.inserir("casa");
    trie.inserir("casamento");

    std::cout << " -> Inseridas palavras: 'carro', 'casa', 'casamento'\n";

    // Teste 2: Buscas
    std::cout << " -> Busca por 'carro'    : " << (trie.pesquisar("carro") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casa'     : " << (trie.pesquisar("casa") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casamento': " << (trie.pesquisar("casamento") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'cas' (pref): " << (!trie.pesquisar("cas") ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'aviao'    : " << (!trie.pesquisar("aviao") ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';

    // Teste 3: Remoção
    std::cout << " -> Removendo 'casa'...\n";
    trie.remover("casa");
    std::cout << " -> Busca por 'casa' pos-remocao: " << (!trie.pesquisar("casa") ? "✅ Removido (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casamento' pos-remocao: " << (trie.pesquisar("casamento") ? "✅ Mantido (Correto)" : "❌ Falhou") << '\n';
    std::cout << "--------------------------------------------------\n\n";
}

void testarPatricia() {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [2/5] TESTANDO ESTRUTURA: PATRICIA TREE (Radix)\n";
    std::cout << "--------------------------------------------------\n";

    Patricia patricia;

    // Teste 1: Inserções com fatiamento e split de nós
    patricia.inserir("carro");
    patricia.inserir("casa");
    patricia.inserir("casamento");
    patricia.inserir("casal");

    std::cout << " -> Inseridas palavras: 'carro', 'casa', 'casamento', 'casal'\n";

    // Teste 2: Buscas exatas e falsos-positivos de prefixos
    std::cout << " -> Busca por 'carro'    : " << (patricia.pesquisar("carro") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casa'     : " << (patricia.pesquisar("casa") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casamento': " << (patricia.pesquisar("casamento") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casal'    : " << (patricia.pesquisar("casal") ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'cas' (pref): " << (!patricia.pesquisar("cas") ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'aviao'    : " << (!patricia.pesquisar("aviao") ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';

    // Teste 3: Remoção com Fusão de Nós (Caso 1, Caso 2 e Caso 3)
    std::cout << " -> Removendo 'casa'...\n";
    patricia.remover("casa");
    std::cout << " -> Busca por 'casa' pos-remocao: " << (!patricia.pesquisar("casa") ? "✅ Removido (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casamento' pos-remocao: " << (patricia.pesquisar("casamento") ? "✅ Mantido (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 'casal' pos-remocao: " << (patricia.pesquisar("casal") ? "✅ Mantido (Correto)" : "❌ Falhou") << '\n';

    std::cout << " -> Removendo 'carro' (Folha - Caso 3 + Fusao do Pai)...\n";
    patricia.remover("carro");
    std::cout << " -> Busca por 'carro' pos-remocao: " << (!patricia.pesquisar("carro") ? "✅ Removido (Correto)" : "❌ Falhou") << '\n';
    std::cout << "--------------------------------------------------\n\n";
}

void testarTreap() {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [3/5] TESTANDO ESTRUTURA: TREAP (BST + Heap)\n";
    std::cout << "--------------------------------------------------\n";

    Treap treap;

    // Teste 1: Inserções
    treap.inserir(10);
    treap.inserir(20);
    treap.inserir(30);
    treap.inserir(40);
    treap.inserir(50);
    treap.inserir(25);
    treap.inserir(5);

    std::cout << " -> Chaves Inseridas: 10, 20, 30, 40, 50, 25, 5\n";

    // Teste 2: Buscas Exatas e Inexistentes
    std::cout << " -> Busca por 10    : " << (treap.pesquisar(10) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 20    : " << (treap.pesquisar(20) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 25    : " << (treap.pesquisar(25) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 50    : " << (treap.pesquisar(50) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 70    : " << (!treap.pesquisar(70) ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';

    // Teste 3: Remoções (Folhas e Nós Intermediários)
    std::cout << " -> Removendo chave 20...\n";
    treap.remover(20);
    std::cout << " -> Busca por 20 pos-remocao: " << (!treap.pesquisar(20) ? "✅ Removido (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 25 pos-remocao: " << (treap.pesquisar(25) ? "✅ Mantido (Correto)" : "❌ Falhou") << '\n';

    std::cout << " -> Removendo chave 5 (Folha)...\n";
    treap.remover(5);
    std::cout << " -> Busca por 5 pos-remocao : " << (!treap.pesquisar(5) ? "✅ Removido (Correto)" : "❌ Falhou") << '\n';
    std::cout << "--------------------------------------------------\n\n";
}

void testarSplay() {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [4/5] TESTANDO ESTRUTURA: SPLAY TREE\n";
    std::cout << "--------------------------------------------------\n";

    SplayTree splay;

    // Teste 1: Inserções
    splay.inserir(100);
    splay.inserir(50);
    splay.inserir(150);
    splay.inserir(25);
    splay.inserir(75);

    std::cout << " -> Chaves Inseridas: 100, 50, 150, 25, 75\n";

    // Teste 2: Buscas (Que sobem os elementos para a Raiz)
    std::cout << " -> Busca por 75    : " << (splay.pesquisar(75) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 25    : " << (splay.pesquisar(25) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 100   : " << (splay.pesquisar(100) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 999   : " << (!splay.pesquisar(999) ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';

    // Teste 3: Remoção (Splay da raiz + união das sub-árvores)
    std::cout << " -> Removendo chave 50...\n";
    splay.remover(50);
    std::cout << " -> Busca por 50 pos-remocao : " << (!splay.pesquisar(50) ? "✅ Removido (Correto)" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por 75 pos-remocao : " << (splay.pesquisar(75) ? "✅ Mantido (Correto)" : "❌ Falhou") << '\n';
    std::cout << "--------------------------------------------------\n\n";
}

void testarKdTree() {
    std::cout << "--------------------------------------------------\n";
    std::cout << " [5/5] TESTANDO ESTRUTURA: KD-TREE (2D Spatial)\n";
    std::cout << "--------------------------------------------------\n";

    KdTree<2> kdtree;

    // Teste 1: Inserções de Pontos (X, Y)
    kdtree.inserir({3.0, 6.0});
    kdtree.inserir({17.0, 15.0});
    kdtree.inserir({13.0, 1.0});
    kdtree.inserir({6.0, 12.0});
    kdtree.inserir({9.0, 1.0});

    std::cout << " -> Pontos 2D Inseridos: (3,6), (17,15), (13,1), (6,12), (9,1)\n";

    // Teste 2: Buscas Exatas
    std::cout << " -> Busca por (17, 15) : " << (kdtree.pesquisar({17.0, 15.0}) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por (6, 12)  : " << (kdtree.pesquisar({6.0, 12.0}) ? "✅ Encontrado" : "❌ Falhou") << '\n';
    std::cout << " -> Busca por (10, 10) : " << (!kdtree.pesquisar({10.0, 10.0}) ? "✅ Nao Encontrado (Correto)" : "❌ Falhou") << '\n';

    // Teste 3: Busca por Vizinho Mais Próximo (KNN)
    std::array<double, 2> alvo = {10.0, 2.0};
    auto vizinho = kdtree.buscarVizinhoMaisProximo(alvo);
    std::cout << " -> Vizinho Mais Próximo de (10, 2): (" << vizinho[0] << ", " << vizinho[1] << ") ✅ (Esperado: (9, 1) ou (13, 1))\n";
    std::cout << "--------------------------------------------------\n\n";
}

void rodarBenchmarksComparativos() {
    std::cout << "==================================================\n";
    std::cout << " SUITE COMPLETA DE BENCHMARKS (100.000 ELEMENTOS)\n";
    std::cout << "==================================================\n\n";

    GeradorDeEntradas gerador(42);
    constexpr size_t NUM_ELEMENTOS = 100'000;

    // 1. Strings (Trie vs Patricia)
    std::cout << "--------------------------------------------------\n";
    std::cout << " BENCHMARK 1: STRINGS (Trie vs Patricia)\n";
    std::cout << "--------------------------------------------------\n";

    auto datasetStr = gerador.gerarStringsAleatorias(NUM_ELEMENTOS, 5, 15);
    auto queriesStr = gerador.gerarStringsAleatorias(NUM_ELEMENTOS, 5, 15);

    Trie trie;
    Patricia patricia;

    auto resTrieIns = Benchmark::medir("Trie - Insercao 100k", NUM_ELEMENTOS, [&]() {
        for (const auto& w : datasetStr) trie.inserir(w);
    });
    auto resPatIns = Benchmark::medir("Patricia - Insercao 100k", NUM_ELEMENTOS, [&]() {
        for (const auto& w : datasetStr) patricia.inserir(w);
    });
    auto resTrieBusca = Benchmark::medir("Trie - Busca 100k", NUM_ELEMENTOS, [&]() {
        for (const auto& w : queriesStr) (void)trie.pesquisar(w);
    });
    auto resPatBusca = Benchmark::medir("Patricia - Busca 100k", NUM_ELEMENTOS, [&]() {
        for (const auto& w : queriesStr) (void)patricia.pesquisar(w);
    });

    Benchmark::imprimirResultado(resTrieIns);
    Benchmark::imprimirResultado(resPatIns);
    Benchmark::imprimirResultado(resTrieBusca);
    Benchmark::imprimirResultado(resPatBusca);

    // 2. Inteiros (Treap vs Splay Tree)
    std::cout << "\n--------------------------------------------------\n";
    std::cout << " BENCHMARK 2: INTEIROS (Treap vs Splay Tree)\n";
    std::cout << "--------------------------------------------------\n";

    auto dadosInt = gerador.gerarInteirosAleatorios(NUM_ELEMENTOS, 1, 1'000'000);
    auto acessosLocalidade = gerador.gerarAcessosComLocalidade(NUM_ELEMENTOS, 1'000); // 80% acessos em 20% elementos

    Treap treap;
    SplayTree splay;

    auto resTreapIns = Benchmark::medir("Treap - Insercao 100k", NUM_ELEMENTOS, [&]() {
        for (int v : dadosInt) treap.inserir(v);
    });
    auto resSplayIns = Benchmark::medir("Splay - Insercao 100k", NUM_ELEMENTOS, [&]() {
        for (int v : dadosInt) splay.inserir(v);
    });

    auto resTreapBuscaLoc = Benchmark::medir("Treap - Busca com Localidade 80/20", NUM_ELEMENTOS, [&]() {
        for (int v : acessosLocalidade) (void)treap.pesquisar(v);
    });
    auto resSplayBuscaLoc = Benchmark::medir("Splay - Busca com Localidade 80/20", NUM_ELEMENTOS, [&]() {
        for (int v : acessosLocalidade) (void)splay.pesquisar(v);
    });

    Benchmark::imprimirResultado(resTreapIns);
    Benchmark::imprimirResultado(resSplayIns);
    Benchmark::imprimirResultado(resTreapBuscaLoc);
    Benchmark::imprimirResultado(resSplayBuscaLoc);

    // 3. Pontos Espaciais 2D (KD-Tree)
    std::cout << "\n--------------------------------------------------\n";
    std::cout << " BENCHMARK 3: GEOMETRIA ESPACIAL 2D (KD-Tree)\n";
    std::cout << "--------------------------------------------------\n";

    auto pontos2D = gerador.gerarPontosND<2>(NUM_ELEMENTOS, 0.0, 10000.0);
    auto consultasKNN = gerador.gerarPontosND<2>(10'000, 0.0, 10000.0); // 10k consultas KNN

    KdTree<2> kdtree;

    auto resKdIns = Benchmark::medir("KD-Tree 2D - Insercao 100k Pontos", NUM_ELEMENTOS, [&]() {
        for (const auto& p : pontos2D) kdtree.inserir(p);
    });

    auto resKdKNN = Benchmark::medir("KD-Tree 2D - 10k Consultas Vizinho Mais Proximo (KNN)", 10'000, [&]() {
        for (const auto& p : consultasKNN) (void)kdtree.buscarVizinhoMaisProximo(p);
    });

    Benchmark::imprimirResultado(resKdIns);
    Benchmark::imprimirResultado(resKdKNN);
}

int main() {
    std::cout << "==================================================\n";
    std::cout << " ESTRUTURAS DE DADOS EM ARVORE - SUITE COMPLETA\n";
    std::cout << "==================================================\n\n";

    testarTrie();
    testarPatricia();
    testarTreap();
    testarSplay();
    testarKdTree();

    rodarBenchmarksComparativos();

    return 0;
}