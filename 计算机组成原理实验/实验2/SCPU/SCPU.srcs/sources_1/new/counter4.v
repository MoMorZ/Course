`timescale 1ns / 1ps
module counter4(
    input CLK,
    input Reset,
    output reg[3:0] count
    );
    //reg[3:0] count;
    initial count <= 0;
    always @(posedge CLK or negedge Reset) begin
        if(Reset == 0) begin
            count <= 4'b0000;//复位
        end
        else begin//一位位循环扫描
            if (count==4'b0000) count<=4'b1110;
            else if (count==4'b1110) count<=4'b1101;
            else if (count==4'b1101) count<=4'b1011;
            else if (count==4'b1011) count<=4'b0111;
            else if (count==4'b0111) count<=4'b1110;
        end
    end
endmodule