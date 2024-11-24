module alu (
  input wire [15:0] a,
  input wire [15:0] b,
  input wire [2:0] sel,
  output reg [15:0] alu_out
);

  reg [15:0] temp;

always @(*) begin      
    temp = 16'b0;
    
case (sel)
    3'b000: temp = a + b;
    3'b001: temp = a - b;
    3'b010: temp = a & b;
    3'b011: temp = a | b;
    3'b100: temp = a ^ b;
    3'b101: temp = a << (b & 4'hF);
    3'b110: temp = a >> (b & 4'hF);
    3'b111: begin
    if (a == b) temp = 16'b0;
    else if (a > b) temp = 16'b1;
    else if (a < b) temp = 16'b10;
    end
    default: temp = 16'b0;
    endcase
    alu_out = temp;
end
    
  endmodule
