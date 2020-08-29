`timescale 1ns / 1ps

module ALU(
    input [2:0] ALUOp,
	input [4:0] sa,
    input [31:0] ReadData1,
	input [31:0] ExtOut,
    input [31:0] ReadData2,
	input ALUSrcA,
	input ALUSrcB,
    output [31:0] result,
    output zero,//zero flag
    output sign//sign flag    
    );
	
	reg zero,sign;
	reg [31:0] result;
	wire [31:0] rega;
	wire [31:0] regb;
	assign rega=(ALUSrcA==1)?sa:ReadData1;//双选，根据ALUSRcA状态选择
	assign regb=(ALUSrcB==1)?ExtOut:ReadData2;//双选，根据ALUSRcB状态选择
    
    always@(*) begin
        case (ALUOp)
            3'b000:result<=rega+regb;
            3'b001:result<=rega-regb;
            3'b010:result<=(regb<<rega);
            3'b011:result<=regb|rega;
            3'b100:result<=regb&rega;
            3'b101:result<=(rega<regb)?1:0;
            3'b110:result<=(((rega<regb) && (rega[31] == regb[31] )) ||( ( rega[31] ==1 && regb[31] == 0))) ? 1:0;
            3'b111:result<=regb^rega;
            default:begin
                    result=32'h00000000;
                    $display("no match");
                end
        endcase
        zero=(result==0)?1:0;
        sign=result[31];
    end
endmodule