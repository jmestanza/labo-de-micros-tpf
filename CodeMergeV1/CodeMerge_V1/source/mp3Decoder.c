#include "mp3Decoder.h"

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <string.h>
#include <stdbool.h>  
#include "mp3decoder.h"
#include "lib/helix/pub/mp3dec.h"
#include "utilities/fsl_debug_console.h"
#include "fatfs/fatfs_include/ff.h"

 /*******************************************************************************
  * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
  ******************************************************************************/

#define MP3DECODER_MODE_NORMAL  0
#define MP3_FRAME_BUFFER_BYTES  6913            // MP3 buffer size (in bytes)
#define DEFAULT_ID3_FIELD       "Unknown"
#define MAX_DEPTH       3

// #define DEBUG_PRO  // enable Debug messages on Debug console

  /*******************************************************************************
   * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
   ******************************************************************************/
typedef struct
{
    // Helix structures
    HMP3Decoder   Decoder;                                   // Helix MP3 decoder instance
    MP3FrameInfo  last_frame_info;                                  // current MP3 frame info

    FIL			file;
    FIL*        mp3File;

    bool 		  sd_initialized;
    uint32_t      file_size;                                       // file size
    uint32_t      bytes_remaining;                                 // Encoded MP3 bytes remaining to be processed by either offset or decodeMP3
    bool          file_opened;                                     // true if there is a loaded file
    uint16_t      last_frame_length;                                // Last frame length

    // MP3-encoded buffer
    uint8_t       encoded_frame_buffer[MP3_FRAME_BUFFER_BYTES];         // buffer for MP3-encoded frames
    uint32_t      top_index;                                            // current position in frame buffer (points to top_index)
    uint32_t      bottom_index;                                         // current position at info end in frame buffer

}mp3_decoder_context_t;
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
//File management functions
static uint32_t getFileSize(void);
static void fileRewind(void);
static uint16_t readFile(void* buf, uint16_t cnt);

//data management
static void copyDataAndMovePointer(void);
static void copyFrameInfo(mp3_decoder_frame_data_t* mp3_data, MP3FrameInfo* helix_data);
static void resetContextData(void);
/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static mp3_decoder_context_t this;
static FRESULT fr;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static bool open_file(const char* file_name) {

    bool ret = false;

    fr = f_open(&this.file, file_name, FA_READ);
    if (fr == FR_OK)
    {
        this.mp3File = &(this.file);
        ret = true;
    }
    return ret;
}

void  MP3DecoderInit(void) {
    resetContextData();
    this.Decoder = MP3InitDecoder();
}


bool  MP3LoadFile(const char* file_name) {

	if(!this.sd_initialized){
		PRINTF("SD not initialized previously!.\r\n");
		return false;
	}

    bool res = false;
    if (this.file_opened == true) {//if there was a opened file, i must close it before opening a new one
        resetContextData();
        MP3CloseFile();
    }
    if (open_file(file_name)){
        this.file_opened = true;
        this.file_size = getFileSize();
        this.bytes_remaining = this.file_size;
#ifdef DEBUG_PRO
        printf("File opened successfully!\nFile size is %d bytes\n", this.file_size);
#endif
        res = true;
    }
    return res;

}

void MP3CloseFile(void) {
    f_close(this.mp3File);
    this.file_opened = false;
}

uint32_t getFileSize(void) {
    uint32_t ret = 0;
    if (this.file_opened)
    {
        ret = f_size(this.mp3File);
    }
    return ret;
}


void MP3SetSDInitializedFlag(void){
	this.sd_initialized = true;
}

bool MP3GetLastFrameData(mp3_decoder_frame_data_t* data) {
    bool ret = false;

    if (this.bytes_remaining < this.file_size)
    {
        copyFrameInfo(data, &this.last_frame_info);
        ret = true;
    }
    return ret;
}

mp3_decoder_result_t MP3GetDecodedFrame(short* outBuffer, uint16_t bufferSize, uint16_t* samples_decoded, uint8_t depth) {

    mp3_decoder_result_t ret = MP3DECODER_NO_ERROR;    // Return value of the function

#ifdef DEBUG_PRO
    PRINTF("Entered decoding. File has %d bytes to decode\n", this.bytes_remaining);
    PRINTF("Buffer has %d bytes to decode\n", this.bottom_index - this.top_index);
#endif

    if (depth < MAX_DEPTH)
    {
        if (!this.file_opened)
        {
            ret = MP3DECODER_NO_FILE;
#ifdef DEBUG_PRO
            PRINTF("There is no opened file\n");
#endif
        }
        else if (this.bytes_remaining) // check if there is remaining info to be decoded
        {
#ifdef DEBUG_PRO
            PRINTF("Current pointers are Head = %d - Bottom = %d\n", this.top_index, this.bottom_index);
#endif

            // scroll encoded info up in array if necessary (TESTED-WORKING)
            if ((this.top_index > 0) && ((this.bottom_index - this.top_index) > 0) && (this.bottom_index - this.top_index < MP3_FRAME_BUFFER_BYTES))
            {
                //memcpy(this.mp3FrameBuffer , this.mp3FrameBuffer + this.top_index, this.bottom_index - this.top_index);
                memmove(this.encoded_frame_buffer, this.encoded_frame_buffer + this.top_index, this.bottom_index - this.top_index);
                this.bottom_index = this.bottom_index - this.top_index;
                this.top_index = 0;

#ifdef DEBUG_PRO
                PRINTF("Copied %d bytes from %d to %d\n", (this.bottom_index - this.top_index), this.top_index, 0);
#endif
            }
            else if (this.bottom_index == this.top_index)
            {
                // If arrived here, there is nothing else to do
#ifdef DEBUG_PRO
                PRINTF("Empty buffer.\n");
#endif

            }
            else if (this.bottom_index == MP3_DECODED_BUFFER_SIZE)
            {
#ifdef DEBUG_PRO
                PRINTF("Full buffer.\n");
#endif
            }

            // Read encoded data from file
            copyDataAndMovePointer();

            // seek mp3 header beginning
            int offset = MP3FindSyncWord(this.encoded_frame_buffer + this.top_index, this.bottom_index);

            if (offset >= 0)
            {
                //! check errors in searching for sync words (there shouldnt be)
                this.top_index += offset; // updating top_index pointer
                this.bytes_remaining -= offset;  // subtract garbage info to file size

#ifdef DEBUG_PRO
                PRINTF("Sync word found @ %d offset\n", offset);
#endif
            }

            //check samples in next frame (to avoid segmentation fault)
            MP3FrameInfo nextFrameInfo;
            int err = MP3GetNextFrameInfo(this.Decoder, &nextFrameInfo, this.encoded_frame_buffer + this.top_index);
            if (err == 0)
            {
#ifdef DEBUG_PRO
                PRINTF("Frame to decode has %d samples\n", nextFrameInfo.outputSamps);
#endif
                if (nextFrameInfo.outputSamps > bufferSize)
                {
#ifdef DEBUG_PRO
                    PRINTF("Out buffer isnt big enough to hold samples.\n");
#endif
                    return MP3DECODER_BUFFER_OVERFLOW;
                }
            }


            // with array organized, lets decode a frame
            uint8_t* decPointer = this.encoded_frame_buffer + this.top_index;
            int bytesLeft = this.bottom_index - this.top_index;
            int res = MP3Decode(this.Decoder, &decPointer, &(bytesLeft), outBuffer, MP3DECODER_MODE_NORMAL);
            //! autodecrements fileSize with bytes decoded. updated inbuf pointer, updated bytesLeft

            if (res == ERR_MP3_NONE) // if decoding successful
            {
                uint16_t decodedBytes = this.bottom_index - this.top_index - bytesLeft;
                this.last_frame_length = decodedBytes;

#ifdef DEBUG_PRO
                PRINTF("Frame decoded!. MP3 frame size was %d bytes\n", decodedBytes);
#endif

                // update header pointer and file size
                this.top_index += decodedBytes;
                this.bytes_remaining -= decodedBytes;

                // update last frame decoded info
                MP3GetLastFrameInfo(this.Decoder, &(this.last_frame_info));

                // update samples decoded
                *samples_decoded = this.last_frame_info.outputSamps;

                // return success code
                ret = MP3DECODER_NO_ERROR;
            }
            else if (res == ERR_MP3_INDATA_UNDERFLOW || res == ERR_MP3_MAINDATA_UNDERFLOW)
            {
                if (this.bytes_remaining == 0)
                {
#ifdef DEBUG_PRO
                    PRINTF("[Error] Buffer underflow and file empty\n");
#endif

                    return MP3DECODER_FILE_END;
                }
#ifdef DEBUG_PRO
                PRINTF("Underflow error (code %d)\n", res);
#endif

                // If there weren't enough bytes on the buffer, try again
                return MP3GetDecodedFrame(outBuffer, bufferSize, samples_decoded, depth + 1);
            }
            else
            {
                if (this.bytes_remaining <= this.last_frame_length)
                {
#ifdef DEBUG_PRO
                    PRINTF("Dropped frame\n");
#endif
                    return MP3DECODER_FILE_END;
                }
                else
                {
                    this.top_index++;
                    this.bytes_remaining--;
#ifdef DEBUG_PRO
                    PRINTF("Error: %d\n", res);
#endif

                    // If invalid header, try with next frame
                    return MP3GetDecodedFrame(outBuffer, bufferSize, samples_decoded, depth + 1);
                }
            }
        }
        else
        {
            ret = MP3DECODER_FILE_END;
        }
    }
    else
    {
        ret = MP3DECODER_ERROR;
    }
    return ret;

}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

 //File management functions

uint16_t readFile(void* buf, uint16_t cnt) {

    uint16_t ret = 0;
    uint16_t read;

	fr = f_read(this.mp3File, ((uint8_t*)buf) + ret, cnt, (UINT*)&read);
    return fr == FR_OK ? read : ret;
}

void copyDataAndMovePointer() {
    uint16_t bytes_read;

    // Fill buffer with info in mp3 file

    uint8_t* dst = this.encoded_frame_buffer + this.bottom_index;

    bytes_read = readFile(dst, (MP3_FRAME_BUFFER_BYTES - this.bottom_index));
    // Update bottom_index pointer
    this.bottom_index += bytes_read;

#ifdef DEBUG_PRO
    if (bytes_read == 0)
    {
        PRINTF("File was read completely.\n");
    }
    PRINTF("[?] Read %d bytes from file. Head = %d - Bottom = %d\n", bytes_read, this.top_index, this.bottom_index);
#endif
}

void copyFrameInfo(mp3_decoder_frame_data_t* mp3_data, MP3FrameInfo* helix_data)
{
    mp3_data->bitRate = helix_data->bitrate;
    mp3_data->binitsPerSample = helix_data->bitsPerSample;
    mp3_data->channelCount = helix_data->nChans;
    mp3_data->sampleRate = helix_data->samprate;
    mp3_data->sampleCount = helix_data->outputSamps;
}

void resetContextData(void) {
	if(this.file_opened){
		MP3CloseFile();
	}else{
		this.file_opened = false;
	}
    this.bottom_index = 0;
    this.top_index = 0;
    this.bytes_remaining = 0;
    this.file_size = 0;
}
