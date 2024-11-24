module register (
    input wire clk,
    input wire rst,
    input wire enr,
    input wire [15:0] in,
    output reg [15:0] out
);

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            out <= 16'b0;
        end else if (enr) begin
            out <= in;
        end
    end

endmodule

