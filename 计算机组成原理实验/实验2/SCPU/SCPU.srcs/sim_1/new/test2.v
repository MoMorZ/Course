`timescale 1ns / 1ps

module test2;
    reg CLK;
    reg btn;// ������
    reg Reset; // ���ð�ť
    reg [1:0] SW; // ѡ������ź�
    wire [3:0] select; // �����λѡ���ź�
    //wire [3:0] num; // ��¼��ǰҪ��ʾλ��ֵ
    wire [6:0] dispcode; // ����������ź�
    reg [7:0] count;//������
    Display disaplay(CLK,Reset,SW,btn,select,/*num,*/dispcode);
    initial begin
        btn<=0;
        Reset<=0;
        //SW<=2'b11;���result��DataOut
        SW<=2'b00;//���CurPC��newPC
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