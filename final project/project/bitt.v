module bitt (
  input wire clk,        
  input wire reset,      
  input wire run,        
  input wire [15:0] instruction, 
  output done,
  output wire [15:0] mo0, mo1, mo2, mo3, mo4, mo5, mo6, mo7
);
  	wire [15:0] immediate;
	wire [15:0] moi, mout, mos, moc, alu_out;
   	wire [2:0] aluSelect; 
   	wire [3:0] muxSelect;
  	wire ce0, ce1, ce2, ce3, ce4, ce5, ce6, ce7, cei, ces, cec;

/*dpi_monitor monitor (
        .done(done),
        .instruction(instruction),
        .hw_result(moc)
    );*/
   
control_unit control_unit (
    .clk(clk),
    .rst(reset),
    .run(run),
    .immediate(immediate),
    .done(done),
    
    .instruction(instruction),
    .muxSelect(muxSelect),
    .aluSelect(aluSelect),
    .en0(ce0),
    .en1(ce1),
    .en2(ce2),
    .en3(ce3),
    .en4(ce4),
    .en5(ce5),
    .en6(ce6),
    .en7(ce7),   
  
    .eni(cei),
    .ens(ces),
    .enc(cec)
);

  register reg0 (.clk(clk), .rst(reset), .enr(ce0), .in(moc), .out(mo0));
  register reg1 (.clk(clk), .rst(reset), .enr(ce1), .in(moc), .out(mo1));
  register reg2 (.clk(clk), .rst(reset), .enr(ce2), .in(moc), .out(mo2));
  register reg3 (.clk(clk), .rst(reset), .enr(ce3), .in(moc), .out(mo3));
  register reg4 (.clk(clk), .rst(reset), .enr(ce4), .in(moc), .out(mo4));
  register reg5 (.clk(clk), .rst(reset), .enr(ce5), .in(moc), .out(mo5));
  register reg6 (.clk(clk), .rst(reset), .enr(ce6), .in(moc), .out(mo6));
  register reg7 (.clk(clk), .rst(reset), .enr(ce7), .in(moc), .out(mo7));
  
  register regS (.clk(clk), .rst(reset), .enr(ces), .in(mout), .out(mos));
  
  register regC (.clk(clk), .rst(reset), .enr(cec), .in(alu_out), .out(moc));
  
  register regi (.clk(clk), .rst(reset), .enr(cei), .in(instruction), .out(moi));

  mux muxm (
    .sel(muxSelect),
    .mux0(mo0),
    .mux1(mo1),
    .mux2(mo2),
    .mux3(mo3),
    .mux4(mo4),
    .mux5(mo5),
    .mux6(mo6),
    .mux7(mo7),
    .immediate(immediate),
    .out(mout)
);

alu alu1 (
    .a(mos),      
    .b(mout),       
    .sel(aluSelect),     
    .alu_out(alu_out)
);

endmodule
