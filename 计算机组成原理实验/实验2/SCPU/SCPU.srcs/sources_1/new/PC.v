`timescale 1ns / 1ps

module PC(
    input PCWre,
    input CLK,
    input Reset,
    input [31:0] newPC,
    output [31:0] CurPC
    );
	reg [31:0] CurPC;
    initial begin
        CurPC=0;
    end
    always@(posedge CLK or negedge Reset)begin
        if (Reset==1'b0) CurPC<=0;
        else if (PCWre) CurPC<=newPC;
    end
endmodule
