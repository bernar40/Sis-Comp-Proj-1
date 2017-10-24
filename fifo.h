#define FIFO_nome "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo_nome"
#define FIFO_tam "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo_tam"
#define FIFO_tempos "/home/bcruga/Documents/inf1019/proj1/Sis-Comp-Proj-1/myfifo_tempos"
#define MAX_BUF 1024

void cria_fifo(char *FIFO);

int abre_fifo_write(int fpFIFO, char * FIFO);

int abre_fifo_read(int fpFIFO, char * FIFO);