library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

ENTITY toplevel is
  port (
    CLOCK_50           : in std_logic;
	 --reset	           : in std_logic;
	 
	 KEY : in std_logic_vector(3 downto 0);
	 SW : in std_logic_vector(7 downto 0);
	 
	 
	 HEX0 : out std_logic_vector(6 downto 0);
	 HEX1 : out std_logic_vector(6 downto 0);
	 HEX2 : out std_logic_vector(6 downto 0);
	 HEX4 : out std_logic_vector(6 downto 0);
	 HEX5 : out std_logic_vector(6 downto 0);
	 
	 GPIO_0 : out std_logic--output wave
	 
	 );
end entity toplevel;

architecture rtl of toplevel is

  signal hex0_Sig : std_logic_vector(6 downto 0);
  signal hex1_Sig : std_logic_vector(6 downto 0);
  signal hex2_Sig : std_logic_vector(6 downto 0);
  signal hex4_Sig : std_logic_vector(6 downto 0);
  signal hex5_Sig : std_logic_vector(6 downto 0);

  signal reset_n : std_logic;
  signal key0_d1 : std_logic;
  signal key0_d2 : std_logic;
  signal key0_d3 : std_logic;
  signal sw_d1 : std_logic_vector(7 downto 0);
  signal sw_d2 : std_logic_vector(7 downto 0);
  signal pwm : std_logic;
  
  signal key_enable_sig : std_logic_vector(3 downto 0);
  
	component nios_system is
		port (
			clk_clk                             : in  std_logic := 'X'; -- clk
			hex0_export                         : out std_logic_vector(6 downto 0);                    -- export
			hex1_export                         : out std_logic_vector(6 downto 0);                    -- export
			hex2_export                         : out std_logic_vector(6 downto 0);                    -- export
			hex4_export                         : out std_logic_vector(6 downto 0);                    -- export
			hex5_export                         : out std_logic_vector(6 downto 0);                    -- export
			pushbuttons_export                         : in  std_logic_vector(3 downto 0) := (others => 'X'); -- export
			my_custom_ip_0_writeresponsevalid_n : out std_logic;                                       -- writeresponsevalid_n
			reset_reset_n                       : in  std_logic := 'X'; -- reset_n
			switches_export                     : in  std_logic_vector(7 downto 0) := (others => 'X')  -- export
		);
	end component nios_system;


  
begin

  hex0(6 downto 0) <= hex0_Sig;
  hex1(6 downto 0) <= hex1_Sig;
  hex2(6 downto 0) <= hex2_Sig;
  hex4(6 downto 0) <= hex4_Sig;
  hex5(6 downto 0) <= hex5_Sig;
  
  key_enable_sig <= KEY;
  
  
  GPIO_0 <= pwm;
  
  

  synchReset_proc : process (CLOCK_50) begin
    if (rising_edge(CLOCK_50)) then
      key0_d1 <= KEY(0);
      key0_d2 <= key0_d1;
      key0_d3 <= key0_d2;
    end if;
  end process synchReset_proc;


  
  synchUserIn_proc : process (CLOCK_50) begin
    if (rising_edge(CLOCK_50)) then
      if (key0_d3 = '0') then

        sw_d1 <= x"00";
		  sw_d2 <= x"00";
		
		else
			sw_d1 <= SW;
			sw_d2 <= sw_d1;
        
      end if;
    end if;
  end process synchUserIn_proc;
  
  
	u0 : component nios_system
		port map (
			clk_clk                             => CLOCK_50,
			reset_reset_n                       => key0_d3,
			
			switches_export                     => sw_d2,--switches.export
			
			
			hex0_export                         => hex0_sig,                   
			hex1_export                         => hex1_sig,                        
			hex2_export                         => hex2_sig,                         
			hex4_export                         => hex4_sig,                         
			hex5_export                         => hex5_sig,                        
			pushbuttons_export                         => key_enable_sig,                         
			my_custom_ip_0_writeresponsevalid_n => pwm -- my_custom_ip_0.writeresponsevalid_n
			
			
		);

end architecture rtl;



	



