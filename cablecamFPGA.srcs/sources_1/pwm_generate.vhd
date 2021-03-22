LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.ALL;

entity pwm_generate is
    port(
        clk     : in std_logic;
        rst_l   : in std_logic;
        pwm_out : out std_logic
    );
end pwm_generate;

architecture behavioral of pwm_generate is

    signal test_cnt  : unsigned(31 downto 0) := (others => '0');
    
    signal inc_cnt   : unsigned(31 downto 0) := (others => '0');
    signal period    : unsigned(31 downto 0) := to_unsigned(200,32);
    signal duty      : unsigned(31 downto 0) := to_unsigned(100,32);
    
    constant inc_period : integer := 100000000;

begin

    pwm_gen : process(rst_l, clk) 
    begin
        if rst_l = '0' then
            test_cnt <= (others => '0');
            pwm_out <= '0';
        else
            if rising_edge(clk) then
                if test_cnt < period then
                    test_cnt <= test_cnt + to_unsigned(1, 32);
                else
                    test_cnt <= (others => '0');
                end if;
                
                if test_cnt < duty then
                    pwm_out <= '1';
                else
                    pwm_out <= '0';
                end if;
            end if;
        end if;
    end process;
    
    increment : process(rst_l, clk)
    begin
        if rst_l = '0' then
            period <= to_unsigned(200,32);
            duty   <= to_unsigned(100,32);
        else
            if rising_edge(clk) then
                if inc_cnt < inc_period then
                    inc_cnt <= inc_cnt + to_unsigned(1, 32);
                    period <= period;
                    duty <= duty;
                else
                    inc_cnt <= (others => '0');
                    period <= period + to_unsigned(1,32);
                    duty <= duty + to_unsigned(1, 32);
                end if;
            end if;
        end if;
    end process;

end behavioral;