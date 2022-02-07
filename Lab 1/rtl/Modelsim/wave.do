onerror {resume}
quietly set dataset_list [list vsim sim]
if {[catch {datasetcheck $dataset_list}]} {abort}
quietly WaveActivateNextPane {} 0
add wave -noupdate vsim:/mips_tb/mips/clock
add wave -noupdate vsim:/mips_tb/mips/reset
add wave -noupdate -radix hexadecimal vsim:/mips_tb/mips/instruction
add wave -noupdate -radix decimal vsim:/mips_tb/mips/pc4
add wave -noupdate -radix binary vsim:/mips_tb/mips/aluop
add wave -noupdate -radix hexadecimal vsim:/mips_tb/mips/register_rs
add wave -noupdate -radix hexadecimal vsim:/mips_tb/mips/register_rt
add wave -noupdate -radix decimal vsim:/mips_tb/mips/sign_extend
add wave -noupdate vsim:/mips_tb/mips/wreg_rd
add wave -noupdate vsim:/mips_tb/mips/wreg_rt
add wave -noupdate -radix decimal vsim:/mips_tb/mips/alu_result
add wave -noupdate -radix hexadecimal vsim:/mips_tb/mips/branch_addr
add wave -noupdate vsim:/mips_tb/mips/wreg_address
add wave -noupdate -radix hexadecimal vsim:/mips_tb/mips/read_data
add wave -noupdate vsim:/mips_tb/mips/do_branch
add wave -noupdate vsim:/mips_tb/mips/regdst
add wave -noupdate vsim:/mips_tb/mips/memread
add wave -noupdate vsim:/mips_tb/mips/memtoreg
add wave -noupdate vsim:/mips_tb/mips/memwrite
add wave -noupdate vsim:/mips_tb/mips/alusrc
add wave -noupdate vsim:/mips_tb/mips/regwrite
add wave -noupdate vsim:/mips_tb/mips/branch
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {2 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 263
configure wave -valuecolwidth 340
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ps
update
WaveRestoreZoom {0 ps} {15 ps}
