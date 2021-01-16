#include <stdio.h>
#include "mp3Decoder.h"

#define DEBUG
#include <stdint.h>
#include <stdio.h>
#include "wav.h"

#include "music.h"

#define MAIN_DEBUG

#define INTBUF

#ifdef INTBUF
#define SAMPLE_FORMAT WAV_FORMAT_PCM
#endif

//#define FILEPATH		"C:/Users/Guido/Documents/GitHub/LaboDeMicrosTPF/TestS/MP3_test_pc/MP3_test_PC/MP3_test_PC/test/test.mp3"
//#define FILEPATH_WAV	"C:/Users/Guido/Documents/GitHub/LaboDeMicrosTPF/TestS/MP3_test_pc/MP3_test_PC/MP3_test_PC/test/test.wav"

//#define FILEPATH		"test/thomas_16k.mp3"
#define	FILEPATH		"banana"
#define FILEPATH_WAV	"test/thomas_16k.wav"

//#define SAMPLE_RATE		44100
#define SAMPLE_RATE		16000



#define NUM_CHANNELS	1

static short buffer[MP3_DECODED_BUFFER_SIZE];


int main(void) {
	printf("********************************* \n");
	printf(" HELIX MP3 DECODER TESTBENCH (PC) \n");
	printf("********************************* \n");

	uint16_t sampleCount;
	uint32_t sr = 0;
	uint8_t j = 0;
	WavFile* wavIn, * wavOut, * wav;
	mp3_decoder_frame_data_t frameData;
	mp3_decoder_tag_data_t ID3Data;


	wav = wav_open(FILEPATH_WAV, "wb");
	wav_set_format(wav, SAMPLE_FORMAT);
	wav_set_sample_rate(wav, SAMPLE_RATE);
	wav_set_num_channels(wav, 1);



	MP3DecoderInit();
//	MP3LoadFile(const char* file_name, uint8_t * audio, uint32_t audio_len)
	if (MP3LoadFile(FILEPATH, thomas_16k_array,sizeof(thomas_16k_array))){

		int i = 0;
		while (1)
		{
#ifdef MAIN_DEBUG
			printf("\n[APP] Frame %d decoding started.\n", i);
#endif
			mp3_decoder_result_t res = MP3GetDecodedFrame(buffer, MP3_DECODED_BUFFER_SIZE, &sampleCount, 0);
			if (res == 0)
			{	
				MP3GetLastFrameData(&frameData);
				if (sr != frameData.sampleRate)
				{
					int huevo = 0;
					huevo++;
				}
#ifdef MAIN_DEBUG
				printf("[APP] Frame %d decoded.\n", i);
#endif
				i++;

				sr = frameData.sampleRate;
#ifdef MAIN_DEBUG
				printf("[APP] FRAME SAMPLE RATE: %d \n", sr);
#endif

				int16_t auxBuffer[MP3_DECODED_BUFFER_SIZE];
				for (uint32_t j = 0; j < sampleCount / frameData.channelCount; j++)
				{
					auxBuffer[j] = buffer[frameData.channelCount * j];
				}
				wav_write(wav, auxBuffer, sampleCount / frameData.channelCount);
			}
			else if (res == MP3DECODER_FILE_END)
			{
				printf("[APP] FILE ENDED. Decoded %d frames.\n", i - 1);
				wav_close(wav);
				break;
			}
			else
			{
				int huevo = 0;
				huevo++;
			}
		}
	}
	else
	{
		printf("Couldnt open file\n");
	}


	return 0;
}