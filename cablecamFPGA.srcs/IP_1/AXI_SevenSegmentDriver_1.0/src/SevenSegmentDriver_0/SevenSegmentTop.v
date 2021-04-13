`timescale 1ns / 1ps

/***************************************************************
 *	THIS IS THE MAIN MODULE where various other modules work		*
 *		together to bring in an 8-bit binary value, convert it	*
 *		to Binary Coded Decimal, and display the initial 8-bit	*
 *    input as a decimal number from 0 - 255 on the Basys2		*
 *		7-segment display with a "F" in the right most position 	*
 *		to represent Fahrenheit.											*
 *																					*
 *	NOTE:  With minor modifications (currently untested), the	*
 *		8-bit binary value can also be represented as a two 		*
 *		digit hexadecimal number.											*
 ***************************************************************/
 
module FullDisplay(
		input clk,
		input [15:0] din,
		input        bcd,
		input  [1:0] dec,
		output [3:0] an, 
		output [6:0] seg,
		output dp
    );
	
	//used to bring .display_digit in and to seg
	wire [6:0] ones_place;
	wire [6:0] tens_place;
	wire [6:0] hundreds_place;
	wire [6:0] thousands_place;
		
	//used to bring bcd in and to .digit in for conversion
	wire [3:0]ones;
	wire [3:0]tens;
	wire [3:0]hundreds;
	wire [3:0]thousands;

	reg [3:0]ones_sel;
	reg [3:0]tens_sel;
	reg [3:0]hundreds_sel;
	reg [3:0]thousands_sel;

	
	wire refclk;	
	//display is active low, inversion needed
	wire [7:0] segNot;
	assign seg[0] = ~segNot[6];
	assign seg[1] = ~segNot[5];
	assign seg[2] = ~segNot[4];
	assign seg[3] = ~segNot[3];
	assign seg[4] = ~segNot[2];
	assign seg[5] = ~segNot[1];
	assign seg[6] = ~segNot[0];
	
	
	// Display Mux
		ThePlexer Mux(
		.disp1(ones_place),
		.disp10(tens_place),
		.disp100(hundreds_place),
		.disp1000(thousands_place),
		.clk(refclk),
		.disp(segNot), //segNot
		.anode(an),
		.dec(dec),
		.dp(dp)
		);
	
	
	// clkB, the system clk divided
		clkB clkInst (
			.sysClk(clk),
			.refClk(refclk)
		);
		
	
	/******************************************************************
	 *		Three instances of in->out truth table for 7-seg display		*
	 * 	--------------------------------------------------------		*
	 * 		(one instance per digit)											*
	 *			## uncomment the module you want to use						*
	 ******************************************************************/
	SevenSegmentTruthTable digit1s( 
		//ONES PLACE
	
			//BCD
			.digit_in(ones_sel),				//in  4-bit
			.display_digit(ones_place)	//out	7-bit

	
			/*
			//HEX
			.digit_in(NumberIn[3:0]),
			.display_digit(1s_place[6:0])
			*/
		);
	 
	SevenSegmentTruthTable digit10s(
		//TENS PLACE
		
			//BCD
			.digit_in(tens_sel),				//in	4-bit
			.display_digit(tens_place)	//out	7-bit
	 
			/*
			//HEX
			.digit_in(NumberIn[7:4]),
			.display_digit(10s_place[6:0])
			*/
		);
	 
	SevenSegmentTruthTable digit100s(
		//HUNDREDS PLACE

			//BCD
			.digit_in(hundreds_sel),			//in	4-bit
			.display_digit(hundreds_place) //out	7-bit
			
			/*
			//HEX
			.digit_in(NumberIn[11:8]),
			.display_digit(100s_place[6:0])
			*/
		);

	SevenSegmentTruthTable digit1000s(
		//HUNDREDS PLACE

			//BCD
			.digit_in(thousands_sel),			//in	4-bit
			.display_digit(thousands_place) //out	7-bit
			
			/*
			//HEX
			.digit_in(NumberIn[11:8]),
			.display_digit(100s_place[6:0])
			*/
		);
	
	
	
	
	/******************************
	 *		INTERFACE INSTANCES		*
	 *		-------------------		*
	 *			(1) HEX					*
	 *			(2) BCD					*
	 *										*
	 *  ## uncomment the one		*
	 *			you want to use		*
	 ******************************/
	
	/*
	//NOTE: may need other modifications to work
	// (1) HEX interface	
	SwitchIn SI(.NumberIn(sw1))	//out
	*/
	
	// (2) BCD interface
	bcd bcd_in(
		.sw(din),			    //in	16-bit
		.ones(ones),			//out	4-bit
		.tens(tens),			//out	4-bit
		.hundreds(hundreds)	,    //out	4-bit
		.thousands(thousands)	//out	4-bit
    );

    always @* begin
        if (bcd) begin
            ones_sel <= ones;
            tens_sel <= tens;
            hundreds_sel <= hundreds;
            thousands_sel <= thousands;
        end
        else begin
            ones_sel <= din[3:0];
            tens_sel <= din[7:4];
            hundreds_sel <= din[11:8];
            thousands_sel <= din[15:12];
        end
    end
endmodule
