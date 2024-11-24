module control_unit(
input clk,
input rst,
input run,
input [15:0] instruction,
output reg [2:0] aluSelect,
output reg [3:0] muxSelect,
output reg en0, en1, en2, en3, en4, en5, en6, en7, eni, ens, enc,
output reg done,
output reg [15:0] immediate
);
    reg immediate_ins;
    parameter STATE_0 = 2'b00;
    parameter STATE_1 = 2'b01;
    parameter STATE_2 = 2'b10;
    parameter STATE_3 = 2'b11;
    reg [1:0] current_state, next_state;
    
    reg [3:0] muxSel;
   
wire [4:0] unused_signal_1 = instruction[9:5]; // Unused bits
// wire [1:0] unused_signal_2 = instruction[1:0]; // Unused bits

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            current_state <= STATE_0;
        end else begin
            current_state <= next_state;
        end
    end
    
    
    always @(*) begin
	case (current_state)
	
	STATE_0: begin
	next_state = STATE_1;
	end
	STATE_1: begin
	next_state = STATE_2;
	end
	STATE_2: begin
	next_state = STATE_3;
	end
	STATE_3: begin
	next_state = STATE_0;
	end
	default: begin
	next_state = STATE_0;
	end
	endcase
	end
	
	
	
    always @(*) begin
	done = 1'b0;                 
        en0 = 1'b0;
        en1 = 1'b0;
        en2 = 1'b0;
        en3 = 1'b0;
        en4 = 1'b0;
        en5 = 1'b0;
        en6 = 1'b0;
        en7 = 1'b0;
        ens = 1'b0;
        enc = 1'b0;
        eni = 1'b0;
        done = 1'b0;
        immediate = 16'b0;
        muxSel = 4'b000;
        aluSelect = 3'b000;
        muxSelect = 4'b0000;
	immediate_ins = (instruction[1:0] == 2'b01);
        case (current_state)
            STATE_0: begin
                if (run) begin
                    eni = 1'b1;
                    ens = 1'b1;
                    muxSelect =  {1'b0, instruction [15:13]};
                end
            end
            STATE_1: begin
                enc = 1'b1;
                aluSelect = instruction [4:2];
        	if (immediate_ins) begin
        		immediate[15:0] = {{8{1'b0}}, instruction[12:5]};
        		muxSelect = 4'b1000;
        	end else begin
                	muxSelect = {1'b0, instruction [12:10]};
                end
            end
            STATE_2: begin
            muxSel = {1'b0, instruction [15:13]};
          case (muxSel)
            4'b0000: begin
            en0 = 1'b1;
            end
            4'b0001: begin
            en1 = 1'b1;
            end
            4'b0010: begin
            en2 = 1'b1;
            end
            4'b0011: begin
            en3 = 1'b1;
            end
            4'b0100: begin
            en4 = 1'b1;
            end
            4'b0101: begin
            en5 = 1'b1;
            end
            4'b0110: begin
            en6 = 1'b1;
            end
            4'b0111: begin
            en7 = 1'b1;
            end
            endcase
          end
          STATE_3: begin

          done = 1'b1;
          
            end
        endcase
    end
endmodule

