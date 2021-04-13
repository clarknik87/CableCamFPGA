`timescale 1ns / 1ps
/******************************************
 *	This module divides the system clock	*
 *		values (@50hz) into arbitrarily		*
 *		lower frequencies for a balanced		*
 *		length of time for anode control		*
 *		on a 7-segment display.					*
 ******************************************/

module clkB(
	input sysClk,
	output reg refClk
	);

	reg [23:0] count;
	
	initial begin
		count = 0;
		refClk = 0;
	end

	always @ (posedge sysClk) begin
		//edit the "count up to" value to change anode display rate
		if (count == 24'd100000) begin 
			count <= 0;
			refClk <= ~refClk;			
		end
		
		else begin
			count <= count + 1;
		end
	end
	
	
	
endmodule
