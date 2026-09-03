module;

#include <unordered_map>
#include <memory>
#include <string_view>
#include <string>

export module patricia;

export struct PatriciaNode {
    std::string prefixo; // O texto compactado desta aresta/nó
    bool fimDaPalavra = false;
    std::unordered_map<char, std::unique_ptr<PatriciaNode>> filhos;
};

export class Patricia {
private:
    std::unique_ptr<PatriciaNode> raiz;

    static int compararPrefixo(const std::string_view palavra, const std::string_view prefixo) {
        const size_t tamanho = std::min(palavra.length(), prefixo.length());
        size_t k;
        for (k = 0; k < tamanho; k++) {
            if (palavra[k] != prefixo[k]) {
                break;
            }
        }
        return k;
    }

public:
    Patricia() : raiz(std::make_unique<PatriciaNode>()) {}

    void inserir(std::string_view palavra) {
        PatriciaNode* atual = raiz.get();

        while (!palavra.empty()) {
            char letra = palavra[0];
            auto it = atual->filhos.find(letra);

            if (it == atual->filhos.end()) {
                // Caso 1: Não tem filho com essa letra -> Cria novo nó folha
                auto novo = std::make_unique<PatriciaNode>();
                novo->prefixo = std::string(palavra);
                novo->fimDaPalavra = true;
                atual->filhos[letra] = std::move(novo);
                return;
            }

            PatriciaNode* noFilho = it->second.get();
            int semelhanca = compararPrefixo(palavra, noFilho->prefixo);

            if (semelhanca == noFilho->prefixo.length()) {
                // Caso 2: Semelhança TOTAL com o nó filho
                palavra.remove_prefix(semelhanca);

                if (palavra.empty()) {
                    noFilho->fimDaPalavra = true;
                    return; // Palavra exata encontrada!
                }

                atual = noFilho; // Desce mais um nível na árvore
            }
            else {
                // Caso 3: Semelhança PARCIAL -> Split do Nó
                std::string prefixoComum = noFilho->prefixo.substr(0, semelhanca);
                std::string restoAntigo   = noFilho->prefixo.substr(semelhanca);
                std::string_view restoNovo = palavra.substr(semelhanca);

                // 1. Cria o nó com a sobra antiga e herda os filhos originais
                auto novoAntigo = std::make_unique<PatriciaNode>();
                novoAntigo->prefixo = restoAntigo;
                novoAntigo->fimDaPalavra = noFilho->fimDaPalavra;
                novoAntigo->filhos = std::move(noFilho->filhos);

                // 2. Atualiza o noFilho para ser o pai intermediário
                noFilho->prefixo = prefixoComum;
                noFilho->filhos.clear();
                noFilho->filhos[restoAntigo[0]] = std::move(novoAntigo);

                // 3. Se sobrou algo da palavra nova, cria o outro filho:
                if (!restoNovo.empty()) {
                    auto novoNovo = std::make_unique<PatriciaNode>();
                    novoNovo->prefixo = std::string(restoNovo);
                    novoNovo->fimDaPalavra = true;
                    noFilho->filhos[restoNovo[0]] = std::move(novoNovo);
                    noFilho->fimDaPalavra = false;
                } else {
                    noFilho->fimDaPalavra = true;
                }

                return; // Inserção concluída!
            }
        }
    }

    [[nodiscard]] bool pesquisar(std::string_view palavra) const {
        const PatriciaNode* atual = raiz.get();

        while (!palavra.empty()) {
            char letra = palavra[0];
            auto it = atual->filhos.find(letra);
            if (it == atual->filhos.end()) {
                return false;
            }

            const PatriciaNode* noFilho = it->second.get();
            int semelhanca = compararPrefixo(palavra, noFilho->prefixo);
            if (semelhanca < noFilho->prefixo.length()) {
                return false;
            }
            palavra.remove_prefix(semelhanca);

            if (palavra.empty()) {
                return noFilho->fimDaPalavra;
            }
            atual = noFilho;
        }
        return false;
    }

    bool remover(std::string_view palavra) {
        PatriciaNode* atual = raiz.get();

        while (!palavra.empty()) {
            char letra = palavra[0];
            auto it = atual->filhos.find(letra);

            // Palavra não encontrada
            if (it == atual->filhos.end()) {
                return false;
            }

            PatriciaNode* noFilho = it->second.get();
            int semelhanca = compararPrefixo(palavra, noFilho->prefixo);

            if (semelhanca < noFilho->prefixo.length()) {
                return false;
            }

            palavra.remove_prefix(semelhanca);

            if (palavra.empty()) {
                if (!noFilho->fimDaPalavra) {
                    return false; // A palavra não estava cadastrada
                }

                // CASO 1: O noAlvo tem 2 ou mais filhos (filhos.size() >= 2)
                if (noFilho->filhos.size() >= 2) {
                    noFilho->fimDaPalavra = false;
                    return true;
                }

                // CASO 2: O nó alvo tem EXATAMENTE 1 filho (Fusão de Nó)
                if (noFilho->filhos.size() == 1) {
                    auto itUnico = noFilho->filhos.begin();
                    
                    // Extração segura das variáveis antes de reatribuir o mapa
                    auto novosFilhos = std::move(itUnico->second->filhos);
                    std::string novoPrefixo = itUnico->second->prefixo;
                    bool novoFim = itUnico->second->fimDaPalavra;

                    noFilho->prefixo += novoPrefixo;
                    noFilho->fimDaPalavra = novoFim;
                    noFilho->filhos = std::move(novosFilhos);
                    return true;
                }

                // CASO 3: O nó alvo é uma folha (0 filhos)
                if (noFilho->filhos.empty()) {
                    atual->filhos.erase(letra);

                    // Efeito colateral no pai: funde se ficou com 1 filho só
                    if (atual->filhos.size() == 1 && !atual->fimDaPalavra && atual != raiz.get()) {
                        auto itUnico = atual->filhos.begin();
                        
                        auto novosFilhos = std::move(itUnico->second->filhos);
                        std::string novoPrefixo = itUnico->second->prefixo;
                        bool novoFim = itUnico->second->fimDaPalavra;

                        atual->prefixo += novoPrefixo;
                        atual->fimDaPalavra = novoFim;
                        atual->filhos = std::move(novosFilhos);
                    }
                    return true;
                }
            }
            atual = noFilho;
        }

        return false;
    }
};