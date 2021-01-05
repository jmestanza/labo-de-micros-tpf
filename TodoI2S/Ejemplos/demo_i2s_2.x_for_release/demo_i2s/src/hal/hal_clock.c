
#include "..\\include\\global.h"
#include "..\\include\\MKxxx.h"
#include "..\\include\\hal_config.h"


// xM --> /n --> 2M --> *24 --> 48M
void pll_init(void)
{
    #if (BOARD == BOARD_K60) || (BOARD == BOARD_K50)
        MCG_C2 = MCG_C2_RANGE0(2);
    #else
        MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK;
    #endif

    // after initialization of oscillator release latched state of oscillator and GPIO
    //SIM_SCGC4 |= SIM_SCGC4_LLWU_MASK;
    //LLWU_CS |= LLWU_CS_ACKISO_MASK;
  
    // Select external oscilator and Reference Divider and clear IREFS to start ext osc
    // CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
    MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);


    #if (BOARD == BOARD_K10) || (BOARD == BOARD_K40)
        // if we aren't using an osc input we don't need to wait for the osc to init
        // wait for oscillator to initialize
        while (!(MCG_S & MCG_S_OSCINIT0_MASK))
            ;
    #endif

    // wait for Reference clock Status bit to clear
    while (MCG_S & MCG_S_IREFST_MASK)
        ; 
    // Wait for clock status bits to show clock source is ext ref clk
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2)
        ; 

    // Now in FBE
    #if (BOARD == BOARD_K10) || (BOARD == BOARD_K40)
    // 输入时钟4分频,8M->2M
    MCG_C5 = MCG_C5_PRDIV0(3);
    #endif

    #if (BOARD == BOARD_K50) || (BOARD == BOARD_K60)
    // 输入时钟25分频,50M->2M
    MCG_C5 = MCG_C5_PRDIV0(24);
    #endif

    // Ensure MCG_C6 is at the reset default of 0. 
    // LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
    MCG_C6 = 0x0;
    // set_sys_dividers(0,1,1,3), to get 1,2,2,4;
    SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) 
                | SIM_CLKDIV1_OUTDIV3(1) | SIM_CLKDIV1_OUTDIV4(3);

    // 0 = 24倍频
    MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0); 

    while (!(MCG_S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set
    while (!(MCG_S & MCG_S_LOCK0_MASK)){}; // Wait for LOCK bit to set

    // Now running PBE Mode
    // Transition into PEE by setting CLKS to 0
    MCG_C1 &= ~MCG_C1_CLKS_MASK;

    // Wait for clock status bits to update
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3)
        ;

    // Now running PEE Mode
}

void port_clock_init(void)
{
    // enable port clock
    SIM_SCGC5 |=  1u << SIM_SCGC5_PORTA_SHIFT; 
    SIM_SCGC5 |=  1u << SIM_SCGC5_PORTB_SHIFT; 
    SIM_SCGC5 |=  1u << SIM_SCGC5_PORTC_SHIFT; 
    SIM_SCGC5 |=  1u << SIM_SCGC5_PORTD_SHIFT; 
    SIM_SCGC5 |=  1u << SIM_SCGC5_PORTE_SHIFT; 
}


void hal_clock_init(void)
{
    pll_init();
    port_clock_init();
}


