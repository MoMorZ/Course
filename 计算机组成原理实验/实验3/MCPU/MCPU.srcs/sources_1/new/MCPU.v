`timescale 1ns / 1ps


module MCPU(
    input CLK,//ʱ���ź�
    input  RST,//����
    output [2:0] curState,//��ǰ״̬
    output [2:0] nextState,//��һ״̬
    output [31:0] CurPC,//��ǰ��ַ
    output [31:0] newPC,//��������µ�ַ
    output [5:0] opCode,//���Ƶ�Ԫ������
    output [4:0] rs,
    output [4:0] rt,
    output [4:0] rd,
    output [15:0] immediate,//��16λ��������
    output [4:0] sa,//ƫ�Ƶ�ַ
    output [31:0] ADROut,//ADR�����
    output [31:0] BDROut,//BDR�����
    output [31:0] ALUDROut,//ALUDR�����
    //output [31:0] Ins,//ָ��
    output [31:0] DataOut,//���ݴ洢�������
    output [31:0] DBDROut,//DBDR�����
    output [31:0] DataToWrite//Ҫд��Ĵ���������
    );
    
    wire [31:0] Data1;//ALU�����룬�Ĵ��������
    wire [31:0] Data2;//ALU�����룬�Ĵ�������������ݴ洢��������
    wire zero,sign;//��־λ
    wire PCWre;//PC���ܣ�Ϊ1���ģ�Ϊ0������
    wire ALUSrcA;//ALU A�˿ڵ����룬1������λ��sa��0���ԼĴ��������data1
    wire ALUSrcB;//ALU B�˿ڵ����룬1������չ����������0���ԼĴ��������data2
    wire DBDataSrc;//д��Ĵ����ѵ�������Դ��1�������ݴ洢�������0����ALU���
    wire RegWre;//�Ĵ�����;//���Ƶ�Ԫ�Ŀ�������
    wire InsMemRW;//ָ��洢����д��1Ϊ����0Ϊд
    wire mRD;//�����ݴ洢����1Ϊ����0Ϊ�������̬
    wire mWR;//д���ݴ洢����1Ϊд��0Ϊ��
    wire [1:0] RegDst;//д�Ĵ�����Ĵ����ĵ�ַ��10Ϊrd��01Ϊrt,00Ϊ31��
    wire ExtSel;//������չ��1Ϊ������չ��0Ϊ����չ
    wire [31:0] ExtOut;//������չ�����������
    wire [25:0] JumpAddr;//��ת��ַ
    wire [1:0] PCSrc;//PC�Ĺ���ѡ��
    wire [2:0] ALUOp;//ALU�Ŀ�������
    wire [31:0] InsDataOut;//InsRom�����
    wire [31:0] DBDRIn;//DBDR������
    wire [31:0] result;//ALU�����
    wire IRWre;//IRдʹ��
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
