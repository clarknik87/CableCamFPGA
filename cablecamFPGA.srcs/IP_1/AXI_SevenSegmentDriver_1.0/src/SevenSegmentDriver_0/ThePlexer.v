`timescale 1ns / 1ps


/******************************************
*	This Multiplexer switches the 			*
*		4 place values across the four		*
*		anodes for display.  Note that		*
*		the right most place value will		*
*		always be an "F" to represent			*
*		degrees Farenheit.						*
*******************************************/

module ThePlexer(
	input [6:0] disp1,
	input [6:0] disp10,
	input [6:0] disp100,
	input [6:0] disp1000,
	input [1:0] dec,
	input clk,
	output reg [6:0] disp,
	output reg [3:0] anode,
	output reg dp
	);
	
	/*
	// Clk for Simulation. In REAL WORLD, use divided 
	//		clock value the main module.
	wire clk;
	clkA clkInst (
		.clk(clk)
	 );
	*/
	
	// rotating select, 2-bits
	reg [1:0] sel;
	
	// initialize select value
	initial begin
		sel <= 0;
	end
	
	// rotate select as the divided clock value changes
	always @ (posedge clk)
	begin
		sel <= sel + 1;
	end
	
	// Plexage
	//		This assigns the place value data and the anode control
	//		to its correct display digit.
	always @(*) begin //* run when any input changes
		case (sel)
			2'b00 : begin //thousands
                // This if statement makes the value 0 not display
                if (disp1000 == 7'b1111110) begin
                        //anode = 4'b1111; //1111
                        disp = 7'b0000000;
                end
                
                else    begin
                    disp = disp1000;
                end
                anode = 4'b0111; //0111                
                if (dec==2'b11)
                    dp = 0;
                else
                    dp = 1;
            end
            
			2'b01 : begin //hundreds
					// This if statement makes the value 0 not display
					if ((disp100== 7'b1111110) & (disp1000 == 7'b1111110)) begin
			     		//anode = 4'b1111; //1111
			     		disp = 7'b0000000;
					end
					
					else	begin
						disp = disp100;
					end
					anode = 4'b1011; //0111
                    if (dec==2'b10)
                        dp = 0;
                    else
                        dp = 1;					
				end
				
			2'b10 : begin //tens
					// This if statement makes the value 0 not display
					//		ONLY if hundreds place is also 0
					if ((disp10== 7'b1111110) & (disp100== 7'b1111110) & (disp1000== 7'b1111110)) begin
						//anode = 4'b1111; //1111
						disp = 7'b0000000;
					end
					
					else	begin
						disp = disp10;
					end
					anode = 4'b1101; //1011
                    if (dec==2'b01)
                        dp = 0;
                    else
                        dp = 1;                    
						
				end	

				
			2'b11 : begin  //ones
				// will always display 0 if it is 0
				disp = disp1;
				anode = 4'b1110; //1101
				end
/*
			2'b11 : begin
				// Fahrenheit symbol
				disp = 7'b1000111; //F
				anode = 4'b1110; //1110
				end
	*/			
				// If the sh** hits the fan with the
				//		rotating select, display an "E"
				// 	for "Error" in the hundreds place
			default : begin
				disp = 7'b1001111;
				anode = 4'b0111; //0111
				end
		endcase
	end
endmodule
