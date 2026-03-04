#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> /* Para ver o codigo do erro */
#include <string.h>

/* Forcamos a definicao aqui para ignorar o header desatualizado do sistema */
/* Se no seu callnr.h voce colocou (PM_BASE + 48), e PM_BASE eh 0, entao eh 48 */
#define SYS_MON_CALL 48 

int main(int argc, char **argv) {
    message m;
    int alvo;
    int result;

    if(argc < 2) {
        alvo = getpid();
    } else {
        alvo = atoi(argv[1]);
    }

    printf("Tentando Syscall numero: %d no alvo: %d\n", SYS_MON_CALL, alvo);

    m.m1_i1 = alvo;

    /* Chamada direta */
    result = _syscall(PM_PROC_NR, SYS_MON_CALL, &m);

    if (result < 0) {
        printf("--- FALHA ---\n");
        printf("Erro numero: %d\n", errno);
        printf("Mensagem: %s\n", strerror(errno));
        
        if (errno == 38) printf("Dica: Erro 38 (ENOSYS) significa que o PM nao reconhece a chamada 48 na tabela.\n");
        if (errno == 1)  printf("Dica: Erro 1 (EPERM) significa que o sys_getinfo falhou.\n");
        
        return 1;
    }

    printf("\n--- SUCESSO ---\n");
    printf("Trocas de Contexto: %d\n", m.m1_i1);
    printf("Tempo Total: %d\n", m.m1_i2);

    return 0;
}
