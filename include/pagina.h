#ifndef PAGINA_H
#define PAGINA_H

typedef struct {
    int pagina;               // Número da página lógica carregada
    int modificado;           // Bit de escrita (dirty)
    int referenciado;         // Bit de referência (para CLOCK)
    unsigned ultimo_acesso;   // Timestamp do último acesso (para LRU)
    unsigned frequencia;      // Contador de acessos (para LFU)
} Frame;

// Interface de manipulação dos quadros
void inicializar_quadros(int num_quadros);
void desalocar_quadros();
int buscar_pagina(int pagina);
int obter_frame_livre();
void marcar_acesso(int idx, unsigned tempo, char rw);
void carregar_pagina(int idx, int pagina, unsigned tempo, char rw);
int esta_modificado(int idx);
int obter_pagina(int idx);
Frame *obter_quadros();

#endif
