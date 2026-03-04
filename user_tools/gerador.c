#include <stdio.h>
#include <unistd.h>

int main() {
    FILE *f;
    int i;
    
    printf("\n[+] Iniciei! Meu PID eh: %d\n", getpid());
    printf("[+] Vou escrever em 'teste_io.txt' por 50 segundos...\n\n");
    
    f = fopen("teste_io.txt", "w");
    if (!f) return 1;

    for(i = 0; i < 50; i++) {
        fprintf(f, "Gerando I/O - Linha %d\n", i);
        fflush(f); /* Força a gravação no disco agora */
        sleep(1);  /* Pausa 1 segundo para você ter tempo de rodar o sysmon */
    }
    
    fclose(f);
    printf("[+] Terminei!\n");
    return 0;
}

