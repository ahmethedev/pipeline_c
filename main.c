#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define DATA_FILE "data.bin"
#define VALUE_SIZE sizeof(int)

int main() {
    // Pipe için file descriptors
    int pipe_fd[2];

    // Pipe oluşturuluyor
    if (pipe(pipe_fd) == -1) {
        perror("Pipe oluşturulamadı");
        exit(EXIT_FAILURE);
    }

    // Fork işlemi
    for (int i = 0; i < 7; ++i) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Fork hatası");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) { // Child process
            // Dosya açma işlemi
            int file_fd = open(DATA_FILE, O_RDONLY);
            if (file_fd == -1) {
                perror("Dosya açma hatası");
                exit(EXIT_FAILURE);
            }

            // Pipe'ı kapatılıyor (çünkü yazma işlemi yapmayacak)
            close(pipe_fd[0]);

            // Dosyadan integer okuma işlemi
            int value;

            // Child process'ler farklı değerleri okumak için seek yapar
            lseek(file_fd, i * VALUE_SIZE, SEEK_SET);

            read(file_fd, &value, VALUE_SIZE);

            // Karesini hesapla
            int square = value * value;

            // Pipe'a yazma işlemi
            write(pipe_fd[1], &square, VALUE_SIZE);

            // Dosya kapatılıyor
            close(file_fd);

            // Child process çıkış yapıyor
            exit(EXIT_SUCCESS);
        }
    }

    // Pipe'ı kapatılıyor (çünkü okuma işlemi yapmayacak)
    close(pipe_fd[1]);

    // Tüm child processlerin bitmesini bekleyen döngü
    while (wait(NULL) > 0) {}

    // Pipe'dan okuma işlemi
    int total = 0;
    int temp;
    while (read(pipe_fd[0], &temp, VALUE_SIZE) > 0) {
        total += temp;
    }

    // Pipe kapatılıyor
    close(pipe_fd[0]);

    // Toplam sonucu ekrana yazdırma
    printf("Toplam: %d\n", total);

    return 0;
}

