// vhdl: sudhakar yalamanchili
// translated to verilog by yehowshua with vhd2l and manual tuning
// https://github.com/ldoolitt/vhd2vl

// control unit. simply implements the truth table for a small set of
// instructions 

module control(
input wire [5:0] opcode,

output wire regdst,
output wire memread,
output wire memtoreg,
output wire memwrite,
output wire alusrc,
output wire regwrite,
output wire branch,
output wire [1:0] aluop,
// new outputs for bonus instructions
output wire jump,
output wire link,
output wire immediate_or,
output wire immediate_load_upper
);

//internals
//register instructions
wire rformat;
wire beq;

//immediate instructions
wire lw;
wire sw;
// additional 
wire ori;
wire lui;

//jump instructions
wire j;
wire jal;

  // recognize opcode for each instruction type
  //r format
  assign rformat = opcode == 6'b000000 ? 1'b1 : 1'b0;
  

  //i format
  assign lw = opcode ==   6'b100011 ? 1'b1 : 1'b0;
  assign sw = opcode ==   6'b101011 ? 1'b1 : 1'b0;
  assign beq = opcode ==  6'b000100 ? 1'b1 : 1'b0;
  assign ori = opcode ==  6'b001101 ? 1'b1 : 1'b0;
  assign lui = opcode ==  6'b001111 ? 1'b1 : 1'b0;

  //j format
  assign j = opcode == 6'b000010 ? 1'b1 : 1'b0;
  assign jal = opcode == 6'b000011 ? 1'b1 : 1'b0;


  // implement each output signal as the column of the truth
  // table which defines the control
  // execute control signals
  assign alusrc = lw | sw | ori | lui;
  assign regdst = rformat;
  assign branch = beq;
  // command signal to execute absolute jump
  assign jump =  j | jal;
  assign link = jal;
  assign aluop[1:0] = {rformat,beq};

  // memory control signals
  assign memwrite = sw;
  
  //decode control signals
  assign memtoreg = lw;
  assign regwrite = rformat | lw | jal | ori | lui;
  
  // memread never used?
  //assign memread = lw;

  
  
  

endmodule