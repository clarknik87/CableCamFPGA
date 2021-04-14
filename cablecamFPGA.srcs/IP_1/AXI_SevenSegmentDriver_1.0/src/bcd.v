`timescale 1ns / 1ps

/***************************************************************
 *	This module takes in an 8-bit binary value and converts		*
 * 	it to three 4-bit Binary Coded Decimal Values. The			*
 *		algorithm used is a variation on the Dibble-Dabble.		*
 ***************************************************************/
module bcd(
		//*NOTE: If this input is set to "sw" then it will be
		//		recieveing input from the 8 onborard toggle switches
		//		as an 8-bit input.
		input [15:0] sw, 				//8-bit binary in
		output reg [3:0] thousands, //4-bit hundereds, 1st column
		output reg [3:0] hundreds, //4-bit hundereds, 1st column
		output reg [3:0] tens, 		//4-bit tens, 2nd column
		output reg [3:0] ones		//4-bit ones, 3rd column
    );
	 
	 integer index; // to decrement with for loop
	 
	 always @(sw) //only change when binary element changes
	 begin
		//set 100's, 10's, and 1's to 0
		thousands = 4'd0; 	//4-bit decimal = 0
		hundreds = 4'd0; 	//4-bit decimal = 0
		tens = 4'd0; 		//4-bit decimal = 0
		ones = 4'd0; 		//4-bit decimal = 0
		
		for (index = 15; index >= 0; index = index - 1) //decrement to start with highest place value
		begin
		
			//add 3 to each column if the value is >= 5
			if (thousands >= 5)
                thousands =  thousands + 3;

			if (hundreds >= 5)
				hundreds =  hundreds + 3;
				
			if (tens >= 5)
				tens =  tens + 3;
				
			if (ones >= 5)
				ones =  ones + 3;
				
			//shift to the left 1 ALWAYS
			//shift the biggest values first so they are out of the way
			thousands = thousands << 1;	//shift hundreds 1 to left
            thousands[0] = hundreds[3];         //highest value in the tens column is assigned to the lowest place value in the hundreds column

			hundreds = hundreds << 1;	//shift hundreds 1 to left
			hundreds[0] = tens[3]; 		//highest value in the tens column is assigned to the lowest place value in the hundreds column
			
			tens = tens << 1; 	//shift tens 1 to left
			tens[0] = ones[3]; 	//lowest of tens is assigned the highest of the ones
			
			ones = ones << 1;		//shift ones 1 to left
			ones[0] = sw[index]; //lowest of ones is assigned next value in binary
		end //for
	end //always
			
			
			
endmodule //bcd
