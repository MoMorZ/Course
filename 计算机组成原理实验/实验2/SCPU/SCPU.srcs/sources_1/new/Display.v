`timescale 1ns / 1ps

module Display(
    input CLK,
    input Reset, // 重置按钮
    input [1:0] SW, // 选择输出信号
    input btn,// 单脉冲
    output [3:0] select, // 数码管位选择信号
    output [6:0] dispcode // 数码管输入信号
    );
       
    reg [3:0] num; // 记录当前要显示位的值
    wire [31:0] CurPC;
    wire [31:0] newPC;
    wire [5:0] opCode;
    wire [4:0] rs;
    wire [4:0] rt;
    wire [4:0] rd;
    wire [15:0] immediate;//低16位的立即数
    wire [4:0] sa;//偏移地址
    wire [31:0] Data1;//ALU的输入，寄存器的输出
    wire [31:0] Data2;//ALU的输入，寄存器的输出，数据存储器的输入
    wire [31:0] result;//ALU输出
    wire [31:0] DataOut;//存储器输出
    wire [31:0] DBout;// DB总线值
    
    wire myCLK;//分频后的时钟
    wire clk;
    
    counter4 co4(CLK,Resets,select);//仿真用
    //counter4 co4(myCLK,Reset,select);//烧板用
    Xiaodou xiaodou(CLK,btn,clk);
    clock_div clk_div(CLK,myCLK);
    SegLED segled(num,dispcode);
    SCPU scpu(clk,Reset,CurPC,newPC,opCode,rs,rt,rd,immediate,sa,Data1,Data2,result,DataOut,DBout);
    
    always@(*) begin
        case (select)
            4'b1110:begin//AN3
               case(SW)
                    2'b00:num=newPC[3:0];
                    2'b01:num=Data1[3:0];
                    2'b10:num=Data2[3:0];
                    2'b11:num=DBout[3:0];
                endcase 
            end
            4'b1101:begin//AN2
               case(SW)
                    2'b00:num=newPC[7:4];
                    2'b01:num=Data1[7:4];
                    2'b10:num=Data2[7:4];
                    2'b11:num=DBout[7:4];
                endcase 
            end
            4'b1011:begin//AN1
               case(SW)
                    2'b00:num=CurPC[3:0];
                    2'b01:num=rs[3:0];
                    2'b10:num=rt[3:0];
                    2'b11:num=result[3:0];
                endcase 
            end
            4'b0111:begin//AN0
               case(SW)
                    2'b00:num=CurPC[7:4];
                    2'b01:num={3'b000,rs[4]};
                    2'b10:num={3'b000,rt[4]};
                    2'b11:num=result[7:4];
                endcase 
            end
        endcase
    end
endmodule
