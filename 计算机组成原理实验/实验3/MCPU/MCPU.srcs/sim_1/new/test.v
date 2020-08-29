`timescale 1ns / 1ps

module test();
    reg CLK;
    reg RST;
    wire [2:0] curState;//��ǰ״̬
    wire [2:0] nextState;//��һ״̬
    wire [31:0] CurPC;
    wire [31:0] newPC;
    wire [5:0] opCode;
    wire [4:0] rs;
    wire [4:0] rt;
    wire [4:0] rd;
    wire [15:0] immediate;//��16λ��������
    wire [4:0] sa;//ƫ�Ƶ�ַ
    wire [31:0] ADROut;//ADR�����
    wire [31:0] BDROut;//BDR�����
    wire [31:0] ALUDROut;
    wire [31:0] DataOut;
    wire [31:0] DBDROut;//DBDR�����
    wire [31:0] DataToWrite;//Ҫд��Ĵ���������
        
    MCPU uut(CLK,RST,curState,nextState,CurPC,newPC,opCode,rs,rt,rd,immediate,sa,ADROut,BDROut,ALUDROut,DataOut,DBDROut,DataToWrite);
    always # 50 CLK=~CLK;

    initial begin
        CLK=0;
        RST=0;
        #100 RST=1;
    end
    
endmodule
