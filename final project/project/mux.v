module mux (
    input [3:0] sel,
    input [15:0] mux0,
    input [15:0] mux1,
    input [15:0] mux2,
    input [15:0] mux3,
    input [15:0] mux4,
    input [15:0] mux5,
    input [15:0] mux6,
    input [15:0] mux7,
    input [15:0] immediate,
    output reg [15:0] out
);

  always @(*) begin
    case (sel)
        4'b0000: out = mux0;
        4'b0001: out = mux1;
        4'b0010: out = mux2;
        4'b0011: out = mux3;
        4'b0100: out = mux4;
        4'b0101: out = mux5;
        4'b0110: out = mux6;
        4'b0111: out = mux7;
        4'b1000: out = immediate;
        default: out = 16'hFFFF;
    endcase
  end
endmodule
