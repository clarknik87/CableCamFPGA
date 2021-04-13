----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 01/08/2021 06:15:31 PM
-- Design Name: 
-- Module Name: top_level - Behavioral
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


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity top_level is
    Port (
        clk             :   in  std_logic;
        rst_l           :   in  std_logic;
        sw              :   in  std_logic_vector(15 downto 0);
        led             :   out std_logic_vector(15 downto 0);
        usb_uart_rxd    :   in  std_logic;
        usb_uart_txd    :   out std_logic;
        stormUART_rxd   :   in  std_logic;
        stormUART_txd   :   out std_logic;
        PWMChannel      :   in  std_logic_vector(5 downto 0);
        driveMotor      :   out std_logic;
        seg             :   out std_logic_vector(6 downto 0);
        an              :   out std_logic_vector(3 downto 0);
        dp              :   out std_logic
    );
end top_level;

architecture Behavioral of top_level is

component mb_subsystem_wrapper is
  port (
    PWMin : in STD_LOGIC_VECTOR ( 5 downto 0 );
    digitan : out STD_LOGIC_VECTOR ( 3 downto 0 );
    digitdp : out STD_LOGIC;
    digitseg : out STD_LOGIC_VECTOR ( 6 downto 0 );
    drive_motor_pwm : out STD_LOGIC;
    led : out STD_LOGIC_VECTOR ( 15 downto 0 );
    stormUART_rxd : in STD_LOGIC;
    stormUART_txd : out STD_LOGIC;
    sw : in STD_LOGIC_VECTOR ( 15 downto 0 );
    sys_clk : in STD_LOGIC;
    sys_reset : in STD_LOGIC;
    usb_uart_rxd : in STD_LOGIC;
    usb_uart_txd : out STD_LOGIC
  );
end component;

--component pwm_generate is
--    port(
--        clk     : in std_logic;
--        rst_l   : in std_logic;
--        pwm_out : out std_logic
--    );
--end component;

--signal pwm_out  : std_logic;
--signal pwm_in   : std_logic_vector(5 downto 0);

begin

microblaze_system : mb_subsystem_wrapper
port map(
    PWMin           => PWMChannel,
    drive_motor_pwm => driveMotor,
    led             => led,
    sw              => sw,
    sys_clk         => clk,
    sys_reset       => rst_l,
    usb_uart_rxd    => usb_uart_rxd,
    usb_uart_txd    => usb_uart_txd,
    stormUART_rxd   => stormUART_rxd,
    stormUART_txd   => stormUART_txd
);

--pwm_create : pwm_generate
--port map(
--    clk     => clk,
--    rst_l   => '1',
--    pwm_out => pwm_out
--);

--duplicate generated pwm signal
--pwm_in(0) <= pwm_out;
--pwm_in(1) <= pwm_out;
--pwm_in(2) <= pwm_out;
--pwm_in(3) <= pwm_out;
--pwm_in(4) <= pwm_out;
--pwm_in(5) <= pwm_out;

end Behavioral;
