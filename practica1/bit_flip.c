#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Modelo NRZ de la practica: 1 -> +5 V y 0 -> -5 V.
void transmitir_bit_nrz(int bit) {
    if (bit == 1) {
        printf("[ +5V ] ");
    } else {
        printf("[ -5V ] ");
    }
}

// Recorre cada byte del bit mas significativo al menor; AND con 1 aisla el bit.
void mostrar(unsigned char palabra[], int n) {
    for (int i = 0; i < n; i++) {
        printf("Byte %d (%u)\nBits: ", i + 1, (unsigned int)palabra[i]);
        for (int j = 7; j >= 0; j--) {
            printf("%d", (palabra[i] >> j) & 1);
        }
        printf("\nSenal: ");
        for (int j = 7; j >= 0; j--) {
            transmitir_bit_nrz((palabra[i] >> j) & 1);
        }
        printf("\n");
    }
}

int main(void) {
    // Los bytes sin signo permiten manipular los ocho bits sin extension de signo.
    unsigned char original[102], recibido[102];
    printf("Introduce una palabra ASCII de 1 a 100 caracteres, sin espacios: ");
    if (fgets((char *)original, sizeof original, stdin) == NULL) {
        fprintf(stderr, "Error: no se recibio una palabra.\n");
        return 1;
    }
    int n = (int)strlen((char *)original);
    if (n > 0 && original[n - 1] == '\n') {
        original[--n] = '\0';
    }
    if (n < 1 || n > 100) {
        fprintf(stderr, "Error: usa entre 1 y 100 caracteres.\n");
        return 1;
    }

    // ASCII visible garantiza un byte por caracter y excluye espacios y controles.
    for (int i = 0; i < n; i++) {
        if (original[i] < 33 || original[i] > 126) {
            fprintf(stderr, "Error: usa ASCII visible, sin espacios ni acentos.\n");
            return 1;
        }
    }

    memcpy(recibido, original, n);
    // La semilla tiene resolucion de segundos; dos ejecuciones pueden repetir la seleccion.
    srand((unsigned int)time(NULL));

    // La posicion global se convierte a byte y desplazamiento, contando desde el bit mayor.
    int posicion = rand() % (n * 8);
    int indice = posicion / 8;
    int bit = 7 - posicion % 8;
    // XOR con una mascara de un solo 1 invierte exactamente un bit.
    recibido[indice] ^= (1u << bit);

    printf("\nTexto transmitido: %s\n\nORIGINAL\n", (char *)original);
    mostrar(original, n);
    printf("\nBit alterado: %d de %d (byte %d, peso %u)\n", posicion + 1, n * 8, indice + 1, 1u << bit);
    printf("\nRECIBIDO\n");
    mostrar(recibido, n);

    // Se recorre por longitud: un byte cero no debe cortar el mensaje recibido.
    // Los bytes no imprimibles se muestran como \xHH para conservar su valor.
    printf("\nTexto recibido: ");
    for (int i = 0; i < n; i++) {
        if (recibido[i] >= 32 && recibido[i] <= 126) {
            printf("%c", recibido[i]);
        } else {
            printf("\\x%02X", (unsigned int)recibido[i]);
        }
    }
    printf("\nBits alterados: 1 de %d\n", n * 8);
    return 0;
}
