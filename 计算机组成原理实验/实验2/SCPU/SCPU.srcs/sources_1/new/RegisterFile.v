`timescale 1ns / 1ps

module RegFile(
    input CLK,
	input RegDst,
	input DBDataSrc,
	input RegWre,
	input [4:0] rs,
	input [4:0] rt,
	input [4:0] rd,
	input [31:0] result,//ALU������
	input [31:0] DataOut,//���ݴ洢�����
    output [31:0] ReadData1,
    output [31:0] ReadData2
    );
	
    reg [31:0] regFile[0:31];
    integer i;
	initial for (i=0;i<=31;i=i+1) regFile<=0;
    
	wire [4:0] RegToWrite;//Ҫд��ļĴ���
	wire [31:0] DataToWrite;//Ҫд�������
	assign RegToWrite=(RegDst==1)?rd:rt;
	assign DataToWrite=(DBDataSrc==1)?DataOut:result;
	
    assign ReadData1=(ReadReg1==1)?regFile[ReadReg1]:0;
    assign ReadData2=(ReadReg2==1)?regFile[ReadReg2]:0;//read reg
    
    always@(negedge CLK) begin//write reg
         if (RegWre==1 && RegToWrite!=0)
            regFile[RegToWrite]<=DataToWrite;
    end
endmodule
