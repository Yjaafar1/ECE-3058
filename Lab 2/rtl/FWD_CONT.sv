//***********************************************************
// ECE 3058 Architecture Concurrency and Energy in Computation
//
// MIPS Processor System Verilog Behavioral Model
//
// School of Electrical & Computer Engineering
// Georgia Institute of Technology
// Atlanta, GA 30332
//
//  Engineer:   Brothers, Tim
//  Module:     SFORWARDING UNIT
//  Functionality:
//      implements the Forwardin unit for the MIPS pipelined processor
//
//  Inputs:
//      ip_opcode: The opcode from the fetch stage
//    
//  Outputs:

//    
//  Version History:
//***********************************************************

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Module Declaration
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

module FWD_CONT(
input logic ip_EX_MEM_RegWrite,  //EX/MEM regwrite
input logic ip_MEM_WB_RegWrite,  //MEM/WB regwrite
input logic [4:0] ip_EX_MEM_dest,//EX/MEM Dest Register
input logic [4:0] ip_MEM_WB_dest,//MEM/WB Dest Register
input logic [4:0] ip_DEC_DEST_RS, //Rs from decode stage
input logic [4:0] ip_DEC_DEST_RT, //Rt from decode stage
output logic  [1:0] op_FA, //select lines for forwarding muxes (Rs)
output logic  [1:0] op_FB  //select lines for forwarding muxes (Rt)
);

logic dest_match_EX_MEM_A;
assign dest_match_EX_MEM_A = ip_EX_MEM_dest == ip_DEC_DEST_RS;
logic dest_match_MEM_WB_A;
assign dest_match_MEM_WB_A = ip_MEM_WB_dest == ip_DEC_DEST_RS;

always @(*) begin
    if (ip_EX_MEM_RegWrite && ip_DEC_DEST_RS && dest_match_EX_MEM_A)
        op_FA = 2'b01;
    else if (ip_MEM_WB_RegWrite && ip_DEC_DEST_RS && dest_match_MEM_WB_A)
        op_FA = 2'b10;
    else 
        op_FA = 2'b00;
end

logic dest_match_EX_MEM_B;
assign dest_match_EX_MEM_B = ip_EX_MEM_dest == ip_DEC_DEST_RT;
logic dest_match_MEM_WB_B;
assign dest_match_MEM_WB_B = ip_MEM_WB_dest == ip_DEC_DEST_RT;

always @(*) begin
    if (ip_EX_MEM_RegWrite && ip_DEC_DEST_RT && dest_match_EX_MEM_A) 
        op_FB = 2'b01;
    else if (ip_MEM_WB_RegWrite && ip_DEC_DEST_RT && dest_match_MEM_WB_A) 
        op_FB = 2'b10;
    else 
        op_FB = 2'b00;
end

endmodule
