#!/bin/bash

EXEC=./bin/simulador
LOG_DIR=logs
PAGE_SIZE=4
MEM_SIZE=1024

ALGORITMOS=("lru" "lfu" "random" "clock")
ESTRUTURAS=("" "-2n" "-3n" "-inv")
ARQUIVOS=("compilador.log" "matriz.log" "compressor.log" "simulador.log")

mkdir -p resultados

echo "Executando testes..."
for alg in "${ALGORITMOS[@]}"; do
  for estrutura in "${ESTRUTURAS[@]}"; do
    for arquivo in "${ARQUIVOS[@]}"; do
      nome=$(basename "$arquivo" .log)
      nome_saida="resultados/${alg}${estrutura}_${nome}.txt"
      echo "Teste: $alg$estrutura com $arquivo" | tee "$nome_saida"
      $EXEC "$alg$estrutura" "$LOG_DIR/$arquivo" $PAGE_SIZE $MEM_SIZE >> "$nome_saida"
      echo -e "---------------------------------------\n" >> "$nome_saida"
    done
  done
done

echo "Todos os testes foram executados. Resultados disponíveis em ./resultados/"
