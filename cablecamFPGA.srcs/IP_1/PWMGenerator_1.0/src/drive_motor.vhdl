--	File Name: drive_motor.vhdl
--	Used to produce PWM signal for ESC to control drive motor speed and direction

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity drive_motor is
	port (
		CLK100MHZ : in std_logic;
		RST_L     : in std_logic;
		SET_SPEED : in std_logic_vector(31 downto 0); -- 100_000 = Full Reverse, 150_000 = Neutral, 200_000 = Full Forward
		DRIVE_SIG_OUT : out std_logic --JB1 {JB[0]}
	);
end entity drive_motor;

architecture behavioral of drive_motor is

constant pwm_max : integer := 1_999_999; -- Max count of 1,999,999 
signal pwm_cnt : std_logic_vector(31 downto 0) := X"00000000"; -- Initial count of 0
signal pulse : std_logic := '1'; 
signal speed : std_logic_vector(31 downto 0) := X"000249F0"; -- Initial speed set to 0/neutral

begin

process(CLK100MHZ) -- Counting
begin
	if(rising_edge(CLK100MHZ)) then	
		if (pwm_cnt < pwm_max) then	
			pwm_cnt <= pwm_cnt + 1;
		else	
			pwm_cnt <= (others=> '0');
		end if;
	end if;
end process;

process(CLK100MHZ) -- Pulsing
begin
	if(rising_edge(CLK100MHZ)) then
		if(pwm_cnt < speed) then
			pulse <= '1';
		else	
			pulse <= '0';
		end if;
	end if;
end process;

process(pulse, RST_L) 
begin
    if RST_L = '0' then
        DRIVE_SIG_OUT <= '0';
    else
	   DRIVE_SIG_OUT <= pulse;
	end if;
end process;		

process(SET_SPEED)
begin
    if SET_SPEED < 100_000 or SET_SPEED > 200_000 then
        speed <= std_logic_vector(to_unsigned(150_000, 32));
    else
	   speed <= SET_SPEED;
	end if;
end process;

end architecture behavioral;	