`timescale 1ns / 1ps

module InsRom(
    input [31:0] Iaddr,//IDataIn,
    input InsMemRW,
    output[31:0] InsDataOut
    );
   
    reg [7:0] rom[0:100];
    
    initial $readmemb("C:/MCPU/ins.txt",rom);

    wire [31:0] ins;
    assign ins[31:24]=rom[Iaddr];
    assign ins[23:16]=rom[Iaddr+1];
    assign ins[15:8]=rom[Iaddr+2];
    assign ins[7:0]=rom[Iaddr+3];
	
    assign InsDataOut=ins;
endmodule 