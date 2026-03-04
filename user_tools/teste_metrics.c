#include <stdio.h>
#include <unistd.h>
#include <lib.h>    // Necessario para _syscall e message
#include <time.h>

/* * IMPORTANTE: Substitua pelo numero da syscall que voce criou no PM.
 * Se voce nao lembra, verifique no /usr/src/minix/include/minix/callnr.h
 * ou onde voce registrou a nova funcao.
 */
#define MY_SYSCALL_NR  48  // <--- TROQUE ISSO PELO SEU NUMERO

int main(int argc, char **argv) {
    message m;
    int status;
    int pid = getpid(); // Vamos monitorar o proprio processo de teste
    long start_time, end_time;
    long start_switches, end_switches;
    int i, j;

    printf("--- Iniciando Teste da Letra A (PID: %d) ---\n", pid);

    // 1. Leitura Inicial
    m.m1_i1 = pid; // Passamos o PID como argumento na mensagem
    status = _syscall(PM_PROC_NR, MY_SYSCALL_NR, &m);
    
    if (status < 0) {
        printf("ERRO: A syscall falhou. Voce usou o numero correto (%d)?\n", MY_SYSCALL_NR);
        return 1;
    }

    // Supondo que sua syscall retorne o tempo em m2_l1 e switches em m2_l2
    // Ajuste conforme voce codificou no kernel!
    start_time = m.m2_l1;
    start_switches = m.m2_l2;

    printf("Inicial -> Tempo: %ld | Switches: %ld\n", start_time, start_switches);

    // 2. Gerar Carga (Consumir CPU)
    printf("... Gerando carga de CPU ...\n");
    for (i = 0; i < 100000; i++) {
        for (j = 0; j < 50000; j++) {
            // Loop inutil apenas para gastar tempo
        }
    }

    // 3. Forcar Troca de Contexto (Sleep)
    printf("... Dormindo para forcar troca de contexto ...\n");
    sleep(1); 

    // 4. Leitura Final
    m.m1_i1 = pid;
    status = _syscall(PM_PROC_NR, MY_SYSCALL_NR, &m);

    end_time = m.m2_l1;
    end_switches = m.m2_l2;

    printf("Final   -> Tempo: %ld | Switches: %ld\n", end_time, end_switches);
    printf("------------------------------------------\n");
    printf("Diferenca: +%ld ticks | +%ld switches\n", 
            (end_time - start_time), (end_switches - start_switches));

    if (end_time > start_time && end_switches > start_switches) {
        printf("SUCESSO: Os contadores estao se movendo!\n");
    } else {
        printf("FALHA: Os valores nao mudaram. Verifique a logica no Kernel.\n");
    }

    return 0;
}
