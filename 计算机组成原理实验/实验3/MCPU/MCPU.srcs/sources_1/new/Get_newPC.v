`timescale 1ns / 1ps

module Get_newPC(
    input [1:0] PCSrc,
    input [31:0] CurPC,
    input [31:0] ExtOut,
    input [31:0] Data1,//¼Ä´æÆ÷µÄÊä³ö
    input [25:0] JumpAddr,
    output reg [31:0] newPC
    );
    wire [31:0] PC4=CurPC+4;
    always@(*) begin
        case(PCSrc)
            2'b00:newPC=PC4;
            2'b01:newPC=PC4+(ExtOut<<2);
            2'b10:newPC=Data1;
            2'b11:newPC={PC4[31:28],JumpAddr[25:0],2'b00};
        endcase
    end
endmodule
