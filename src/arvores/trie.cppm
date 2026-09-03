module;

#include <unordered_map>
#include <memory>
#include <string_view>

export module trie;

export struct TrieNode {
    std::unordered_map<char, std::unique_ptr<TrieNode>> filhos;
    bool fimDaPalavra = false;
};

export class Trie {
private:
    std::unique_ptr<TrieNode> raiz;
    bool removerAux(TrieNode* atual, std::string_view palavra, int indice) {
        //Caso base: fim dos caracteres da palavra
        if (indice == palavra.size()) {
            if (!atual->fimDaPalavra) {
                return false; //A palavra não existe na Trie, era apenas um prefixo
            }
            atual->fimDaPalavra = false; //Desmarca a palavra
            return atual->filhos.empty();
        }

        char letra = palavra[indice];
        if (!atual->filhos.contains(letra)){
            return false;
        }

        bool apagarFilho = removerAux(atual->filhos[letra].get(), palavra, indice + 1);

        if (apagarFilho) {
            atual->filhos.erase(letra);
            return !atual->fimDaPalavra && atual->filhos.empty();
        }

        return false;
    }

public:
    Trie() : raiz(std::make_unique<TrieNode>()) {}

    void inserir(std::string_view palavra) {
        TrieNode* atual = raiz.get();
        for (char letra : palavra) {
            if (!atual->filhos.contains(letra)) {
                atual->filhos[letra] = std::make_unique<TrieNode>();
            }
            atual = atual->filhos[letra].get();
        }
        atual->fimDaPalavra = true;
    }

    [[nodiscard]] bool comecaCom(const std::string_view palavra) const {
        const TrieNode* atual = raiz.get();
        for (char letra : palavra) {
            auto it = atual->filhos.find(letra);

            if (it == atual->filhos.end()) return false; //Letra não encontrada: sufixo não existe
            atual = it->second.get();
        }
        return true;
    }

    [[nodiscard]] bool pesquisar(std::string_view palavra) const {
        const TrieNode* atual = raiz.get();
        for (char letra : palavra) {
            auto it = atual->filhos.find(letra);
            if (it == atual->filhos.end()) {
                return false;
            }
            atual = it->second.get();
        }
        return atual->fimDaPalavra;
    }

    bool remover(std::string_view palavra) {
        if (palavra.empty()) return false;
        return removerAux(raiz.get(), palavra, 0);
    }
};