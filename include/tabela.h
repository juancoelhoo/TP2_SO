#ifndef TABELA_H
#define TABELA_H

/**
 * Inicializa a simulação, estrutura de tabela e quadros.
 */
void inicializar_simulador(unsigned page_size_kb, unsigned mem_size_kb, const char *algoritmo);

/**
 * Realiza um acesso à memória virtual com leitura ou escrita.
 */
void acessar_memoria(unsigned addr, char rw);

/**
 * Exibe estatísticas finais da simulação.
 */
void imprimir_estatisticas(const char *arquivo, unsigned page_size_kb, unsigned mem_size_kb, const char *algoritmo);

#endif // TABELA_H
