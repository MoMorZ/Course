`timescale 1ns / 1ps

 module ControlUnit(
 
         input zero,
         input sign,
         input [5:0] Opcode,
         output PCWre,
         output ALUSrcA,
         output  ALUSrcB,
         output DBDataSrc,
         output RegWre,
         output InsMemRW,
         output mRD,
         output mWR,
         output  RegDst,
         output ExtSel,
         output[1:0] PCSrc,
         output [2:0] ALUOp
     );
     
     //¿ØÖÆÐÅºÅ
	 
	 assign PCWre=(Opcode==6'b111111)?0:1;
	 assign ALUSrcA=(Opcode==6'b011000)?1:0;
	 assign ALUSrcB=(Opcode == 6'b000010 || Opcode == 6'b010000 || Opcode == 6'b010010 || Opcode == 6'b011100 || Opcode == 6'b100110 || Opcode == 6'b100111)?1:0;
     assign DBDataSrc=(Opcode == 6'b100111)?1:0;
	 assign RegWre = (Opcode == 6'b100110 || Opcode == 6'b111111 || Opcode == 6'b110010 || Opcode == 6'b110001 || Opcode == 6'b110000)?0:1;
     assign RegDst=(Opcode == 6'b000000 || Opcode == 6'b000001 || Opcode == 6'b010001 || Opcode == 6'b010011 || Opcode == 6'b011000) ? 1 : 0;
     assign ExtSel = (Opcode == 6'b010000/* || Opcode == 6'b000010 */|| Opcode == 6'b010010) ? 0 : 1;
     assign mWR = (Opcode == 6'b100110) ? 1 : 0;
     assign mRD = (Opcode == 6'b100111) ? 1 : 0;     
     //PC next address
      assign PCSrc[0] = ((Opcode == 6'b110000 && zero == 1) || (Opcode == 6'b110001 && zero == 0) || (Opcode == 6'b110010 && zero==0)) ? 1 : 0;
      assign PCSrc[1] = (Opcode == 6'b111000) ? 1 : 0;     
      //ALUOp ALU functions
      assign ALUOp[0] = (/*Opcode == 6'b110010 ||*/Opcode == 6'b000001 || Opcode == 6'b010010 || Opcode == 6'b010011 || Opcode == 6'b110000 || Opcode == 6'b110001) ? 1 : 0;
      assign ALUOp[1] = (Opcode == 6'b110010 || Opcode == 6'b010010 || Opcode == 6'b010011 || Opcode == 6'b011000 || Opcode == 6'b011100 || Opcode == 6'b111000) ? 1 : 0;
      assign ALUOp[2] = (Opcode == 6'b010000 || Opcode == 6'b010001 || Opcode == 6'b011100 || Opcode == 6'b110010) ? 1 : 0;
      
  endmodule
