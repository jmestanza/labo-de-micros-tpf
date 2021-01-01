

#ifndef __HW_AUDIO_SGTL5000__
#define __HW_AUDIO_SGTL5000__


bool hw_audio_sgtl5000_test_reg(void);
void hw_audio_sgtl5000_init(void);
void hw_audio_sgtl5000_play(s32 *sin_table, s32 *sin_table2, uint buf_size);

#endif

