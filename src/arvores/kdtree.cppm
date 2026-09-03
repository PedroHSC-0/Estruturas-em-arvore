module;

#include <array>
#include <memory>
#include <vector>
#include <cmath>
#include <limits>
#include <iostream>

export module kdtree;

export template <size_t K = 2>
struct KdNode {
    std::array<double, K> ponto;
    std::unique_ptr<KdNode<K>> esquerda;
    std::unique_ptr<KdNode<K>> direita;

    explicit KdNode(const std::array<double, K>& pt)
        : ponto(pt), esquerda(nullptr), direita(nullptr) {}
};

export template <size_t K = 2>
class KdTree {
private:
    std::unique_ptr<KdNode<K>> raiz;

    // Distância Euclidiana ao Quadrado (evita sqrt desnecessário)
    double distanciaQuadrada(const std::array<double, K>& a, const std::array<double, K>& b) const {
        double soma = 0.0;
        for (size_t i = 0; i < K; ++i) {
            double diff = a[i] - b[i];
            soma += diff * diff;
        }
        return soma;
    }

    // Inserção Auxiliar Recursiva
    std::unique_ptr<KdNode<K>> inserirAux(std::unique_ptr<KdNode<K>> no, const std::array<double, K>& ponto, size_t prof) {
        if (!no) {
            return std::make_unique<KdNode<K>>(ponto);
        }

        size_t eixo = prof % K;

        if (ponto[eixo] < no->ponto[eixo]) {
            no->esquerda = inserirAux(std::move(no->esquerda), ponto, prof + 1);
        } else {
            no->direita = inserirAux(std::move(no->direita), ponto, prof + 1);
        }

        return no;
    }

    // Busca Exata Auxiliar
    bool pesquisarAux(const KdNode<K>* no, const std::array<double, K>& ponto, size_t prof) const {
        if (!no) return false;

        if (no->ponto == ponto) return true;

        size_t eixo = prof % K;

        if (ponto[eixo] < no->ponto[eixo]) {
            return pesquisarAux(no->esquerda.get(), ponto, prof + 1);
        } else {
            return pesquisarAux(no->direita.get(), ponto, prof + 1);
        }
    }

    // Busca de Vizinho Mais Próximo Auxiliar com Poda do Hiperplano (Branch and Bound)
    void buscarVizinhoAux(const KdNode<K>* no, const std::array<double, K>& alvo, size_t prof,
                          const KdNode<K>*& melhorNo, double& melhorDist2) const {
        if (!no) return;

        double dist2Atual = distanciaQuadrada(no->ponto, alvo);

        if (dist2Atual < melhorDist2) {
            melhorDist2 = dist2Atual;
            melhorNo = no;
        }

        size_t eixo = prof % K;
        double diffEixo = alvo[eixo] - no->ponto[eixo];

        const KdNode<K>* primeiro = (diffEixo < 0) ? no->esquerda.get() : no->direita.get();
        const KdNode<K>* segundo = (diffEixo < 0) ? no->direita.get() : no->esquerda.get();

        // 1. Explora primeiro o lado mais provável
        buscarVizinhoAux(primeiro, alvo, prof + 1, melhorNo, melhorDist2);

        // 2. Poda por hiperplano: Se a distância em 1D for menor que a melhor distância 2D encontrada até agora
        if (diffEixo * diffEixo < melhorDist2) {
            buscarVizinhoAux(segundo, alvo, prof + 1, melhorNo, melhorDist2);
        }
    }

public:
    KdTree() : raiz(nullptr) {}

    void inserir(const std::array<double, K>& ponto) {
        raiz = inserirAux(std::move(raiz), ponto, 0);
    }

    [[nodiscard]] bool pesquisar(const std::array<double, K>& ponto) const {
        return pesquisarAux(raiz.get(), ponto, 0);
    }

    // Busca pelo Ponto Geométrico Mais Próximo (Nearest Neighbor Search)
    [[nodiscard]] std::array<double, K> buscarVizinhoMaisProximo(const std::array<double, K>& alvo) const {
        if (!raiz) return {};

        const KdNode<K>* melhorNo = nullptr;
        double melhorDist2 = std::numeric_limits<double>::infinity();

        buscarVizinhoAux(raiz.get(), alvo, 0, melhorNo, melhorDist2);

        return melhorNo ? melhorNo->ponto : std::array<double, K>{};
    }
};