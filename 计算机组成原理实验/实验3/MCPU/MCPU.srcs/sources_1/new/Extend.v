`timescale 1ns / 1ps

module Extend(
    input ExtSel,
    input [15:0] imm,
    output [31:0] ExtOut
    );
    reg [31:0] ExtOut;
	always@(*) begin
		if (ExtSel==0) ExtOut={16'h0000,imm};
		else ExtOut=(imm[15]==0)?{16'h0000,imm}:{16'hffff,imm};
	end
endmodule
