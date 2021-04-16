----------------------------------------------------------------------------------
-- Company: 
-- Engineer: Brian sort of NIk sometimes
-- 
-- Create Date: 04/15/2021 07:59:21 PM
-- Design Name: 
-- Module Name: position - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all; 

entity position is
	port(
	    CLK100MHZ : in std_logic; 
		HALL_EFFECT : in std_logic;
		an : out std_logic_vector(3 downto 0);
		seg : out std_logic_vector(0 to 6);
		RST_L : in std_logic;
		SET_SPEED : in std_logic_vector(31 downto 0);
		POSITION : out std_logic_vector(31 downto 0)
	);
end entity position;

architecture behavioral of position is
	type MOVEMENT is (REVERSE, FORWARD, IDLE);
	signal direction : MOVEMENT := IDLE;
	signal cur_position : signed(31 downto 0) := X"00000000";
	signal count : unsigned(31 downto 0) := X"00000000";
	signal ones : std_logic_vector(6 downto 0):= "0000000";
	signal tens : std_logic_vector(6 downto 0):= "0000000";
	signal huns: std_logic_vector(6 downto 0):= "0000000";
	signal one : integer := 0;
	signal ten : integer := 0;
	signal hun : integer := 0;
	signal hall_high : std_logic := '0';
	
	signal reg1 : std_logic;
	signal reg2 : std_logic;
	
begin

	set_direction : process(SET_SPEED)
	begin
		if (SET_SPEED >= std_logic_vector(to_unsigned(100_000,32)) AND SET_SPEED <= std_logic_vector(to_unsigned(149_000,32))) then
			direction <= REVERSE;
		elsif (SET_SPEED >= std_logic_vector(to_unsigned(151_000,32)) AND SET_SPEED <= std_logic_vector(to_unsigned(200_000,32))) then
			direction <= FORWARD;
		else	
			direction <= IDLE;
		end if;
	end process set_direction;
	
	calculate_position : process(HALL_EFFECT, RST_L, direction, CLK100MHZ)
	begin
		if (RST_L = '0') then
			cur_position <= X"00000000";
		elsif(hall_high = '1' and rising_edge(CLK100MHZ)) then
			if (direction = REVERSE) then
				cur_position <= cur_position - 1;
			elsif(direction = FORWARD) then
				cur_position <= cur_position + 1;
			else
				cur_position <= cur_position + 0;
    		end if;
		end if;
	end process calculate_position;
	
	set_position : process(CLK100MHZ, cur_position)
	begin
	   if rising_edge(CLK100MHZ) then
	       POSITION <= std_logic_vector(cur_position);
	   end if;	
	end process set_position;
    
    display_refresh : process(CLK100MHZ, HALL_EFFECT)
    begin
        if (rising_edge(CLK100MHZ)) then
            reg1 <= HALL_EFFECT;
            reg2 <= reg1;
            
            if (count < 1048576) then
                count <= count + 1;
            else
                count <= X"00000000";
            end if;    
        end if;
    end process display_refresh;
    
    update_count : process(cur_position) 
    begin
        one <= (to_integer(cur_position mod 10));
        ten <= (to_integer((cur_position mod 100)/ 10));
        hun <= (to_integer((cur_position mod 1000)/ 100));

         -- ones display unit
        case one is
            when 0 =>
                ones <= "0000001";
            when 1 =>
                ones <= "1001111";
            when 2 =>
                ones <= "0010010";                     
            when 3 =>
                ones <= "0000110";
            when 4 =>
                ones <= "1001100";
            when 5 =>
                ones <= "0100100";                      
            when 6 =>
                ones <= "0100000";                     
            when 7 =>
                ones <= "0001111";
            when 8 =>
                ones <= "0000000";
            when 9 =>
                ones <= "0001100";
            when others =>
                ones <= "1111111";    
        end case;
        
         -- tens display unit
       case ten is
           when 0 =>
               tens <= "0000001";
           when 1 =>
               tens <= "1001111";
           when 2 =>
               tens <= "0010010";                     
           when 3 =>
               tens <= "0000110";
           when 4 =>
               tens <= "1001100";
           when 5 =>
               tens <= "0100100";                      
           when 6 =>
               tens <= "0100000";                     
           when 7 =>
               tens <= "0001111";
           when 8 =>
               tens <= "0000000";
           when 9 =>
               tens <= "0001100";
           when others =>
               tens <= "1111111";    
       end case;                                   

    
      -- hundreds display unit
      case hun is
          when 0 =>
              huns <= "0000001";
          when 1 =>
              huns <= "1001111";
          when 2 =>
              huns <= "0010010";                     
          when 3 =>
              huns <= "0000110";
          when 4 =>
              huns <= "1001100";
          when 5 =>
              huns <= "0100100";                      
          when 6 =>
              huns <= "0100000";                     
          when 7 =>
              huns <= "0001111";
          when 8 =>
              huns <= "0000000";
          when 9 =>
              huns <= "0001100";
          when others =>
              huns <= "1111111";    
      end case;                                   
    end process update_count;  
    
    update_seg : process(count, cur_position) 
    begin
        if (count < 262144) then
           an <= "1110";
           seg <= ones;
        elsif(count < 524288) then
            an <= "1101";
            seg <= tens;
        elsif(count < 786432) then
            an <= "1011";
            seg <= huns;
        else
            an <= "0111";
            if (cur_position < 0) then
                seg <= "1111110";
            else
                seg <= "1111111";
            end if;    
        end if;    
    end process update_seg;
    
    hall_high <= reg1 and (not reg2);
	
end architecture behavioral;
