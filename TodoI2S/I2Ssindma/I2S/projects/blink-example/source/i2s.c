/*
 * i2s.c
 *
 *  Created on: Feb 1, 2020
 *      Author: joa-m
 */

#include "i2s.h"

static I2S_Type * i2s_ptr = I2S0;
static SIM_Type* sim_ptr = SIM;
static PORT_Type* portPointers[] = PORT_BASE_PTRS;


#define WORDS_PER_FRAME	2
#define WORD_BITS		24


bool isWordStartFlag(uint32_t csr){
	return (csr & I2S_TCSR_WSF_MASK) == I2S_TCSR_WSF_MASK;
}

bool isSyncErrorFlag(uint32_t csr){
	return (csr & I2S_TCSR_SEF_MASK) == I2S_TCSR_SEF_MASK;
}

bool isFIFOErrorFlag(uint32_t csr){
	return (csr & I2S_TCSR_FEF_MASK) == I2S_TCSR_FEF_MASK;
}

bool isFIFOWarningFlag(uint32_t csr){
	return (csr & I2S_TCSR_FWF_MASK) == I2S_TCSR_FWF_MASK;
}

bool isFIFORequestFlag(uint32_t csr){
	return (csr & I2S_TCSR_FRF_MASK) == I2S_TCSR_FRF_MASK;
}


__ISR__ I2S0_Tx_IRQHandler(void){

	if(isWordStartFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_WSF_MASK; //  clear the flag, w1c
	}
	if(isSyncErrorFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR &= ~I2S_TCSR_SEF_MASK; //  clear the sync error flag w1c
	}
	if(isFIFOErrorFlag(i2s_ptr->TCSR)){
		i2s_ptr->TCSR |= I2S_TCSR_FEF_MASK; //  clear the fifo error flag w1c
		i2s_ptr->TCSR |= I2S_TCSR_FR_MASK; // FIFO RESET!
	}
	if(isFIFOWarningFlag(i2s_ptr->TCSR)){
		// Indicates that an enabled transmit FIFO is empty.
		i2s_ptr->TCSR &= ~I2S_TCSR_FWF_MASK; //  clear the fifo warning flag
	}
	if(isFIFORequestFlag(i2s_ptr->TCSR)){
		// Indicates that the number of words in an enabled transmit channel FIFO
		//is less than or equal to the transmit FIFO watermark.
		i2s_ptr->TCSR &= ~I2S_TCSR_FRF_MASK; //  clear the fifo request flag
	}
}

void i2s_set_pin(pin_t pin, uint8_t mux_alt, bool irqEnabled){
	sim_ptr->SCGC6 |= SIM_SCGC6_I2S_MASK;  // module clk gating
	sim_ptr->SCGC5 |= SIM_SCGC5_PORTB_MASK; // clk gating port B and C
	sim_ptr->SCGC5 |= SIM_SCGC5_PORTC_MASK;

	PORT_Type * port = portPointers[PIN2PORT(pin)];

	port-> PCR[PIN2NUM(pin)] &=~(PORT_PCR_PE_MASK |
								PORT_PCR_PS_MASK |
								PORT_PCR_MUX_MASK |
								PORT_PCR_IRQC_MASK); // set all values to edit to zero

	port-> PCR[PIN2NUM(pin)] |= PORT_PCR_PE(0) |
								PORT_PCR_PS(0) |
								PORT_PCR_MUX(mux_alt) |
								PORT_PCR_IRQC(irqEnabled); // edit them
}

void i2s_init(void){


	bool irqEnabled = false;
	//clock gating (ports and module) and mux alternative selection
	i2s_set_pin( PIN_I2S_TX_BLCK, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_TX_FS, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_TX_D0, 6, irqEnabled);
	i2s_set_pin( PIN_I2S_MCLK, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_RX_BCLK, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_RX_FS, 4, irqEnabled);
	i2s_set_pin( PIN_I2S_RX_D0, 4, irqEnabled);

	i2s_ptr->MCR = (i2s_ptr->MCR & ~I2S_MCR_DUF_MASK) | I2S_MCR_DUF(0);
	i2s_ptr->MCR = (i2s_ptr->MCR & ~I2S_MCR_MOE_MASK) | I2S_MCR_MOE(1);
	i2s_ptr->MCR = (i2s_ptr->MCR & ~I2S_MCR_MICS_MASK) | I2S_MCR_MICS(0);

	// por UDA1380 => mclk = 256fs => si fs = 48KHz => mclk = 12.288MHz

	i2s_ptr->MDR =  (i2s_ptr->MDR & ~I2S_MDR_DIVIDE_MASK) | I2S_MDR_DIVIDE(1041); // lo llevo a 96KHz (48/2)
	i2s_ptr->MDR =  (i2s_ptr->MDR & ~I2S_MDR_FRACT_MASK) | I2S_MDR_FRACT(127); // lo multiplico por 128 (256/2)

	// y de esa forma llego a 12.288MHz

	//This register (2, 3, 4 and 5) must not be altered when TCSR[TE] is set (TX_SET_REG).
	i2s_ptr->TCSR &= ~I2S_TCSR_TE_MASK;
	i2s_ptr->TCSR |= I2S_TCSR_FR_MASK; // FIFO RESET!

	tx_config_regs tx_cfg;

	tx_cfg.tx_cfg_1_reg.transmit_fifo_watermark = 7; // max value just in case

	tx_set_reg_1(& tx_cfg.tx_cfg_1_reg);

	tx_cfg.tx_cfg_2_reg.synchronous_mode = asynchronous_mode; // ??
	tx_cfg.tx_cfg_2_reg.bit_clock_swap = false; // use the normal bit clock source
	tx_cfg.tx_cfg_2_reg.bit_clock_input = false; //??

	tx_cfg.tx_cfg_2_reg.master_clock_select = bus_clock;
	tx_cfg.tx_cfg_2_reg.master_clock_select = master_clock_3;// andan todos, elegi uno al azar


	tx_cfg.tx_cfg_2_reg.bit_clock_polarity = false; // clk active high when => out rising edge and sample input falling edge
	tx_cfg.tx_cfg_2_reg.bit_clock_direction = true; // bit clk is gneerated internally in Master mode
	tx_cfg.tx_cfg_2_reg.bit_clock_divide = 4; // divides by (DIV+1)*2
	// si quiero fs = 48kHz
	// un sample son 24 bits.
	// f_bit_clk/24 = fs => si fs = 48KHz => f_bit_clk = 1.152MHz

	// ahora cuanto tengo que dividir mclk para obtener ese bit clk
	//	12.288MHz/1.152MHz = 10.66 => 10? pero divide por (DIV+1)*2 asi que DIV = 4?

	// con DIV = 4 => f_bit_clk = 1.228MHz != 1.152MHz  => err = 76KHz
	// con DIV = 5 => f_bit_clk = 1.0240MhZ != 1.152MHz => err = 128KHz

	// con 16 bits da justo el numero! creo que convendria eso, por lo de los clocks

	// con esto seteo el fs => ... f_bit_clk = m_clk/ [(div+1)*2]

	tx_set_reg_2(& tx_cfg.tx_cfg_2_reg);

	tx_cfg.tx_cfg_3_reg.transmit_channel_1_enable = true;
	tx_cfg.tx_cfg_3_reg.transmit_channel_2_enable = true;
	tx_cfg.tx_cfg_3_reg.word_flag_configuration = 0; // ??

	tx_set_reg_3(& tx_cfg.tx_cfg_3_reg); // dsp de esto sale warning y request fifo

	tx_cfg.tx_cfg_4_reg.frame_size = WORDS_PER_FRAME-1; // (N+1) word per frame
	tx_cfg.tx_cfg_4_reg.sync_width = ((WORDS_PER_FRAME*WORD_BITS)/2) -1; // #bit_clks_deseados - 1
	// esto deberia ser la mitad, para que sea L-R-L-R, pero podria ser L-L-L-R-R-R-...

	// este numero cambia si:
	// - tengo mas frames (se multiplica por la cantidad de frames)
	// - numero de bits usados por palabra.
	// la cuenta seria algo como: (number_of_frames*bits_per_word)/2 - 1


	tx_cfg.tx_cfg_4_reg.is_MSB_first = true;
	tx_cfg.tx_cfg_4_reg.frame_sync_early = false; // frame sync asserts with first bit of the frame
	tx_cfg.tx_cfg_4_reg.frame_sync_polarity = false; // frame sync active high
	tx_cfg.tx_cfg_4_reg.frame_sync_direction = true ; // frame sync is generated internally in master mode
	tx_set_reg_4(& tx_cfg.tx_cfg_4_reg);

	tx_cfg.tx_cfg_5_reg.word_n_width = WORD_BITS-1; // (N-1)number of bits in each word except for the first
	tx_cfg.tx_cfg_5_reg.word_0_width = WORD_BITS-1;
	tx_cfg.tx_cfg_5_reg.first_bit_shifted = -1;

	tx_set_reg_5(& tx_cfg.tx_cfg_5_reg);


	tx_cfg.tx_cfg_0_reg.rx_tx_enable = true; // solo es tx
	tx_cfg.tx_cfg_0_reg.stop_enable = false; // transmitter enabled in stop mode
	tx_cfg.tx_cfg_0_reg.debug_enable = false; // transmitter enabled in debug mode
	tx_cfg.tx_cfg_0_reg.bit_clock_enable = true; // if Trans. En. is set, this too.
	//when software clear this field the T.E remains enabled and this bit too until the end of the frame
	tx_cfg.tx_cfg_0_reg.fifo_reset = false; // should reset when Tx is disabled or the fifo error flag is set
	tx_cfg.tx_cfg_0_reg.software_reset = false; // software resets if 1
	// aca falta sync error flag
	//tx_cfg.tx_cfg_0_reg.fifo_error_flag   don't config
	//tx_cfg.tx_cfg_0_reg.fifo_warning_flag  don't config
	//tx_cfg.tx_cfg_0_reg.fifo_request_flag don't config
	tx_cfg.tx_cfg_0_reg.word_start_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.sync_error_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_error_interrupt_enable = true;
	tx_cfg.tx_cfg_0_reg.fifo_warning_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_request_interrupt_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_warning_dma_enable = false;
	tx_cfg.tx_cfg_0_reg.fifo_request_dma_enable = false;

	tx_set_reg_0(& tx_cfg.tx_cfg_0_reg);

	//NVIC_EnableIRQ(I2S0_Rx_IRQn);
	NVIC_EnableIRQ(I2S0_Tx_IRQn);

}

bool isFIFOFull(uint32_t tranfer_fifo_register_n){

	uint32_t WFP = tranfer_fifo_register_n & I2S_TFR_WFP_MASK;
	uint32_t RFP = tranfer_fifo_register_n & I2S_TFR_RFP_MASK;
	uint32_t WFP_MSB = (WFP & (1<<19)) == (1<<19);
	uint32_t RFP_MSB = (RFP & (1<<3)) == (1<<3);
	uint32_t WFP_others = ( WFP & ~(1<<19) ) >> 16; // creo que es 16 y no 15, hay que testear
	uint32_t RFP_others = ( RFP & ~(1<<3) );

	if( (WFP_others == RFP_others) && (WFP_MSB != RFP_MSB) ){
		return true; // fifo full
	}else{
		return false; // fifo not full
	}
}


void i2s_send_data(uint32_t msg){



//	Transmit Data Register
//	The corresponding TCR3[TCE] bit must be set before accessing the channel's transmit data register.
//	Writes to this register when the transmit FIFO is not full
//  will push the data written into the transmit data
//	FIFO. Writes to this register when the transmit
// 	FIFO is full are ignored.
	uint32_t TFR0 = i2s_ptr->TFR[0];



	while(isFIFOFull(TFR0)){
	}

	if(!isFIFOFull(TFR0)){
		i2s_ptr->TDR[0] = msg;
	}

	while(isFIFOFull(TFR0)){
	}


}

void SAI_TxEnable(I2S_Type *base, bool enable)
{
    if (enable)
    {
        /* If clock is sync with Rx, should enable RE bit. */
        if (((base->TCR2 & I2S_TCR2_SYNC_MASK) >> I2S_TCR2_SYNC_SHIFT) == 0x1U)
        {
            base->RCSR = ((base->RCSR & 0xFFE3FFFFU) | I2S_RCSR_RE_MASK);
        }
        base->TCSR = ((base->TCSR & 0xFFE3FFFFU) | I2S_TCSR_TE_MASK);
        /* Also need to clear the FIFO error flag before start */
        SAI_TxClearStatusFlags(base, kSAI_FIFOErrorFlag);
    }
    else
    {
        /* If RE not sync with TE, than disable TE, otherwise, shall not disable TE */
        if (((base->RCR2 & I2S_RCR2_SYNC_MASK) >> I2S_RCR2_SYNC_SHIFT) != 0x1U)
        {
            /* Should not close RE even sync with Rx */
            base->TCSR = ((base->TCSR & 0xFFE3FFFFU) & (~I2S_TCSR_TE_MASK));
        }
    }
}

