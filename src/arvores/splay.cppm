module;

#include <memory>
#include <iostream>

export module splay;

export struct SplayNode {
    int chave;
    std::unique_ptr<SplayNode> esquerda;
    std::unique_ptr<SplayNode> direita;

    explicit SplayNode(int val)
        : chave(val), esquerda(nullptr), direita(nullptr) {}
};

export class SplayTree {
private:
    std::unique_ptr<SplayNode> raiz;

    // Rotação à Direita (Right Rotation)
    std::unique_ptr<SplayNode> rotacionarDireita(std::unique_ptr<SplayNode> p) {
        if (!p || !p->esquerda) return p;
        std::unique_ptr<SplayNode> u = std::move(p->esquerda);
        p->esquerda = std::move(u->direita);
        u->direita = std::move(p);
        return u;
    }

    // Rotação à Esquerda (Left Rotation)
    std::unique_ptr<SplayNode> rotacionarEsquerda(std::unique_ptr<SplayNode> p) {
        if (!p || !p->direita) return p;
        std::unique_ptr<SplayNode> u = std::move(p->direita);
        p->direita = std::move(u->esquerda);
        u->esquerda = std::move(p);
        return u;
    }

    // Operação Splay Recursiva (Zig, Zig-Zig, Zig-Zag)
    std::unique_ptr<SplayNode> splay(std::unique_ptr<SplayNode> no, int chave) {
        if (!no || no->chave == chave) {
            return no;
        }

        // Caso a chave buscada esteja no lado ESQUERDO
        if (chave < no->chave) {
            if (!no->esquerda) return no;

            // 1. Caso Zig-Zig (Esquerda-Esquerda)
            if (chave < no->esquerda->chave) {
                no->esquerda->esquerda = splay(std::move(no->esquerda->esquerda), chave);
                no = rotacionarDireita(std::move(no)); // Rotaciona o Avô primeiro!
            }
            // 2. Caso Zig-Zag (Esquerda-Direita)
            else if (chave > no->esquerda->chave) {
                no->esquerda->direita = splay(std::move(no->esquerda->direita), chave);
                if (no->esquerda->direita) {
                    no->esquerda = rotacionarEsquerda(std::move(no->esquerda)); // Rotaciona o Pai primeiro!
                }
            }

            // Rotação final (Zig)
            return (!no->esquerda) ? std::move(no) : rotacionarDireita(std::move(no));
        }
        // Caso a chave buscada esteja no lado DIREITO
        else {
            if (!no->direita) return no;

            // 3. Caso Zig-Zig (Direita-Direita)
            if (chave > no->direita->chave) {
                no->direita->direita = splay(std::move(no->direita->direita), chave);
                no = rotacionarEsquerda(std::move(no)); // Rotaciona o Avô primeiro!
            }
            // 4. Caso Zig-Zag (Direita-Esquerda)
            else if (chave < no->direita->chave) {
                no->direita->esquerda = splay(std::move(no->direita->esquerda), chave);
                if (no->direita->esquerda) {
                    no->direita = rotacionarDireita(std::move(no->direita)); // Rotaciona o Pai primeiro!
                }
            }

            // Rotação final (Zag)
            return (!no->direita) ? std::move(no) : rotacionarEsquerda(std::move(no));
        }
    }

public:
    SplayTree() : raiz(nullptr) {}

    void inserir(int chave) {
        if (!raiz) {
            raiz = std::make_unique<SplayNode>(chave);
            return;
        }

        // Traz o nó mais próximo ou exato para a raiz
        raiz = splay(std::move(raiz), chave);

        // Se a chave já existe, não duplica
        if (raiz->chave == chave) return;

        auto novo = std::make_unique<SplayNode>(chave);

        if (chave < raiz->chave) {
            novo->direita = std::move(raiz);
            novo->esquerda = std::move(novo->direita->esquerda);
        } else {
            novo->esquerda = std::move(raiz);
            novo->direita = std::move(novo->esquerda->direita);
        }

        raiz = std::move(novo);
    }

    [[nodiscard]] bool pesquisar(int chave) {
        if (!raiz) return false;

        // Pesquisar faz o splay do elemento para a raiz!
        raiz = splay(std::move(raiz), chave);
        return (raiz->chave == chave);
    }

    bool remover(int chave) {
        if (!raiz) return false;

        // Traz o nó alvo (ou o mais próximo) para a raiz
        raiz = splay(std::move(raiz), chave);

        // Se o elemento não existe na árvore
        if (raiz->chave != chave) {
            return false;
        }

        // Se não tem filho na esquerda, a direita vira a nova raiz
        if (!raiz->esquerda) {
            raiz = std::move(raiz->direita);
        }
        // Se não tem filho na direita, a esquerda vira a nova raiz
        else if (!raiz->direita) {
            raiz = std::move(raiz->esquerda);
        }
        // Se tem 2 filhos: desconecta a direita, faz splay do maior da esquerda e conecta
        else {
            auto subDireita = std::move(raiz->direita);
            raiz = std::move(raiz->esquerda);

            // Traz o maior elemento do ramo esquerdo para a nova raiz
            raiz = splay(std::move(raiz), chave);
            raiz->direita = std::move(subDireita);
        }

        return true;
    }
};
