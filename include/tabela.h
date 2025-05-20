#ifndef TABELA_H
#define TABELA_H

void inicializar_simulador(unsigned page_size_kb, unsigned mem_size_kb, const char *algoritmo);
void acessar_memoria(unsigned addr, char rw);
void imprimir_estatisticas(const char *arquivo, unsigned page_size_kb, unsigned mem_size_kb, const char *algoritmo);

#endif