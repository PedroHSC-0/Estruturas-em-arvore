module;

#include <chrono>
#include <iostream>
#include <string>
#include <functional>
#include <iomanip>

export module benchmark;

export struct ResultadoBenchmark {
    std::string nomeOperacao;
    size_t numeroOperacoes;
    double tempoTotalMs;
    double tempoMedioNs;
    double operacoesPorSegundo;
};

export class Benchmark {
public:
    template <typename Func>
    static ResultadoBenchmark medir(const std::string& nomeOperacao, size_t numOps, Func&& func) {
        auto inicio = std::chrono::high_resolution_clock::now();
        
        func();

        auto fim = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duracaoMs = fim - inicio;

        double totalMs = duracaoMs.count();
        double medioNs = (totalMs * 1'000'000.0) / numOps;
        double opsPerSec = (totalMs > 0.0) ? (static_cast<double>(numOps) / totalMs) * 1000.0 : 0.0;

        return ResultadoBenchmark{
            .nomeOperacao = nomeOperacao,
            .numeroOperacoes = numOps,
            .tempoTotalMs = totalMs,
            .tempoMedioNs = medioNs,
            .operacoesPorSegundo = opsPerSec
        };
    }

    static void imprimirResultado(const ResultadoBenchmark& res) {
        std::cout << "==================================================\n";
        std::cout << " Benchmark: " << res.nomeOperacao << "\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << " Total de Operações : " << res.numeroOperacoes << "\n";
        std::cout << " Tempo Total        : " << std::fixed << std::setprecision(3) << res.tempoTotalMs << " ms\n";
        std::cout << " Tempo Médio/Op     : " << std::fixed << std::setprecision(2) << res.tempoMedioNs << " ns\n";
        std::cout << " Throughput         : " << std::fixed << std::setprecision(0) << res.operacoesPorSegundo << " ops/sec\n";
        std::cout << "==================================================\n\n";
    }
};
