`timescale 1ns / 1ps


module MCPU(
    input CLK,//时钟信号
    input  RST,//置零
    output [2:0] curState,//当前状态
    output [2:0] nextState,//下一状态
    output [31:0] CurPC,//当前地址
    output [31:0] newPC,//计算出的新地址
    output [5:0] opCode,//控制单元操作码
    output [4:0] rs,
    output [4:0] rt,
    output [4:0] rd,
    output [15:0] immediate,//低16位的立即数
    output [4:0] sa,//偏移地址
    output [31:0] ADROut,//ADR的输出
    output [31:0] BDROut,//BDR的输出
    output [31:0] ALUDROut,//ALUDR的输出
    //output [31:0] Ins,//指令
    output [31:0] DataOut,//数据存储器的输出
    output [31:0] DBDROut,//DBDR的输出
    output [31:0] DataToWrite//要写入寄存器的数据
    );
    
    wire [31:0] Data1;//ALU的输入，寄存器的输出
    wire [31:0] Data2;//ALU的输入，寄存器的输出，数据存储器的输入
    wire zero,sign;//标志位
    wire PCWre;//PC接受，为1更改，为0不更改
    wire ALUSrcA;//ALU A端口的输入，1来自移位数sa，0来自寄存器堆输出data1
    wire ALUSrcB;//ALU B端口的输入，1来自扩展的立即数，0来自寄存器堆输出data2
    wire DBDataSrc;//写入寄存器堆的数据来源，1来自数据存储器输出，0来自ALU输出
    wire RegWre;//寄存器组;//控制单元的控制命令
    wire InsMemRW;//指令存储器读写，1为读，0为写
    wire mRD;//读数据存储器，1为读，0为输出高阻态
    wire mWR;//写数据存储器，1为写，0为无
    wire [1:0] RegDst;//写寄存器组寄存器的地址，10为rd，01为rt,00为31号
    wire ExtSel;//符号扩展，1为符号扩展，0为零扩展
    wire [31:0] ExtOut;//符号扩展输出的立即数
    wire [25:0] JumpAddr;//跳转地址
    wire [1:0] PCSrc;//PC的功能选择
    wire [2:0] ALUOp;//ALU的控制命令
    wire [31:0] InsDataOut;//InsRom的输出
    wire [31:0] DBDRIn;//DBDR的输入
    wire [31:0] result;//ALU的输出
    wire IRWre;//IR写使能
    wire WrRegDSrc;
    	
    DataReg ADR(CLK,Data1,ADROut);
    DataReg BDR(CLK,Data2,BDROut);
    DataReg ALUoutDR(CLK,result,ALUDROut);
    ALU alu(ALUOp,sa,ADROut,ExtOut,BDROut,ALUSrcA,ALUSrcB,result,zero,sign);
    
    ControlUnit controlunit(CLK,RST,zero,sign,opCode,PCWre,ALUSrcA,ALUSrcB,DBDataSrc,RegWre,WrRegDSrc,InsMemRW,mRD,mWR,IRWre,RegDst,ExtSel,PCSrc,ALUOp,curState,nextState);
    
    DataRam dataram(ALUDROut,BDROut,mRD,mWR,DataOut);
    TwoToOne ALU_Data_DB(DBDataSrc,result,DataOut,DBDRIn);
    DataReg DBDR(CLK,DBDRIn,DBDROut);
    
    InsRom insrom(CurPC,InsMemRW,InsDataOut);
    IR ir(CLK,IRWre,InsDataOut,opCode,rs,rt,rd,sa,immediate,JumpAddr);
    
    TwoToOne PC4_DB_REG(WrRegDSrc,CurPC+4,DBDROut,DataToWrite);
    RegFile regfile(CLK,RegDst,RegWre,WrRegDSrc,rs,rt,rd,DataToWrite,Data1,Data2);
    
    Extend extend(ExtSel,immediate,ExtOut);
    
    Get_newPC get_newpc(PCSrc,CurPC,ExtOut,Data1,JumpAddr,newPC);
    PC pc(PCWre,CLK,RST,newPC,CurPC);
endmodule
