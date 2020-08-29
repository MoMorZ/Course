`timescale 1ns / 1ps

module test();
    reg CLK;
    reg Reset;
    wire [31:0] CurPC;
    wire [31:0] newPC;
    wire [5:0] opCode;
    wire [4:0] rs;
    wire [4:0] rt;
    wire [4:0] rd;
    wire [15:0] immediate;//低16位的立即数
    wire [4:0] sa;//偏移地址
    wire [31:0] Data1;
    wire [31:0] Data2;
    wire [31:0] result;
    wire [31:0] DataOut;
        
    SCPU uut(CLK,Reset,CurPC,newPC,opCode,rs,rt,rd,immediate,sa,Data1,Data2,result,DataOut);
    always # 50 CLK=~CLK;

    initial begin
        CLK=0;
        Reset=0;
        #100 Reset=1;
    end
    
endmodule
