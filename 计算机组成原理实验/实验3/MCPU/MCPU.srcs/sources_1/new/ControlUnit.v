`timescale 1ns / 1ps

module ControlUnit(
    input CLK,
    input RST,
    input zero,
    input sign,
    input [5:0] Opcode,
    output reg PCWre,
    output reg ALUSrcA,
    output reg ALUSrcB,
    output reg DBDataSrc,
    output reg RegWre,
    output reg WrRegDSrc,
    output reg InsMemRW,
    output reg mRD,
    output reg mWR,
    output reg IRWre,
    output reg [1:0] RegDst,
    output reg ExtSel,
    output reg [1:0] PCSrc,
    output reg [2:0] ALUOp,
    output reg [2:0] curState,
    output reg [2:0] nextState
    );
    
    parameter [2:0]
    IF=3'b000,
    ID=3'b001,
    EXE1=3'b110,
    EXE2=3'b101,
    EXE3=3'b010,
    MEM=3'b011,
    WB1=3'b111,
    WB2=3'b100;
    
    parameter [5:0]
    add=6'b000000,
    sub=6'b000001,
    addiu=6'b000010,
    _and=6'b010000,
    andi=6'b010001,
    ori=6'b010010,
    xori=6'b010011,
    sll=6'b011000,
    slti=6'b100110,
    slt=6'b100111,
    sw=6'b110000,
    lw=6'b110001,
    beq=6'b110100,
    bne=6'b110101,
    bltz=6'b110110,
    j=6'b111000,
    jr=6'b111001,
    jal=6'b111010,
    halt=6'b111111;
      
    initial begin
        nextState<=ID;
        //IRWre=1;
    end
    always @(posedge CLK) begin//状态转换
        if (RST==1'b0) curState<=IF;
        else curState=nextState;
    end
    
    always@(*) begin//获取新状态
        case (curState)
            IF:nextState<=ID;
            ID:begin
                case(Opcode)
                    beq,bne,bltz:nextState<=EXE2;
                    sw,lw:nextState<=EXE3;
                    j,jal,jr,halt:nextState<=IF;
                    default:nextState<=EXE1;
                endcase
            end
            EXE1:nextState<=WB1;
            EXE2:nextState<=IF;
            EXE3:nextState<=MEM;
            MEM:nextState<=(Opcode==lw)?WB2:IF;
            WB1:nextState<=IF;
            WB2:nextState<=IF;
        endcase
    end

    always@(*) begin//控制信号部分
        case(curState)
            IF:begin
                //PCWre=(Opcode==halt)?0:1;
                PCWre=0;
                IRWre=1;
                InsMemRW=1;
                mRD=0;
                mWR=0;
                RegWre=0;
            end
            ID:begin
                //PCWre=0;
                PCWre=(Opcode==jal || Opcode==j || Opcode==jr)?1:0;
                IRWre=0;
                WrRegDSrc=(Opcode==jal)?0:1;
                ExtSel=(Opcode == andi || Opcode == ori || Opcode == xori)?0:1;
                RegWre=(Opcode==jal)?1:0;
                RegDst[0]=(Opcode == addiu || Opcode == andi || Opcode == ori || Opcode == xori || Opcode == slti || Opcode == lw)?1:0;
                RegDst[1]=(Opcode == add || Opcode == sub || Opcode == _and || Opcode == sll || Opcode == slt)?1:0;
                if (Opcode==j || Opcode==jal) PCSrc=2'b11;
                else if (Opcode==jr) PCSrc=2'b10;
                else PCSrc=2'b00;
            end
            EXE1:begin //add sub……
                //IRWre=0;
                ALUSrcA=(Opcode==sll)?1:0;
                ALUSrcB=(Opcode == addiu || Opcode == andi || Opcode == ori || Opcode == xori || Opcode == slti)?1:0;
                ALUOp[0]=(Opcode == sub || Opcode == ori || Opcode == xori)?1:0;
                ALUOp[1]=(Opcode == ori || Opcode == xori || Opcode == sll || Opcode == slti || Opcode == slt)?1:0;
                ALUOp[2]=(Opcode == _and || Opcode == andi || Opcode == xori || Opcode == slti || Opcode == slt)?1:0;
                DBDataSrc=0;
            end
            EXE2:begin//beq bne bltz
                PCWre=1;//
                //IRWre=0;
                ALUSrcA=0;
                ALUSrcB=0;
                ALUOp=(Opcode==bltz)?3'b110:3'b001;
                PCSrc[0]=((Opcode == beq && zero == 1) || (Opcode == bne && zero == 0) || (Opcode == bltz && zero==0))?1:0;
                PCSrc[1]=0;
            end
            EXE3:begin//sw lw
                //IRWre=0;
                ALUSrcA=0;
                ALUSrcB=1;
                ALUOp=3'b000;
                DBDataSrc=1;
            end
            MEM:begin//sw lw
                PCWre=(Opcode==sw)?1:0;
                mRD=(Opcode == lw)?1:0;
                mWR=(Opcode == sw)?1:0;       
            end
            WB1:begin//add sub……
                PCWre=1;
                mRD=0;
                mWR=0;
                RegWre=1;
            end
            WB2:begin//lw
                PCWre=1;
                mRD=0;
                mWR=0;
                RegWre=1;
            end
        endcase
    end
endmodule
