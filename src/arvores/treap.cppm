module;

#include <algorithm>
#include <memory>
#include <random>

export module treap;

export struct TreapNode {
    int chave;
    int prioridade;
    std::unique_ptr<TreapNode> esquerda;
    std::unique_ptr<TreapNode> direita;

    TreapNode(int val, int prio)
        : chave(val), prioridade(prio), esquerda(nullptr), direita(nullptr) {}
};

export class Treap {
private:
    std::unique_ptr<TreapNode> raiz;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist;

    // TODO: Implementar Rotação à Direita (Right Rotation)
    std::unique_ptr<TreapNode> rotacionarDireita(std::unique_ptr<TreapNode> p) {
        // Move o filho da esquerda para a posição de pai e restaura a propriedade da Heap
        //       p (Pai Antigo)                  u (Novo Pai)
        //      / \                             / \
        //     u   C            --->           A   p
        //    / \                                 / \
        //   A   B (Sub-árvore do meio)          B   C
        std::unique_ptr<TreapNode> u = std::move(p->esquerda);
        p->esquerda = std::move(u->direita);
        u->direita = std::move(p);
        return u;
    }

    // TODO: Implementar Rotação à Esquerda (Left Rotation)
    std::unique_ptr<TreapNode> rotacionarEsquerda(std::unique_ptr<TreapNode> p) {
        // Move o filho da direita para a posição de pai e restaura a propriedade da Heap
        //       p (Pai Antigo)                   u (Novo Pai)
        //      / \                              / \
        //     A   u              --->          p   C
        //        / \                          / \
        //       B   C                        A   B
        std::unique_ptr<TreapNode> u = std::move(p->direita);
        p->direita = std::move(u->esquerda);
        u->esquerda = std::move(p);
        return u;
    }

    // TODO: Implementar Inserção Recursiva com Rotação
    std::unique_ptr<TreapNode> inserirAux(std::unique_ptr<TreapNode> no, int chave, int prioridade) {
        // 1. Se o nó for nulo, cria e retorna o novo TreapNode(chave, pri   no->esquerda = inserirAux(std::move(no->esquerda), chave, prioridade);oridade)
        if (!no) {
            return std::make_unique<TreapNode>(chave, prioridade);
        }

        // 2. Se chave < no->chave: insere na esquerda e verifica se precisa de rotacionarDireita
        if (chave < no->chave) {
            no->esquerda = inserirAux(std::move(no->esquerda), chave, prioridade);
            // Na volta da recursão: Se a prioridade da esquerda ficou maior, rotaciona à DIREITA!
            if (no->esquerda->prioridade > no->prioridade) {
                no = rotacionarDireita(std::move(no));
            }
        }
        // 3. Se chave > no->chave: insere na direita e verifica se precisa de rotacionarEsquerda
        else if (chave > no->chave) {
            no->direita = inserirAux(std::move(no->direita), chave, prioridade);
            // Na volta da recursão: Se a prioridade da direita ficou maior, rotaciona à ESQUERDA!
            if (no->direita->prioridade > no->prioridade) {
                no = rotacionarEsquerda(std::move(no));
            }
        }

        return no;
    }

    std::unique_ptr<TreapNode> removerAux(std::unique_ptr<TreapNode> no, int chave, bool& removido) {
        // Chegamos ao fim da árvore e não encontramos a chave;
        if (!no) {
            removido = false;
            return nullptr;
        }

        if (chave < no->chave) {
            no->esquerda = removerAux(std::move(no->esquerda), chave, removido);
        }

        else if (chave > no->chave) {
            no->direita = removerAux(std::move(no->direita), chave, removido);
        }

        else {
            removido = true;

            // O nó não tem nenhum filho, retornamos nulo
            if (!no->esquerda && !no->direita) {
                return nullptr;
            }

            // O nó tem apenas um filho, 2 casos possíveis
            if (!no->esquerda) return std::move(no->direita);
            if (!no->direita)  return std::move(no->esquerda);

            // O nó tem 2 filhos
            if (no->esquerda->prioridade >= no->direita->prioridade) {
                no = rotacionarDireita(std::move(no));
                no->direita = removerAux(std::move(no->direita), chave, removido);
            }
            else {
                no = rotacionarEsquerda(std::move(no));
                no->esquerda = removerAux(std::move(no->esquerda), chave, removido);
            }
        }
        return no;
    }

public:
    Treap(uint32_t seed = 42) : raiz(nullptr), rng(seed), dist(1, 1'000'000) {}

    void inserir(int chave) {
        int prioridade = dist(rng); // Prioridade gerada aleatoriamente
        raiz = inserirAux(std::move(raiz), chave, prioridade);
    }

    [[nodiscard]] bool pesquisar(int chave) const {
        const TreapNode* atual = raiz.get();
        while (atual) {
            if (chave == atual->chave) return true;
            if (chave < atual->chave) atual = atual->esquerda.get();
            else atual = atual->direita.get();
        }
        return false;
    }

    bool remover(int chave) {
        bool removido = false;
        raiz = removerAux(std::move(raiz), chave, removido);
        return removido;
    }
};
