`timescale 1ns / 1ps

module TwoToOne(
    input select,
    input [31:0] A,
    input [31:0] B,
    output wire [31:0] out 
    );
    assign out=(select==1)?B:A;
endmodule
