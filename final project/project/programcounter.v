module programcounter (
    input wire [7:0] in,
    input wire clk,
    input wire reset,
    input wire enable,
    output reg [7:0] out
);
    always @(posedge clk or posedge reset) begin
        if (reset)
            out <= 8'd0;  // Reset to the start address
        else if (enable)
            out <= out + 1; // Increment to the next instruction
    end
endmodule
