#ifndef UTIL_H
#define UTIL_H
#include <stdio.h>
unsigned obter_numero_pagina(unsigned addr, unsigned s);
unsigned calcular_shift(unsigned page_size);
void debug_print(const char *fmt, ...);
#endif
