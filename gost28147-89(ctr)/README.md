# GOST Block Cipher (режим гаммирования)

ГОСТ 28147-89 (64-битные блоки, 32 раунда, S-box RFC 4357).

## Быстрый старт
```bash
# Компиляция
gcc -o cipher main.c

# Создать входные файлы
echo "5A327B91E42F68C31D964EA7F5388B2C67D149B83AFC85297E14A65DC9732BE8" > key.txt
echo "0123456789abcdef" > init.txt
echo "This is my super secret message. Secret af!" > source.txt

# Запуск
./cipher
cat result.txt
```

Результат в `result.txt`.
