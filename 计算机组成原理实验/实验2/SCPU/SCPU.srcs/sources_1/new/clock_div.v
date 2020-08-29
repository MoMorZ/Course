`timescale 1ns / 1ps

module clock_div(
    input clk, //100MHz�� ϵͳĬ����Ƶ��
    output reg clk_sys = 0 //1Hz�� ��Ƶ���Ƶ�ʡ� �����ʼ��Ϊ 0
    );
    reg [25:0] div_counter = 0;
    always @(posedge clk) begin
        //if(div_counter>=1) begin // ���� ����
         if(div_counter>=50000) begin // �������� �������ļ� ������
            clk_sys <= ~clk_sys; // ��ƽ����
            div_counter <= 0;
        end else begin
            div_counter <= div_counter + 1;
        end
    end
endmodule