#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *const ALPHA[] = {
    ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--..", // Z
};

static const char *const NUMBER[] = {
    "-----", // 0
    ".----", // 1
    "..---", // 2
    "...--", // 3
    "....-", // 4
    ".....", // 5
    "-....", // 6
    "--...", // 7
    "---..", // 8
    "----.", // 9
};

int isAlpha(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
int isNumber(char c) { return (c >= '0' && c <= '9'); }
int validate(char c) { return isAlpha(c) || isNumber(c); }
char toUpper(char c) { return c >= 'a' && c <= 'z' ? c - 32 : c; }

const char *encodeCharacter(char c) {
    if (isAlpha(c)) {
        return ALPHA[toUpper(c) - 'A'];
    } else if (isNumber(c)) {
        return NUMBER[c - '0'];
    }
    return "";
}

char *encodeString(char *str) {
    size_t len = strlen(str);
    char *encodedStr = (char *)malloc((4 + 1) * len);
    if (encodedStr == NULL) {
        printf("Error while allocating memory");
        return NULL;
    }

    encodedStr[0] = '\0';

    for (; *str != '\0'; ++str) {
        if (!validate(*str)) {
            fprintf(stderr, "Invalid character: %c. Only a-z, A-Z, 0-9\n",
                    *str);
            free(encodedStr);
            return NULL;
        }

        const char *encoded = encodeCharacter(*str);
        strcat(encodedStr, encoded);
        strcat(encodedStr, " ");
    }

    return encodedStr;
}

char decodeToCharacter(char *str) {
    for (int i = 0; i < 26; i++) {
        if (strcmp(str, ALPHA[i]) == 0) {
            return 'A' + i;
        }
    }

    for (int i = 0; i < 10; i++) {
        if (strcmp(str, NUMBER[i]) == 0) {
            return '0' + i;
        }
    }

    return '\0';
}

char *decodeToString(char *str) {

    size_t len = strlen(str);
    char *decodedStr = (char *)malloc(len + 1);
    if (decodedStr == NULL) {
        printf("Error while allocating memory");
        return NULL;
    }

    decodedStr[0] = '\0';

    char *copy = strdup(str);
    if (copy == NULL) {
        free(decodedStr);
        return NULL;
    }
    char *token = strtok(copy, " ");
    while (token != NULL) {
        char decodedChar = decodeToCharacter(token);
        if (decodedChar != '\0') {
            strncat(decodedStr, &decodedChar, 1);
        } else {
            printf("Invalid Morse code sequence: %s\n", token);
            free(decodedStr);
            return NULL;
        }
        token = strtok(NULL, " ");
    }

    return decodedStr;
}

int main(int argc, char **argv) {
    if (argc != 3 || (strcmp(argv[1], "--encode") != 0 &&
                      strcmp(argv[1], "--decode") != 0)) {
        printf("Usage: morse --encode <string> or morse --decode <string>\n");
        return 1;
    }

    if (strcmp(argv[1], "--encode") == 0) {
        char *encoded = encodeString(argv[2]);
        if (encoded != NULL) {
            printf("%s\n", encoded);
            free(encoded);
        }
    } else if (strcmp(argv[1], "--decode") == 0) {
        char *decoded = decodeToString(argv[2]);
        if (decoded != NULL) {
            printf("%s\n", decoded);
            free(decoded);
        }
    }

    return 0;
}
