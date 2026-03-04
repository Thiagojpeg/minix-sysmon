#include <lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define VFS_GET_IO_STATS 0x13F 

int main(int argc, char **argv) {
    message m;
    pid_t pid;

    if (argc < 2) {
        printf("Uso: ./sysmon <PID>\n");
        return 1;
    }

    pid = atoi(argv[1]);
    m.m_lc_vfs_readwrite.fd = pid; 

    if (_syscall(VFS_PROC_NR, VFS_GET_IO_STATS, &m) != 0) {
        printf("Erro: Processo com PID %d nao encontrado ou falha no VFS.\n", pid);
        return 1;
    }

    printf("\n====================================\n");
    printf("  SysMon - Monitor de I/O (Letra C) \n");
    printf("====================================\n");
    printf(" PID Alvo        : %d\n", pid);
    printf(" Bytes Lidos     : %ld\n", m.m2_l1);
    printf(" Bytes Escritos  : %ld\n", m.m2_l2);
    printf("====================================\n");

    return 0;
}
