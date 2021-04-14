`timescale 1ns / 1ps
/***************************************************************
 *	This module takes in a 4-bit Binary Coded Decimal value	 	*
 *		and assigns it a 7-bit value for display on a 7-segment	*
 *		display.																	*
 ***************************************************************/
module SevenSegmentTruthTable(
		input [3:0] digit_in, 
		output reg [6:0] display_digit
    );
	
	always @(*) begin //* run when any input changes
			case (digit_in)
			4'b0000 : display_digit = 7'b1111110; //0
			4'b0001 : display_digit = 7'b0110000; //1
			4'b0010 : display_digit = 7'b1101101; //2
			4'b0011 : display_digit = 7'b1111001; //3
			4'b0100 : display_digit = 7'b0110011; //4
			4'b0101 : display_digit = 7'b1011011; //5
			4'b0110 : display_digit = 7'b1011111; //6
			4'b0111 : display_digit = 7'b1110000; //7
			4'b1000 : display_digit = 7'b1111111; //8
			4'b1001 : display_digit = 7'b1111011; //9
			4'b1010 : display_digit = 7'b1110111; //a
			4'b1011 : display_digit = 7'b0011111; //b
			4'b1100 : display_digit = 7'b0001101; //c
			4'b1101 : display_digit = 7'b0111101; //d
			4'b1110 : display_digit = 7'b1001111; //e
			4'b1111 : display_digit = 7'b1000111; //f
			// default case to catch unexpected or invalid inputs :
			default : display_digit = 7'b0000000;
		endcase
	end

endmodule
