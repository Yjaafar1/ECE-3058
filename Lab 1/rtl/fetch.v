// vhdl: sudhakar yalamanchili
// translated to verilog by yehowshua with vhd2l and manual tuning
// https://github.com/ldoolitt/vhd2vl

// instruction fetch behavioral model. instruction memory is
// provided within this model. if increments the pc,  
// and writes the appropriate output signals. 


module fetch(
input wire clock,
input wire reset,
input wire [31:0] branch_addr,
input wire [31:0] jump_addr,
input wire do_branch,
input wire do_jump,

output wire [31:0] instruction,
output wire [31:0] pc4
);

// internals
reg [31:0] pc;
wire [31:0] next_pc;


  // load the program into the memory
  // a better way to do this is with gnu-as
  // and verilog primitive ``readmemh``.
  // note we use [0:31]. This allows us to 
  // paste instructions from big-endian
  // assemblers lik gnu-as in big-endian
  // mode and most online mips assemblers

  reg [0:31] iram [0:2];
    initial begin 
      iram[0] = 32'h022182A; // slt $1, $2, $3
      iram[1] = 32'h023102A; // slt $1, $3, $2
      iram[2] = 32'h0000000; // no op

      //ori test
      // iram[0] = 32'b00110100000000010000000000000011; //ori $1, $0, 3
      // iram[1] = 32'b00000000000000010001000000100000; //add $2, $0, $1
      // iram[2] = 32'h0000000; // no op

      //lui test
      // iram[0] = 32'b00111100000000010000000000001111; //lui $1, xF
      // iram[1] = 32'b00000000000000010001000000100000; //add $2, $0, $1
      // iram[2] = 32'h0000000; // no op

      //j test
      // iram[0] = 32'b00110100000000010000000000000011; //ori $1, $0, 3
      // iram[1] = 32'b00000000000000010001000000100000; //add $2, $0, $1
      // iram[2] = 32'b00001000000000000000000000000001; //j 4

      //jal test
      // iram[0] = 32'b00110100000000010000000000000011; //ori $1, $0, 3
      // iram[1] = 32'b00000000000111110001000000100000; //add $2, $0, $31
      // iram[2] = 32'b00001100000000000000000000000001; //jal 4
    end 
  
  // access instruction pointed to by current pc
  assign instruction = iram[pc[4:2]];

  // compute value of next pc
  assign next_pc = do_jump == 1'b1 ? jump_addr : do_branch == 1'b1 ? branch_addr : pc + 4;

  // update the pc on the next clock			   
  always @(posedge clock) begin
    if(reset == 1'b1)
      pc <= 32'h00000000;
    else
      pc <= next_pc;
  end

  assign pc4 = pc + 4;

endmodule