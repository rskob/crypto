# ГОСТ 28147-89 (ECB)

**Учебная реализация.** Режим простой замены **небезопасен** для реальных данных.

## Быстрый старт

```bash
gcc -o gost gost.c
echo "Secret message!!!!" > text.txt
# key.txt: 64 hex символа подряд (32 байта)
echo "5A327B91E42F68C31D964EA7F5388B2C67D149B83AFC85297E14A65DC9732BE8" > key.txt
./gost
cat result.txt
