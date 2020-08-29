`timescale 1ns / 1ps

module RegFile(
    input CLK,
	input [1:0]RegDst,
	input RegWre,
	input WrRegDSrc,
	input [4:0] rs,
	input [4:0] rt,
	input [4:0] rd,
	input [31:0] DataToWrite,//Ҫд��Ĵ���������
    output [31:0] ReadData1,//rs
    output [31:0] ReadData2//rt
    );
	
    reg [31:0] regFile[0:31];
    integer i;
	initial for (i=0;i<=31;i=i+1) regFile[i]<=0;
    
	wire [4:0] RegToWrite;//Ҫд��ļĴ���
	assign RegToWrite=(RegDst==2'b00)?5'b11111:((RegDst==2'b10)?rd:rt);//��ѡ
	
    assign ReadData1=regFile[rs];
    assign ReadData2=regFile[rt];//read reg
    
    always@(negedge CLK) begin//write reg
         if (RegWre==1 && RegToWrite!=0)
            regFile[RegToWrite]<=DataToWrite;
    end
endmodule
