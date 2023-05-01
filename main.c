
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<conio.h>
#include <windows.h>
#define Rtype 0
#define Itype 1
#define Jtype 2
#ifndef MAX
#define MAX 10
#endif
struct Instruction{
    char name[10];
    char hex[8];
    int dec;
    int type;
    int opcode;
    int rs;
    int rt;
    int rd;
    int imm;
};
struct Register{
    int counter;
    int value;
};
void decToHexa(char* number, char * offsethex,char * res)
{

    int n= atoi(number);
    long unsigned int num=42994967295;
    if(n<0){
        n+=1;
        num+=n;
    }else{
        num=n;
    }
    int i = 0;
    while (num != 0) {
        int temp = 0;
        temp = num % 16;
        if (temp < 10) {
            offsethex[i] = temp + 48;
            i++;
        }
        else {
            offsethex[i] = temp + 55;
            i++;
        }
        num = num / 16;
    }
    int size= strlen(offsethex);
    int k=0;
    for(int j=size-1;j>=0 ;j--){
        res[k]=offsethex[j];
        k++;
    }
    res[k]='\0';
}
int charToint(char c){
    int x=0;
    if(c>='0'&&c<='9'){
        x=c-'0';
    }else{
        x=c-'A'+10;
    }
    return x;
}
void printRegister(struct Register aRegister[16]){
    for(int i=0;i<16;i++){
        printf("%s%d%s%d\t","Reg[",i,"]=",aRegister[i].value);
    }
    printf("\n");
}
int main() {
    HANDLE  hConsole;
    int k=3;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FILE *file;
    FILE *machp;
    char *line;
    line=(char*) malloc(100);
    size_t lineSize=100;
    int counter=0;
    int signe=0;
    char instruction[100][100];
    struct Instruction instruc[100];
    struct Register registers[16];
    for(int i=0;i<16;i++){
        registers[i].value=0;
    }
    for(int i=0;i<16;i++){
        registers[i].counter=-1;
    }
    int memory[100];
    char *name_Instruction[]={"add", "sub", "slt", "or", "nand", "addi", "slti", "ori", "lui", "lw", "sw", "beq", "jalr", "j", "halt"};
    if(file= fopen("test2.mc", "r") == NULL)
    {
        printf("%s cannot be openned\n");
        exit(1);
    }
    if((machp= fopen("test.log","w+"))==NULL){
        printf("%s cannot be oppened\n");
        exit(1);
    }
    file=fopen("test4.mc", "r");
    while (fgets(line , lineSize , file) != NULL)
    {
        strcpy(instruction[counter], line );
        //printf("%s" , instruction[counter]);
        counter++;
    }
    int num=0;
    while (num<counter){
        char hextemp[9];
        int size= strlen(hextemp);
        char res[size];
        instruc[num].dec= atoi(instruction[num]);
        strcpy(hextemp,"00000000");
        decToHexa(instruction[num],hextemp,res);
        strcpy(instruc[num].hex,res);
       // printf("%s,\n",instruc[num].hex);
        int opcode= charToint(instruc[num].hex[1]);
        instruc[num].opcode=opcode;
        if(opcode >= 0 && opcode<=4){
            strcpy(instruc[num].name,name_Instruction[opcode]);
            instruc[num].type=0;
            instruc[num].rs= charToint(instruc[num].hex[2]);
            instruc[num].rt= charToint(instruc[num].hex[3]);
            instruc[num].rd= charToint(instruc[num].hex[4]);
        }
        else if(opcode>=5 && opcode<=12){
            strcpy(instruc[num].name,name_Instruction[opcode]);
            instruc[num].type=1;
            instruc[num].rs= charToint(instruc[num].hex[2]);
            instruc[num].rt= charToint(instruc[num].hex[3]);
            int immidiate=charToint(instruc[num].hex[4])*16*16*16+charToint(instruc[num].hex[5])*16*16+charToint(instruc[num].hex[6])*16+charToint(instruc[num].hex[7]);
            if(immidiate>32768){
                immidiate-=65536;
            }
            instruc[num].imm=immidiate;
        }
        else if(opcode>=13 && opcode<=14){
            strcpy(instruc[num].name,name_Instruction[opcode]);
            instruc[num].type=2;
            int immidiate=charToint(instruc[num].hex[4])*16*16*16+charToint(instruc[num].hex[5])*16*16+charToint(instruc[num].hex[6])*16+charToint(instruc[num].hex[7]);
            if(immidiate>32768){
                immidiate-=65536;
            }
            instruc[num].imm=immidiate;
            if(opcode==14){
                signe=num;
                break;
            }
        }
        num++;
    }
    signe++;
    int numOfmemory=0;
    while (signe<counter){
        memory[signe]=atoi(instruction[signe]);
        //printf("%d,\n",memory[signe]);
        numOfmemory++;
        signe++;
    }
    int instructnum=0;
    int opCode_know=0;
    int pc=0;
    while (opCode_know!=14){
        if(instruc[pc].type == 0){
            if(instruc[pc].opcode == 0){
                opCode_know=0;
                registers[instruc[pc].rd].value= registers[instruc[pc].rs].value + registers[instruc[pc].rt].value;
                registers[instruc[pc].rd].counter=1;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            }else if(instruc[pc].opcode == 1){
                opCode_know=1;
                registers[instruc[pc].rd].value= registers[instruc[pc].rs].value - registers[instruc[pc].rt].value;
                registers[instruc[pc].rd].counter=1;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            } else if(instruc[pc].opcode == 2){
                opCode_know=2;
                if(registers[instruc[pc].rs].value < registers[instruc[pc].rt].value){
                    registers[instruc[pc].rd].value=1;
                }else{
                    registers[instruc[pc].rd].value=0;
                }
                registers[instruc[pc].rd].counter=1;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            } else if(instruc[pc].opcode == 3){
                opCode_know=3;
                registers[instruc[pc].rd].value= registers[instruc[pc].rs].value | registers[instruc[pc].rt].value;
                registers[instruc[pc].rd].counter=1;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            } else if(instruc[pc].opcode == 4){
                opCode_know=4;
                registers[instruc[pc].rd].value= ~(registers[instruc[pc].rs].value & registers[instruc[pc].rt].value);
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            }
        } else if(instruc[pc].type == 1){
            if(instruc[pc].opcode == 5){
                opCode_know=5;
                registers[instruc[pc].rt].value= registers[instruc[pc].rs].value + instruc[pc].imm;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            } else if(instruc[pc].opcode == 6){
                opCode_know=6;
                if(registers[instruc[pc].rs].value < instruc[pc].imm){
                    registers[instruc[pc].rt].value=1;
                } else{
                    registers[instruc[pc].rt].value=0;
                }
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            }else if(instruc[pc].opcode == 7){
                opCode_know=7;
                registers[instruc[pc].rt].value= registers[instruc[pc].rs].value | instruc[pc].imm;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            } else if(instruc[pc].opcode == 8){
                opCode_know=8;
                registers[instruc[pc].rt].value=instruc[pc].imm<<16;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            }else if(instruc[pc].opcode == 9){
                opCode_know=9;
                registers[instruc[pc].rt].value=memory[registers[instruc[pc].rs].value + instruc[pc].imm];
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                numOfmemory+=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            } else if(instruc[pc].opcode == 10){
                opCode_know=10;
                memory[registers[instruc[pc].rs].value + instruc[pc].imm]=registers[instruc[pc].rt].value;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
                numOfmemory+=1;
                printf("%s%s\n",instruc[pc].name,":");
                pc+=1;
            }else if(instruc[pc].opcode == 11){
                opCode_know=11;
                printf("%s%s\n",instruc[pc].name,":");
                if(registers[instruc[pc].rs].value == registers[instruc[pc].rt].value){
                    pc=pc+1+instruc[pc].imm;
                }else{
                    pc+=1;
                }
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
            } else if(instruc[pc].opcode == 12){
                opCode_know=12;
                printf("%s%s\n",instruc[pc].name,":");
                registers[instruc[pc].rt].value=pc+1;
                pc=registers[instruc[pc].rs].value;
                registers[instruc[pc].rs].counter=1;
                registers[instruc[pc].rt].counter=1;
            }
        } else if(instruc[pc].type == 2){
            if(instruc[pc].opcode == 13){
                opCode_know=13;
                printf("%s%s\n",instruc[pc].name,":");
                pc=instruc[pc].imm;
            }else if(instruc[pc].opcode == 14){
                opCode_know=14;
                printf("%s%s\n",instruc[pc].name,":");
            }
        }
        instructnum++;
        int numOfRegused=0;
        char *row1[] = {"num", "Register", "Amount"};
        int col1[] = { 1, 2, 3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
        char* col2[] = { "R0", "R1", "R2","R3","R4","R5","R6","R7","R8", "R9","R10","R11","R12","R13","R14","R15"};
        int col3[16];
        for(int i=0;i<16;i++){
            col3[i]= registers[i].value;
        }
        int k5=9;
        SetConsoleTextAttribute(hConsole, k5);
        printf("%*s  %*s  %*s\n",-3, row1[0],-MAX, row1[1], MAX, row1[2]);
        size_t len = sizeof col1 / sizeof col1[0];
        for (int i = 0; i < len; ++i) {
            SetConsoleTextAttribute(hConsole, k);
            printf("%-3d | %-10s | %10d\n", col1[i], col2[i], col3[i]);
        }

        printf("\n");
        for(int i=0;i<16;i++){
            if(registers[i].counter==1){
                numOfRegused++;
            }
        }
        double percent=(double )numOfRegused/16;
        percent*=100;
        int k2=12;
        int k3=14;
        SetConsoleTextAttribute(hConsole, k2);
        printf("%s%0.4f\n","percentOfUsedReg: ",percent);
        printf("%s%d\n","numberOfInstruction: ",instructnum);
        printf("%s%d\n","numOfMemory: ",numOfmemory);
        SetConsoleTextAttribute(hConsole, k3);
        printf("*****************************************************************************************");
        printf("\n");
        for(int i=0;i<16;i++){
            fprintf(machp,"%d\t" ,col3[i]);
        }
        fprintf(machp,"\n" );
        fprintf(machp,"%0.4f\n" ,percent);
        fprintf(machp,"%d\n" ,instructnum);
        fprintf(machp,"%d\n" ,numOfmemory);
    }
    fclose(machp);
}