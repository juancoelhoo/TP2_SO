#ifndef SIMULADOR_H
#define SIMULADOR_H

/**
 * Função principal de simulação.
 * 
 * @param algoritmo Algoritmo de substituição (ex: "lru", "lfu", etc.)
 * @param arquivo Caminho para o arquivo .log com os acessos
 * @param tam_pagina_kb Tamanho da página em kilobytes
 * @param tam_memoria_kb Tamanho da memória física em kilobytes
 * @param debug Ativa ou desativa saída de depuração
 */
void simular(const char *algoritmo, const char *arquivo, unsigned int tam_pagina_kb, unsigned int tam_memoria_kb, int debug);

#endif // SIMULADOR_H
