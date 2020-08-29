`timescale 1ns / 1ps

module DataRam(
    input [31:0] Daddr,
    input [31:0] DataIn,
    input mRD,//Îª1¶Á
    input mWR,//Îª1Ð´
    output reg [31:0] DataOut
    );
    reg [7:0] ram [0:31];
    integer i;
    initial begin
        DataOut<=0;
        for (i=0;i<=31;i=i+1) ram[i]<=0;
    end
    always@(*) begin
        if (mRD==1) begin
            DataOut[7:0]=ram[Daddr+3];//read
            DataOut[15:8]=ram[Daddr+2];
            DataOut[23:16]=ram[Daddr+1];
            DataOut[31:24]=ram[Daddr];
        end
        else if (mRD==0) begin
            DataOut[31:0]=32'bz;
        end
        if (mWR==1) begin//write
            ram[Daddr+3]<=DataIn[7:0];
            ram[Daddr+2]<=DataIn[15:8];
            ram[Daddr+1]<=DataIn[23:16];
            ram[Daddr]<=DataIn[31:24];
            DataOut<=0;
        end
    end
endmodule

