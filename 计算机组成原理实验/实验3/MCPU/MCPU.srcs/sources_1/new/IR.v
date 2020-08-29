`timescale 1ns / 1ps

module IR(
    input CLK,
    input IRWre,
    input [31:0] InsDataOut,
    output reg[5:0] op,
    output reg[4:0] rs,
    output reg[4:0] rt,
    output reg[4:0] rd,
    output reg[4:0] sa,
    output reg[15:0] imm,
    output reg[25:0] JumpAddr
    );
    always@(posedge CLK) begin
        if (IRWre==1) begin
            op=InsDataOut[31:26];
            rs=InsDataOut[25:21];
            rt=InsDataOut[20:16];
            rd=InsDataOut[15:11];
            sa=InsDataOut[10:6];
            imm=InsDataOut[15:0];
            JumpAddr=InsDataOut[25:0];
        end
    end
endmodule
