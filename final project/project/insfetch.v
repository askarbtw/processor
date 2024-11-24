module insfetch (
    input wire clk,            // Clock signal
    input wire reset,          // Reset signal
    input wire run,            // Signal to start execution
    output wire done,          // Indicates instruction execution completion
    output wire [15:0] instruction   // Instruction fetched by the fetch module
);
    wire [7:0] pc;	

    insmem insmem (
    	.clk(clk),
	.address(pc),
	.instruction(instruction)
    );

    programcounter pcmod (
	.in(pc),
        .clk(clk),
        .reset(reset),
        .enable(done),
        .out(pc)
    );
		
    bitt bitty_core (
        .clk(clk),
        .reset(reset),
        .run(run),             // Signal to run the processor
        .instruction(instruction), // Instruction provided by the fetch module
        .done(done)            // Completion signal from the processor
    );
endmodule
