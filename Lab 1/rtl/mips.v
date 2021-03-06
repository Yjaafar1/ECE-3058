module mips(
    input wire clock,
    input wire reset
);

// explicit instantiation of multi-bit wires 
// connecting together different modules.
// single bit wires such as control.alusrc
// are implicitly automatically instantiated

// fetch
wire [31:0] instruction;
wire [31:0] pc4;
// decode
wire [1:0] aluop;
wire [31:0] register_rs;
wire [31:0] register_rt;
wire [31:0] sign_extend;
wire [4:0] wreg_rd;
wire [4:0] wreg_rt;
wire [4:0] wreg_rs;
// execute
wire [31:0] alu_result;
wire [31:0] branch_addr;
wire [31:0] jump_addr;
wire [4:0] wreg_address;
// memory
wire [31:0] read_data;

fetch fetch(
// inputs
.clock(clock),
.reset(reset),
.branch_addr(branch_addr),
.jump_addr(jump_addr),
.do_branch(do_branch),
.do_jump(do_jump),

// outputs
.instruction(instruction),
.pc4(pc4)
);

control control(
// inputs
.opcode(instruction[31:26]),

// outputs
.regdst(regdst),
.memread(memread),
.memtoreg(memtoreg),
.memwrite(memwrite),
.alusrc(alusrc),
.regwrite(regwrite),
.branch(branch),
.aluop(aluop),
.jump(jump),
.link(link),
.immediate_or(immediate_or),
.immediate_load_upper(immediate_load_upper)
);

decode decode(
// inputs
.clock(clock),
.reset(reset),
.instruction(instruction),
.memory_data(read_data),
.alu_result(alu_result),
.regwrite(regwrite),
.memtoreg(memtoreg),
.wreg_address(wreg_address),

// outputs
.register_rs(register_rs),
.register_rt(register_rt),
.sign_extend(sign_extend),
.wreg_rd(wreg_rd),
.wreg_rt(wreg_rt),
.wreg_rs(wreg_rs)   
);

execute execute(
// inputs
.pc4(pc4),
.register_rs(register_rs),
.register_rt(register_rt),
.function_opcode(instruction[5:0]),
.sign_extend(sign_extend),
.wreg_rd(wreg_rd),
.wreg_rt(wreg_rt),
.wreg_rs(wreg_rs),
.aluop(aluop),
.branch(branch),
.alusrc(alusrc),
.regdst(regdst),
.jump(jump),
.link(link),
.immediate_or(immediate_or),
.immediate_load_upper(immediate_load_upper),

// outputs
.alu_result(alu_result),
.branch_addr(branch_addr),
.jump_addr(jump_addr),
.wreg_address(wreg_address),
.do_branch(do_branch),
.do_jump(do_jump)
);

memory memory(
// inputs
.clock(clock),
.address(alu_result),
.write_data(register_rt),
.memwrite(memwrite),

// outputs
.read_data(read_data)
);
endmodule