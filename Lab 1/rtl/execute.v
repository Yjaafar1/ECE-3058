// vhdl: sudhakar yalamanchili
// translated to verilog by yehowshua with vhd2l and manual tuning
// https://github.com/ldoolitt/vhd2vl

//execution unit. only a subset of instructions are supported in this
//model, specifically add, sub, lw, sw, beq, and, or

module execute(
input wire [31:0] pc4,
// added input wire to provide 26 bits needed for absolute jump - RENAME
input wire [25:0] j_address,
input wire [31:0] register_rs,
input wire [31:0] register_rt,
input wire [5:0] function_opcode,
input wire [31:0] sign_extend,
input wire [4:0] wreg_rd,
input wire [4:0] wreg_rt,
input wire [1:0] aluop,
input wire branch,
input wire alusrc,
input wire regdst,
// input wires to control whether to absolute jump and 
// whether to register write-back to save PC + 4 in $31
input wire jump,
input wire link,
// input wire controlling ori instruction
input wire immediate_or,
// input wire controlling lui instruction
input wire immediate_load_upper,

output reg [31:0] alu_result,
output wire [31:0] branch_addr,
//jump and branch could a shared bus, but it's best to be explicit
output wire [31:0] jump_addr,
output wire [4:0] wreg_address,
output wire do_branch,
output wire do_jump
);

//internals
wire zero;
wire [31:0] ainput;
wire [31:0] binput;
reg [2:0] alu_ctl;

  // compute the two alu inputs
  assign ainput = register_rs;

  // set input b to zero_extend, sign_extend, or register_rt
  assign binput = (immediate_or == 1'b1) ? {{16{1'b0}}, sign_extend[16:0]} :
                  (alusrc == 1'b1)       ? sign_extend : 
                                           register_rt;

  // compute alu_ctl from function_opcode
  // and alu_op 
  always @(*) begin
    // r type instructions
    if ((aluop == 2'b10) & (function_opcode == 6'b100000))
      alu_ctl = 3'b010; // add
    else if ((aluop == 2'b10) & (function_opcode == 6'b100010))
      alu_ctl = 3'b110; // subtract
    else if ((aluop == 2'b10) & (function_opcode == 6'b100100))
      alu_ctl = 3'b000; // and
    else if ((aluop == 2'b10) & (function_opcode == 6'b100101))
      alu_ctl = 3'b001; // or
    else if ((aluop == 2'b10) & (function_opcode == 6'b101010))
      alu_ctl = 3'b111; // slt
    // i type instructions
    else if (immediate_or == 1'b1)
      alu_ctl = 3'b001;

    // for lw, sw, and beq
    else if (aluop == 2'b00 & immediate_load_upper == 1'b0)
      alu_ctl = 3'b010; // add
    else if (aluop == 2'b01)
      alu_ctl = 3'b110; // subtract

  end

  // use alu_ctl to set alu_result
  always @(*) begin
    if (alu_ctl == 3'b010)
      alu_result = ainput + binput;
    else if (alu_ctl == 3'b110)
      alu_result = ainput - binput;
    else if (alu_ctl == 3'b000)
      alu_result = ainput & binput;
    else if (alu_ctl == 3'b001)
      alu_result = ainput | binput;
    else if (alu_ctl == 3'b111)
      alu_result = (ainput < binput) ? 32'b1 : 32'b0;
    else if (link == 1'b1)
      alu_result = pc4;
    else if (immediate_load_upper == 1'b0)
      alu_result = {binput[15:0] ,{16{1'b0}}};
  end


  assign zero = (alu_result == 32'h00000000) ? 1'b1 : 1'b0;

  // branch if beq instruction called
  assign do_branch = (branch & zero);

  //jump if j or jal instruction called
  assign do_jump = jump;

  // set register write-back based on 
  // 1. if JAL (in which case write to $31)
  // 2. if r format (wreg_rd)
  // 3. if not r format or JAL (wreg_rt)
  assign wreg_address = (link == 1'b1)   ? 5'b11111 :
                        (regdst == 1'b1) ? wreg_rd :
                                           wreg_rt;

  assign branch_addr = pc4 + {sign_extend[29:0],2'b00};
  assign jump_addr = {pc4[31:28], j_address, 2'b00};

endmodule
