#ifndef PAGINA_H
#define PAGINA_H

/**
 * Estrutura que representa um quadro de memória física.
 */
typedef struct {
    int pagina;               // Número da página lógica carregada
    int modificado;           // Bit de escrita (dirty)
    int referenciado;         // Bit de referência (para CLOCK)
    unsigned ultimo_acesso;   // Timestamp do último acesso (para LRU)
    unsigned frequencia;      // Contador de acessos (para LFU)
} Frame;

// Inicializa todos os quadros de memória
void inicializar_quadros(int num_quadros);

// Libera a memória alocada para os quadros
void desalocar_quadros();

// Retorna o índice do quadro onde está a página, ou -1
int buscar_pagina(int pagina);

// Retorna o índice de um quadro livre, ou -1
int obter_frame_livre();

// Marca o acesso ao quadro (atualiza bits e contadores)
void marcar_acesso(int idx, unsigned tempo, char rw);

// Carrega uma nova página no quadro especificado
void carregar_pagina(int idx, int pagina, unsigned tempo, char rw);

// Verifica se o quadro está marcado como modificado
int esta_modificado(int idx);

// Retorna o número da página presente em um quadro
int obter_pagina(int idx);

// Retorna o vetor de quadros em uso
Frame *obter_quadros();

#endif // PAGINA_H
