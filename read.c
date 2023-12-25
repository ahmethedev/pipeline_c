#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define DATA_FILE "data.bin"

int main() {
    // Dosya açma işlemi
    int file_fd = open(DATA_FILE, O_RDONLY);
    if (file_fd == -1) {
        perror("Dosya açma hatası");
        exit(EXIT_FAILURE);
    }

    // Dosyadan integer değerleri okuma işlemi
    int value;
    while (read(file_fd, &value, sizeof(int)) > 0) {
        printf("%d\n", value);
    }

    // Dosya kapatılıyor
    close(file_fd);

    return 0;
}

