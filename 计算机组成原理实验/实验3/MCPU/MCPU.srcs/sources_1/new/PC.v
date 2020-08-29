`timescale 1ns / 1ps

module PC(
    input PCWre,
    input CLK,
    input RST,
    input [31:0] newPC,
    output reg [31:0] CurPC
    );
    initial begin
        CurPC=0;
    end
    always@(posedge CLK or negedge RST)begin
        if (RST==1'b0) CurPC<=0;
        else if (PCWre) CurPC<=newPC;
    end
endmodule
