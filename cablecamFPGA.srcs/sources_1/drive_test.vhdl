library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity drive_test is
	port (
		CLK100MHZ : in std_logic;
		dir : in std_logic;
		sw : in std_logic_vector(2 downto 0);
		DRIVE_SIG_OUT : out std_logic
	);
end entity drive_test;

architecture behavioral of drive_test is

component drive_motor is
	port (
		CLK100MHZ : in std_logic;
		SET_SPEED : in std_logic_vector(31 downto 0);
		DRIVE_SIG_OUT : out std_logic
	);
end component drive_motor;

signal SET_SPEED : std_logic_vector(31 downto 0) := X"000249F0";

begin

	drive_motor_1 : drive_motor
		port map 
		(
			CLK100MHZ => CLK100MHZ,
			SET_SPEED => SET_SPEED,
			DRIVE_SIG_OUT => DRIVE_SIG_OUT
		);

process(sw, dir)
begin
	case sw is
		when "000" =>
			SET_SPEED <= X"000249F0";
		when "001" =>
			if dir = '1' then
				SET_SPEED <= X"0002625A";
			else
				SET_SPEED <= X"00023186";
			end if;
		when "010" =>
			if dir = '1' then
				SET_SPEED <= X"00027AC4";
			else
				SET_SPEED <= X"0002191C";
			end if;
		when "011" =>
			if dir = '1' then
				SET_SPEED <= X"0002932E";
			else
				SET_SPEED <= X"000200B2";
			end if;
		when "100" =>
			if dir = '1' then
				SET_SPEED <= X"0002AB98";
			else
				SET_SPEED <= X"0001E848";
			end if;
		when "101" =>
			if dir = '1' then
				SET_SPEED <= X"0002C402";
			else
				SET_SPEED <= X"0001CFDE";
			end if;
		when "110" =>
			if dir = '1' then
				SET_SPEED <= X"0002DC6C";
			else
				SET_SPEED <= X"0001B774";
			end if;
		when others =>
			if dir = '1' then
				SET_SPEED <= X"0002F4D6";
			else
				SET_SPEED <= X"00019F0A";
			end if;		
	end case;
end process;	

end architecture behavioral;