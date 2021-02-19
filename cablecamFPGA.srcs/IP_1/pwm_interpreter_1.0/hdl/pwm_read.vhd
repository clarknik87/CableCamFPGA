----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 02/18/2021 10:18:34 PM
-- Design Name: 
-- Module Name: pwm_read - behavioral
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
use IEEE.NUMERIC_STD.ALL;

entity pwm_read is
    port( 
        clk         : in std_logic;
        rst_l       : in std_logic;
        pwm_in      : in std_logic;
        enable_reg  : in std_logic_vector(31 downto 0);
        period_reg  : out std_logic_vector(31 downto 0);
        duty_reg    : out std_logic_vector(31 downto 0);
        interrupt   : out std_logic
    );
end pwm_read;

architecture Behavioral of pwm_read is

    signal enable           : std_logic;
    signal int_enable       : std_logic;

    type state_type is (LOW, HIGH);
    signal state            : state_type;
    
    signal duty_cnt         : unsigned(31 downto 0) := (others => '0');
    signal period_cnt       : unsigned(31 downto 0) := (others => '0');
    
    signal duty_reg_buf     : std_logic_vector(31 downto 0) := (others => '0');
    signal period_reg_buf   : std_logic_vector(31 downto 0) := (others => '0');
    
    constant int_length     : unsigned(3 downto 0) := to_unsigned(3, 4);
    signal int_state        : state_type;
    signal int_cnt          : unsigned(3 downto 0) := (others => '0');
    signal duty_reg_comp    : std_logic_vector(31 downto 0) := (others => '0');
    signal period_reg_comp  : std_logic_vector(31 downto 0) := (others => '0');
    

begin
    
    enable      <= enable_reg(0);
    int_enable  <= enable_reg(1);

    duty_reg    <= duty_reg_buf;
    period_reg  <= period_reg_buf;
    
    sync : process(clk, rst_l, enable)
    begin
        if rising_edge(clk) then
            
            if (rst_l = '0' or enable = '0') then
                state   <= LOW;
                duty_cnt        <= (others => '0');
                period_cnt      <= (others => '0');
            else
            
                case state is
            
                    when LOW =>
                        if pwm_in = '1' then
                            duty_cnt   <= (others => '0');
                            period_cnt <= to_unsigned(1, period_cnt'length);
                            state      <= HIGH;
                        else
                            duty_cnt   <= duty_cnt;
                            period_cnt <= period_cnt+1;
                            state      <= LOW;
                        end if;
                    
                    when HIGH =>
                        if pwm_in = '1' then
                            duty_cnt   <= duty_cnt+1;
                            period_cnt <= period_cnt+1;
                            state      <= HIGH;
                        else
                            duty_cnt   <= duty_cnt+1;
                            period_cnt <= period_cnt+1;
                            state      <= LOW;
                        end if;
                    
            end case;
                                
            end if;
            
            
        end if;
    end process;

    update_reg : process(pwm_in, enable)
    begin
        if rst_l = '0' then
            duty_reg_buf    <= (others => '0');
            period_reg_buf  <= (others => '0');
        else
            if enable = '1' then
                if rising_edge(pwm_in) then
                    duty_reg_buf   <= std_logic_vector(duty_cnt);
                    period_reg_buf <= std_logic_vector(period_cnt);
                else
                    duty_reg_buf   <= duty_reg_buf;
                    period_reg_buf <= period_reg_buf;
                end if;
            else
                duty_reg_buf   <= duty_reg_buf;
                period_reg_buf <= period_reg_buf;
            end if;
        end if;
    end process;
    
    compare_regs : process(clk, int_enable, rst_l)
    begin
        if rising_edge(clk) then
        
            duty_reg_comp   <= duty_reg_buf;
            period_reg_comp <= period_reg_comp;
        
            if (rst_l = '0' or int_enable = '0') then
                interrupt   <= '0';
                int_cnt     <= (others => '0');
                int_state   <= LOW;
            else
                
                case int_state is
                
                    when LOW    =>
                        if duty_reg_comp = duty_reg_buf then
                            interrupt   <= '0';
                            int_cnt     <= (others => '0');
                            int_state   <= LOW;
                        else
                            interrupt   <= '1';
                            int_cnt     <= to_unsigned(1, int_cnt'length);
                            int_state   <= HIGH;
                        end if;
                        
                    when HIGH   =>
                        if int_cnt = int_length then
                            interrupt   <= '0';
                            int_cnt     <= (others => '0');
                            int_state   <= LOW;
                        else
                            interrupt   <= '1';
                            int_cnt     <= int_cnt+to_unsigned(1, int_cnt'length);
                            int_state   <= HIGH;
                        end if;
                        
                end case;
                
            end if;         
            
        end if;
    end process;

end Behavioral;
