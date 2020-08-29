#define Debug
extern void FuncPrintSL();

int NEW = 0;
int READY = 1;
int RUNNING = 2;
int EXIT = 3;

typedef struct PCB {
	int IP;
	int CS;
	int FLAGS;
	int SS;
	int ES;
	int DS;
	int SI;
	int BP;
	int SP;
	int BX;
	int DX;
	int CX;
	int AX;
} PCB;



PCB pcb_list[8];
PCB* Cur_PCB = &pcb_list[0];
int CurrentPCBno = 0;
int Program_Num = 0;

void init(PCB*, int, int);
void Schedule();

void in_test() {
	FuncPrintSL("Ready to init...", 16);
}

void sc_test() {
	FuncPrintSL("Ready to schedule...", 20);
}

void rs_test() {
	FuncPrintSL("Ready to restore...", 19);
}

void rb_test() {
	FuncPrintSL("Ready to restart...", 19);
}

void rb_test2() {
	FuncPrintSL("End restart...", 14);
}

void sv_test() {
	FuncPrintSL("Ready to save...", 16);
}

void Schedule() {
#ifdef Debug
	FuncPrintSL("schedule", 8);
#endif
	CurrentPCBno++;
	if (CurrentPCBno > Program_Num) CurrentPCBno = 1;
	Cur_PCB = &pcb_list[CurrentPCBno];
	return;
}


void init(PCB* pcb, int segement, int offset) {
	pcb->SS = segement;
	pcb->ES = segement;
	pcb->DS = segement;
	pcb->CS = segement;
	pcb->IP = offset;
	pcb->SP = offset - 4;
	pcb->AX = 0;
	pcb->BX = 0;
	pcb->CX = 0;
	pcb->DX = 0;
	pcb->SI = 0;
	pcb->BP = 0;
	pcb->FLAGS = 512;
}