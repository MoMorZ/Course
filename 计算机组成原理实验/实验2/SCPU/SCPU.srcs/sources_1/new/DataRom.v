`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2018/11/26 08:42:18
// Design Name: 
// Module Name: DataRom
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module DataRom(
    input [31:0] Daddr,
    input [31:0] DataIn,
    input mRD,
    input mWR,
    input CLK,
    output [31:0] DataOut
    );
    reg [7:0] ram [0:31];
    integer i;
    initial begin
        for (i=0;i<=31;i=i+1) ram[i]<=0;
    end
    assign DataOut[7:0]=(mRD==1)?ram[Daddr+3]:8'bz;//read
    assign DataOut[15:8]=(mRD==1)?ram[Daddr+2]:8'bz;
    assign DataOut[23:16]=(mRD==1)?ram[Daddr+1]:8'bz;
    assign DataOut[31:24]=(mRD==1)?ram[Daddr]:8'bz;
    always@(negedge CLK) begin//write
        if (mWR==1) begin
            ram[Daddr]<= DataIn[31:24];
            ram[Daddr+1]<=DataIn[23:16];
            ram[Daddr+2]<=DataIn[15:8];
        end
    end
        );
endmodule
