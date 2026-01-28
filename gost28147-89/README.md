# ГОСТ 28147-89 (ECB)

**Режим простой замены** не стоит использовать для шифрования непосредственно данных.

## Быстрый старт

```bash
gcc -o gost gost.c
echo "Secret message!!!!" > text.txt
# key.txt: 64 hex символа подряд (32 байта)
echo "ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff" > key.txt
./gost
cat result.txt
