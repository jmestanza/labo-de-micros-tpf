const sai_config_t SAI_1_tx_config = {
  .protocol = kSAI_BusI2S,
  .syncMode = kSAI_ModeAsync,
  .mclkOutputEnable = true,
  /* MCLK clock source */
  .mclkSource = kSAI_MclkSourceSysclk,
  .bclkSource = kSAI_BclkSourceMclkDiv, //kSAI_BclkSourceMclkOption1
  .masterSlave = kSAI_Master
};

sai_transfer_format_t SAI_1_tx_format = {
  .sampleRate_Hz = kSAI_SampleRate44100Hz,
  .bitWidth = kSAI_WordWidth24bits,
  .stereo = kSAI_Stereo,
  .masterClockHz = 6144000UL,
  .watermark = 4U,
  .channel = 0U,
  .protocol = kSAI_BusI2S,
  .isFrameSyncCompact = true
};


void SAI_1_init(void) {
  /* Initialize SAI Tx sub-module functionality */
  SAI_TxInit(SAI_1_PERIPHERAL, &SAI_1_tx_config);
  /* Initialize SAI Tx transfer format */
  SAI_TxSetFormat(SAI_1_PERIPHERAL, &SAI_1_tx_format, SAI_1_TX_MCLK_SOURCE_CLOCK_HZ, SAI_1_TX_BCLK_SOURCE_CLOCK_HZ);
  /* Enable selected Tx interrupts */
  SAI_TxEnableInterrupts(SAI_1_PERIPHERAL, (kSAI_WordStartInterruptEnable | kSAI_SyncErrorInterruptEnable | kSAI_FIFOWarningInterruptEnable | kSAI_FIFOErrorInterruptEnable | kSAI_FIFORequestInterruptEnable));
  /* Enable interrupt I2S0_Tx_IRQn request in the NVIC */
  EnableIRQ(SAI_1_SERIAL_TX_IRQN);
}
