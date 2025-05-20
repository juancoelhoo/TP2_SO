#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "pagina.h"
int substituir_pagina_random(Frame *quadros, int num_quadros);
int substituir_pagina_lru(Frame *quadros, int num_quadros);
int substituir_pagina_lfu(Frame *quadros, int num_quadros);
int substituir_pagina_clock(Frame *quadros, int num_quadros);
#endif
