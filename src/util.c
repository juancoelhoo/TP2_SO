#include "util.h"
#include <stdarg.h>

static int modo_debug_ativo = 0;
void set_debug(int val) { modo_debug_ativo = val; }

unsigned obter_numero_pagina(unsigned addr, unsigned s) {
    return addr >> s;
}

unsigned calcular_shift(unsigned page_size) {
    unsigned tmp = page_size;
    unsigned s = 0;
    while (tmp > 1) {
        tmp >>= 1;
        s++;
    }
    return s;
}

void debug_print(const char *fmt, ...) {
    if (modo_debug_ativo) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
}
