module insmem (
    input clk,
    input wire [7:0] address,
    output reg [15:0] instruction
);
    reg [15:0] memarr [0:255];
    initial begin
    	$readmemh("/home/askarbtw/cs335_material/bitty/rtl/instructions.hex", memarr);
    end
    always @(posedge clk) begin
        instruction <= memarr[address];
    end
endmodule
