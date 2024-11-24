import "DPI-C" function void verify_instruction(input shortint instruction, input shortint hw_result);

module dpi_monitor (
    input logic done,
    input logic [15:0] instruction,
    input logic [15:0] hw_result
);

    always @(posedge done) begin
        verify_instruction(instruction, hw_result);
    end

endmodule
