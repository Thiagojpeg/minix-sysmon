#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* Usando os números brutos para evitar problemas com arquivos .h não atualizados */
#define VM_PROC_NR 8
#define VM_GET_MEM_STATS 0xC37 /* 0xC00 (VM_RQ_BASE) + 55 */

int main() {
    message m;
    int i, status;
    char *ptr[10];

    printf("--- Iniciando Teste de Memoria ---\n");

    memset(&m, 0, sizeof(m));
    
    /* Envia a mensagem para o VM */
    status = _syscall(VM_PROC_NR, VM_GET_MEM_STATS, &m);
    
    if (status != 0) {
        printf("Falha na syscall! Status: %d | Erro (errno): %d - %s\n", status, errno, strerror(errno));
        
        if (errno == ENOSYS) {
            printf(">> DIAGNOSTICO: O VM recebeu a mensagem, mas rejeitou. Motivo: 'ENOSYS'.\n");
            printf(">> Isso significa que o NR_VM_CALLS continuou 49 em vez de 56, ou o ACL falhou.\n");
        }
        return 1;
    }

    printf("Inicio -> Alocadas: %ld | Liberadas: %ld\n", m.m2_l1, m.m2_l2);

    /* Alocando para forçar o Page Fault */
    printf("... Alocando e usando memoria ...\n");
    for(i = 0; i < 10; i++) {
        ptr[i] = malloc(4096);
        if (ptr[i]) ptr[i][0] = 'X'; 
    }

    memset(&m, 0, sizeof(m));
    _syscall(VM_PROC_NR, VM_GET_MEM_STATS, &m);
    printf("Meio   -> Alocadas: %ld | Liberadas: %ld\n", m.m2_l1, m.m2_l2);

    /* Liberando */
    printf("... Liberando memoria ...\n");
    for(i = 0; i < 10; i++) {
        free(ptr[i]);
    }

    memset(&m, 0, sizeof(m));
    _syscall(VM_PROC_NR, VM_GET_MEM_STATS, &m);
    printf("Fim    -> Alocadas: %ld | Liberadas: %ld\n", m.m2_l1, m.m2_l2);

    return 0;
}

