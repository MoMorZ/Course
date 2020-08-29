`timescale 1ns / 1ps

module test2;
    reg CLK;
    reg btn;// 单脉冲
    reg Reset; // 重置按钮
    reg [1:0] SW; // 选择输出信号
    wire [3:0] select; // 数码管位选择信号
    //wire [3:0] num; // 记录当前要显示位的值
    wire [6:0] dispcode; // 数码管输入信号
    reg [7:0] count;//计数器
    Display disaplay(CLK,Reset,SW,btn,select,/*num,*/dispcode);
    initial begin
        btn<=0;
        Reset<=0;
        //SW<=2'b11;输出result或DataOut
        SW<=2'b00;//输出CurPC或newPC
        CLK<=0;
        count<=0;
        #1;
        Reset<=1;
        forever #4 begin
            CLK=~CLK;
            if (count>6) begin
                count<= 0;
                btn=~btn;
            end
            else  count<=count+1;
        end
    end
endmodule