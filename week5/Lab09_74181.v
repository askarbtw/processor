module alu74181(
  input wire carry_in,
  input wire [15:0] in_a,
  input wire [15:0] in_b,
  input wire [3:0] select,
  input wire mode,
  output wire carry_out,
  output wire compare,
  output wire [15:0] alu_out
);

  reg [15:0] logic_out;
  reg [15:0] ar_out;
  reg internal_carry_out;

  always @(*) begin
    if (mode == 1) begin
      case(select)
        4'b0000: logic_out = ~in_a;
        4'b0001: logic_out = ~(in_a | in_b);
        4'b0010: logic_out = ~in_a & in_b;
        4'b0011: logic_out = 16'b0;
        4'b0100: logic_out = ~(in_a & in_b);
        4'b0101: logic_out = ~in_b;
        4'b0110: logic_out = in_a ^ in_b;
        4'b0111: logic_out = ~in_b & in_a;
        4'b1000: logic_out = ~in_a | in_b;
        4'b1001: logic_out = ~(in_a ^ in_b);
        4'b1010: logic_out = in_b;
        4'b1011: logic_out = in_a & in_b;
        4'b1100: logic_out = 16'b1;
        4'b1101: logic_out = in_a | (~in_b);
        4'b1110: logic_out = in_a | in_b;
        4'b1111: logic_out = in_a;
      endcase
    end else begin
      case(select)
        4'b0000: {internal_carry_out, ar_out} = in_a + carry_in;
        4'b0001: {internal_carry_out, ar_out} = in_a + in_b + carry_in;
        4'b0010: {internal_carry_out, ar_out} = in_a + (~in_b) + carry_in;
        4'b0011: ar_out = -1;
        4'b0100: {internal_carry_out, ar_out} = in_a + (in_a & ~in_b) + carry_in;
        4'b0101: {internal_carry_out, ar_out} = in_a + in_b + (in_a & in_b) + carry_in;
        4'b0110: {internal_carry_out, ar_out} = in_a - in_b - 1 + carry_in;
        4'b0111: {internal_carry_out, ar_out} = (in_a & ~in_b) - 1 + carry_in;
        4'b1000: {internal_carry_out, ar_out} = in_a + (in_a & in_b) + carry_in;
        4'b1001: {internal_carry_out, ar_out} = in_a + in_b + carry_in;
        4'b1010: {internal_carry_out, ar_out} = (in_a | ~in_b) + (in_a & in_b) + carry_in;
        4'b1011: {internal_carry_out, ar_out} = (in_a & in_b) - 1 + carry_in;
        4'b1100: {internal_carry_out, ar_out} = in_a + in_a + carry_in;
        4'b1101: {internal_carry_out, ar_out} = (in_a | in_b) + in_a + carry_in;
        4'b1110: {internal_carry_out, ar_out} = (in_a | ~in_b) + in_a + carry_in;
        4'b1111: {internal_carry_out, ar_out} = in_a - 1 + carry_in;
      endcase
    end
  end

  assign alu_out = mode ? logic_out : ar_out;
  assign carry_out = mode ? 1'b0 : internal_carry_out; 
  assign compare = (in_a == in_b);

endmodule
