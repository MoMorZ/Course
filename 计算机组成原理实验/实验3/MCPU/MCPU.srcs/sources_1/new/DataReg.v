`timescale 1ns / 1ps

module DataReg(
    input CLK,
    input[31:0] DataIn,
    output reg[31:0] DataOut
    );
    always@(posedge CLK) DataOut=DataIn;
endmodule
