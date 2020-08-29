`timescale 1ns / 1ps

module InsRom(
    input [31:0] Iaddr,//IDataIn,
    input InsMemRW,
    output[5:0] op,
    output[4:0] rs,
    output[4:0] rt,
    output[4:0] rd,
    output[4:0] sa,
    output[15:0] imm,
    output[25:0] JumpAddr
    );
   
    reg [7:0] rom[0:100];
    
    initial begin $readmemb("C:/MCPU/instructions.txt",rom);end

    wire [31:0] ins;
    assign ins[31:24]=rom[Iaddr];
    assign ins[23:16]=rom[Iaddr+1];
    assign ins[15:8]=rom[Iaddr+2];
    assign ins[7:0]=rom[Iaddr+3];
	
    
    assign op=ins[31:26];
    assign rs=ins[25:21];
    assign rt=ins[20:16];
    assign rd=ins[15:11];
    assign sa=ins[10:6];
    assign imm=ins[15:0];
    assign JumpAddr=ins[25:0];

endmodule

/*
    // addiu $1, $0, 8  08010008
    assign rom[0] = 8'h08;
    assign rom[1] = 8'h01;
    assign rom[2] = 8'h00;
    assign rom[3] = 8'h08;
    
    // ori $2. $0, 2  48020002?
    assign rom[4] = 8'h48;
    assign rom[5] = 8'h02;
    assign rom[6] = 8'h00;
    assign rom[7] = 8'h02;
    
    // add $3, $2, $1  00411800
    assign rom[8] = 8'h00;
    assign rom[9] = 8'h41;
    assign rom[10] = 8'h18;
    assign rom[11] = 8'h00;
    
    // sub $5, $3, $2  04622800
    assign rom[12] = 8'h04;
    assign rom[13] = 8'h62;
    assign rom[14] = 8'h28;
    assign rom[15] = 8'h00;
    
    // and $4, $5, $2  44a22000
    assign rom[16] = 8'h44;
    assign rom[17] = 8'ha2;
    assign rom[18] = 8'h20;
    assign rom[19] = 8'h00;
    
    // or $8, $4, $2  4C824000?
    assign rom[20] = 8'h4c;
    assign rom[21] = 8'h82;
    assign rom[22] = 8'h40;
    assign rom[23] = 8'h00;
    
    // sll $8, $8, 1  60084040
    assign rom[24] = 8'h60;
    assign rom[25] = 8'h08;
    assign rom[26] = 8'h40;
    assign rom[27] = 8'h40;
    
    // bne $8, $1, -2  c501fffe
    assign rom[28] = 8'hc5;
    assign rom[29] = 8'h01;
    assign rom[30] = 8'hff;
    assign rom[31] = 8'hfe;  
    
    // slti $6, $2, 4  70460004?
    assign rom[32] = 8'h70;
    assign rom[33] = 8'h46;
    assign rom[34] = 8'h00;
    assign rom[35] = 8'h04;
    
    // slti $7, $6, 0  70C70000?
    assign rom[36] = 8'h70;
    assign rom[37] = 8'hc7;
    assign rom[38] = 8'h00;
    assign rom[39] = 8'h00;
    
    // addiu $7, $7, 8  08e70008
    assign rom[40] = 8'h08;
    assign rom[41] = 8'he7;
    assign rom[42] = 8'h00;
    assign rom[43] = 8'h08;  
    
    // beq $7, $1, -2  c0e1fffe
    assign rom[44] = 8'hc0;
    assign rom[45] = 8'he1;
    assign rom[46] = 8'hff;
    assign rom[47] = 8'hfe;
    
    // sw $2, 4($1)  98220004
    assign rom[48] = 8'h98;
    assign rom[49] = 8'h22;
    assign rom[50] = 8'h00;
    assign rom[51] = 8'h04;  
    
    // lw $9, 4($1)  9c290004
    assign rom[52] = 8'h9c;
    assign rom[53] = 8'h29;
    assign rom[54] = 8'h00;
    assign rom[55] = 8'h04; 
    
    // addiu $10, $0, -2  080afffe
    assign rom[56] = 8'h08;
    assign rom[57] = 8'h0a;
    assign rom[58] = 8'hff;
    assign rom[59] = 8'hfe;
    
    // addiu $10, $10, 1  0?94a0001?
    assign rom[60] = 8'h09;
    assign rom[61] = 8'h4a;
    assign rom[62] = 8'h00;
    assign rom[63] = 8'h01;
    
    //bltz $10, -2 c940fffe
    assign rom[64] = 8'hc9;
    assign rom[65] = 8'h40;
    assign rom[66] = 8'hff;
    assign rom[67] = 8'hfe;
    
    //andi $11,$2,2 ?404b0002?
    assign rom[68] = 8'h40;
    assign rom[69] = 8'h4b;
    assign rom[70] = 8'h00;
    assign rom[71] = 8'h02;
    
    // j 0x00000050  e0000014
    assign rom[72] = 8'he0;
    assign rom[73] = 8'h00;
    assign rom[74] = 8'h00;
    assign rom[75] = 8'h14;
    
    // or $8. $4, 2  ?4c821000?
    assign rom[76] = 8'h4c;
    assign rom[77] = 8'h82;
    assign rom[78] = 8'h10;
    assign rom[79] = 8'h00;
    
    // halt  fc000000
    assign rom[80] = 8'hfc;
    assign rom[81] = 8'h00;
    assign rom[82] = 8'h00;
    assign rom[83] = 8'h00;
    
    wire [31:0] ins;
    assign ins[31:24]=rom[Iaddr[6:2]*4];
    assign ins[23:16]=rom[Iaddr[6:2]*4+1];
    assign ins[15:8]=rom[Iaddr[6:2]*4+2];
    assign ins[7:0]=rom[Iaddr[6:2]*4+3];
*/    