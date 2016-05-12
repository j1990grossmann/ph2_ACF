/*

    \file                          Definition.h
    \brief                         Definition File, listing the registers
    \author                        Nicolas PIERRE
    \version                       1.0
    \date                          07/06/14
    Support :                      mail to : nico.pierre@icloud.com

 */



//-----------------------------------------------------------------------------
//Glib Config Files

#define XML_DESCRIPTION_FILE_2CBC "settings/HWDescription_2CBC.xml"
#define XML_DESCRIPTION_FILE_8CBC "settings/HWDescription_8CBC.xml"
#define XML_DESCRIPTION_FILE_16CBC "settings/Beamtest_Nov15.xml"


//Time out for stack writing
#define TIME_OUT         5


//------------------------------------------------------------------------------
//Events
#define NCHANNELS                   254

//in uint32_t words
#define CBC_EVENT_SIZE_32   9 // 9 32bit words per CBC
#define EVENT_HEADER_TDC_SIZE_32    6 // total of 6 32 bit words for HEADER + TDC
#define EVENT_HEADER_SIZE_32    5  // 5 words for the header

//in chars
#define CBC_EVENT_SIZE_CHAR 9 * 4
#define EVENT_HEADER_TDC_SIZE_CHAR  6 * 4
#define EVENT_HEADER_SIZE_CHAR    5 * 4


//Event
#define OFFSET_BUNCH               8
#define WIDTH_BUNCH                24
#define OFFSET_ORBIT               1*32+8
#define WIDTH_ORBIT                24
#define OFFSET_LUMI                2*32+8
#define WIDTH_LUMI                 24
#define OFFSET_EVENT_COUNT         3*32+8
#define WIDTH_EVENT_COUNT          24
#define OFFSET_EVENT_COUNT_CBC     4*32+8
#define WIDTH_EVENT_COUNT_CBC      3*8

//Cbc Event
#define NCHANNELS                   254
#define OFFSET_ERROR              0
#define WIDTH_ERROR               2
#define OFFSET_PIPELINE_ADDRESS   2       //OFFSET_ERROR + WIDTH_ERROR
#define WIDTH_PIPELINE_ADDRESS    8
#define OFFSET_CBCDATA            2+8     //OFFSET_PIPELINE_ADDRESS + WIDTH_PIPELINE_ADDRESS
#define WIDTH_CBCDATA             254     //NCHANNELS
#define OFFSET_GLIBFLAG           10+254  //OFFSET_CBCDATA + WIDTH_CBCDATA
#define WIDTH_GLIBFLAG            12
#define OFFSET_CBCSTUBDATA        264+23  //LAST BIT
#define WIDTH_CBCSTUBDATA         12
//------------------------------------------------------------------------------
//MPAL Event
#define MPAL_EVENT_SIZE_72   96 // 96 72bit words per MPA
#define MPAL_WORD_SIZE       72 // 96 72bit words per MPA
// Asynchrounous
#define MPAL_ASYNCHROUNOUS_HEADER_OFFSET                      0
#define MPAL_ASYNCHROUNOUS_HEADER_WIDTH                       32
#define MPAL_ASYNCHROUNOUS_RIPPLE_COUNTER_BITS                16
#define MPAL_NCHANNELS                                        48
// Synchrounous
// MPA_BX_WIDTH
#define MPAL_BX_WIDTH                           16
// MPA_STUB_WORD
#define MPAL_STUB_OFFSET_Z_POS                  0
#define MPAL_STUB_WIDTH_Z_POS                   2
#define MPAL_STUB_OFFSET_BENDING                2
#define MPAL_STUB_WIDTH_BENDING                 5
#define MPAL_STUB_OFFSET_POSITION               7
#define MPAL_STUB_WIDTH_POSITION                7
// MPA_CENTROID_WORD
#define MPAL_CENTROID_Z_POS_OFFSET 0
#define MPAL_CENTROID_Z_POS_WIDTH  2
#define MPAL_CENTROID_POSITION_OFFSET  2
#define MPAL_CENTROID_POSITION_OFFSET  6
// Stub Finding Logic Mode OM 00
#define MPAL_SF_MODE_STUBS_PER_BX              4
#define MPAL_SF_MODE_BX_OFFSET                 52
#define MPAL_SF_MODE_HEADER_OFFSET             68
#define MPAL_SF_MODE_HEADER_WIDTH              4
// No Processing Mode OM 11
#define MPAL_NP_MODE_PIXEL_MATRIX_HITS_OFFSET 0
#define MPAL_NP_MODE_PIXEL_MATRIX_HITS_WIDTH  48
#define MPAL_NP_MODE_BX_OFFSET                48
#define MPAL_NP_MODE_HEADER_OFFSET            64
#define MPAL_NP_MODE_HEADER_WIDTH             8
// Strip Emulator Mode OM 01
#define MPAL_CENTROID_EXTRACTION_MODE_CENTROIDS_PER_BX  8
#define MPAL_CENTROID_EXTRACTION_MODE_BX_OFFSET         56
// Centroid Extraction Mode OM 11

//------------------------------------------------------------------------------