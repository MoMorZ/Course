`timescale 1ns / 1ps


module SCPU(
    input CLK,//ʱ���ź�
    input  Reset,//����
    output [31:0] CurPC,//��ǰ��ַ
    output [31:0] newPC,//��������µ�ַ
    output [5:0] opCode,//���Ƶ�Ԫ������
    output [4:0] rs,
    output [4:0] rt,
    output [4:0] rd,
    output [15:0] immediate,//��16λ��������
    output [4:0] sa,//ƫ�Ƶ�ַ
    output [31:0] Data1,//ALU�����룬�Ĵ��������
    output [31:0] Data2,//ALU�����룬�Ĵ�������������ݴ洢��������
    output [31:0] result,//ALU�����
    //output [31:0] Ins,//ָ��
    output [31:0] DataOut,//���ݴ洢�������
    output [31:0] DBout//DB�������
    );
    
    wire zero,sign;//��־λ
    wire PCWre;//PC���ܣ�Ϊ1���ģ�Ϊ0������
    wire ALUSrcA;//ALU A�˿ڵ����룬1������λ��sa��0���ԼĴ��������data1
    wire ALUSrcB;//ALU B�˿ڵ����룬1������չ����������0���ԼĴ��������data2
    wire DBDataSrc;//д��Ĵ����ѵ�������Դ��1�������ݴ洢�������0����ALU���
    wire RegWre;//�Ĵ�����;//���Ƶ�Ԫ�Ŀ�������
    wire InsMemRW;//ָ��洢����д��1Ϊ����0Ϊд
    wire mRD;//�����ݴ洢����1Ϊ����0Ϊ�������̬
    wire mWR;//д���ݴ洢����1Ϊд��0Ϊ��
    wire RegDst;//д�Ĵ�����Ĵ����ĵ�ַ��1Ϊrd��2Ϊrt
    wire ExtSel;//������չ��1Ϊ������չ��0Ϊ����չ
    wire [31:0] ExtOut;//������չ�����������
    wire [25:0] JumpAddr;//��ת��ַ
    wire [1:0] PCSrc;//PC�Ĺ���ѡ��
    wire [2:0]ALUOp;//ALU�Ŀ�������
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
