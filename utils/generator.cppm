module;

#include <vector>
#include <string>
#include <string_view>
#include <random>
#include <algorithm>
#include <array>
#include <cstddef>

export module generator;

export class GeradorDeEntradas {
private:
    std::mt19937_64 rng;

public:
    GeradorDeEntradas(uint64_t seed = 42) : rng(seed) {}

    // 1. Gerador de Inteiros Aleatórios (para Treap, Splay, BST)
    std::vector<int> gerarInteirosAleatorios(size_t quantidade, int min = 1, int max = 1'000'000) {
        std::uniform_int_distribution<int> dist(min, max);
        std::vector<int> resultado(quantidade);
        for (size_t i = 0; i < quantidade; ++i) {
            resultado[i] = dist(rng);
        }
        return resultado;
    }

    // 2. Gerador de Inteiros Sequenciais (Excelente para testar pior caso de BST vs Splay/Treap)
    static std::vector<int> gerarInteirosSequenciais(const size_t quantidade, const int inicio = 1) {
        std::vector<int> resultado(quantidade);
        for (size_t i = 0; i < quantidade; ++i) {
            resultado[i] = inicio + static_cast<int>(i);
        }
        return resultado;
    }

    // 3. Gerador com Localidade de Referência (80% dos acessos focados em 20% dos elementos)
    // Excelente para demonstrar a vantagem da Splay Tree!
    std::vector<int> gerarAcessosComLocalidade(size_t totalAcessos, size_t universoElementos) {
        std::uniform_int_distribution<size_t> distPorcento(1, 100);
        std::uniform_int_distribution<size_t> distHot(1, std::max<size_t>(1, universoElementos / 5));
        std::uniform_int_distribution<size_t> distCold(universoElementos / 5 + 1, universoElementos);

        std::vector<int> resultado(totalAcessos);
        for (size_t i = 0; i < totalAcessos; ++i) {
            if (distPorcento(rng) <= 80) {
                resultado[i] = static_cast<int>(distHot(rng));
            } else {
                resultado[i] = static_cast<int>(distCold(rng));
            }
        }
        return resultado;
    }

    // 4. Gerador de Strings Aleatórias (para Trie, Patricia, Radix Tree)
    std::vector<std::string> gerarStringsAleatorias(size_t quantidade, size_t tamanhoMin = 3, size_t tamanhoMax = 10) {
        static const char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
        std::uniform_int_distribution<size_t> distTam(tamanhoMin, tamanhoMax);
        std::uniform_int_distribution<size_t> distChar(0, sizeof(alfabeto) - 2);

        std::vector<std::string> resultado(quantidade);
        for (size_t i = 0; i < quantidade; ++i) {
            size_t tam = distTam(rng);
            std::string str;
            str.reserve(tam);
            for (size_t j = 0; j < tam; ++j) {
                str.push_back(alfabeto[distChar(rng)]);
            }
            resultado[i] = str;
        }
        return resultado;
    }

    // 5. Gerador de Strings com Prefixo Comum (para testar compactação em Patricia/Radix)
    std::vector<std::string> gerarStringsComPrefixo(size_t quantidade, std::string_view prefixo, size_t tamanhoSufixo = 5) {
        auto sufixos = gerarStringsAleatorias(quantidade, tamanhoSufixo, tamanhoSufixo);
        std::vector<std::string> resultado;
        resultado.reserve(quantidade);
        for (auto& sufixo : sufixos) {
            resultado.push_back(std::string(prefixo) + sufixo);
        }
        return resultado;
    }

    // 6. Gerador de Pontos N-Dimensionais (para KD-Tree)
    template <size_t N>
    std::vector<std::array<double, N>> gerarPontosND(size_t quantidade, double min = 0.0, double max = 1000.0) {
        std::uniform_real_distribution<double> dist(min, max);
        std::vector<std::array<double, N>> resultado(quantidade);
        for (size_t i = 0; i < quantidade; ++i) {
            for (size_t d = 0; d < N; ++d) {
                resultado[i][d] = dist(rng);
            }
        }
        return resultado;
    }
};
