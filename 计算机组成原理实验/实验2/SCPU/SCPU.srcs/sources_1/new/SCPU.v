`timescale 1ns / 1ps


module SCPU(
    input CLK,//时钟信号
    input  Reset,//置零
    output [31:0] CurPC,//当前地址
    output [31:0] newPC,//计算出的新地址
    output [5:0] opCode,//控制单元操作码
    output [4:0] rs,
    output [4:0] rt,
    output [4:0] rd,
    output [15:0] immediate,//低16位的立即数
    output [4:0] sa,//偏移地址
    output [31:0] Data1,//ALU的输入，寄存器的输出
    output [31:0] Data2,//ALU的输入，寄存器的输出，数据存储器的输入
    output [31:0] result,//ALU的输出
    //output [31:0] Ins,//指令
    output [31:0] DataOut,//数据存储器的输出
    output [31:0] DBout//DB总线输出
    );
    
    wire zero,sign;//标志位
    wire PCWre;//PC接受，为1更改，为0不更改
    wire ALUSrcA;//ALU A端口的输入，1来自移位数sa，0来自寄存器堆输出data1
    wire ALUSrcB;//ALU B端口的输入，1来自扩展的立即数，0来自寄存器堆输出data2
    wire DBDataSrc;//写入寄存器堆的数据来源，1来自数据存储器输出，0来自ALU输出
    wire RegWre;//寄存器组;//控制单元的控制命令
    wire InsMemRW;//指令存储器读写，1为读，0为写
    wire mRD;//读数据存储器，1为读，0为输出高阻态
    wire mWR;//写数据存储器，1为写，0为无
    wire RegDst;//写寄存器组寄存器的地址，1为rd，2为rt
    wire ExtSel;//符号扩展，1为符号扩展，0为零扩展
    wire [31:0] ExtOut;//符号扩展输出的立即数
    wire [25:0] JumpAddr;//跳转地址
    wire [1:0] PCSrc;//PC的功能选择
    wire [2:0]ALUOp;//ALU的控制命令
    assign DBout=(DBDataSrc==0)?result:DataOut;

    ALU alu(ALUOp,sa,Data1,ExtOut,Data2,ALUSrcA,ALUSrcB,result,zero,sign);
    ControlUnit controlunit(zero,sign,opCode,PCWre,ALUSrcA,ALUSrcB,DBDataSrc,RegWre,InsMemRW,mRD,mWR,RegDst,ExtSel,PCSrc,ALUOp);
    DataRam dataram(result,Data2,mRD,mWR,CLK,DataOut);
    InsRom insrom(CurPC,InsMemRW,opCode,rs,rt,rd,sa,immediate,JumpAddr);
    RegFile regfile(CLK,RegDst,DBDataSrc,RegWre,rs,rt,rd,result,DataOut,Data1,Data2);
    Extend extend(ExtSel,immediate,ExtOut);
    Get_newPC get_newpc(CLK,PCSrc,CurPC,ExtOut,JumpAddr,newPC);
    PC pc(PCWre,CLK,Reset,newPC,CurPC);
endmodule
