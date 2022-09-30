// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the PATCHER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// PATCHER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef PATCHER_EXPORTS
#define PATCHER_API 
#else
#define PATCHER_API 
#endif

enum EErrorID
{
	EID_OK = 0,
	EID_Unknown,			// Unknown error
	EID_GetDM,				// Getting DeviceMode for this printer failed (Is driver correctly installed?)
	EID_InvalidDM,			// DeviceMode for this printer is wrong (Is this EuroPlus driver?)
	EID_OutOfMemory,		// Memory allocation failed
	EID_APIFailure,			// Call to Windows API failed
	EID_BufferTooSmall,		// Supplied buffer is too small
	EID_WrongData,			// Data pointer supplied to Patcher entry point is corrupted
	EID_NullPointer,		// Data pointer supplied to Patcher entry point is NULL
	EID_DriverError,		// Driver reported error
	EID_UnsupportedValue,	// Wrong setting ID supplied to Patcher entry point
	EID_UnknownDrvVersion,	// Driver version is not V5 neither V8
};

#define PATCH_LABEL_HEIGHT	  1			// (System) Unit
#define PATCH_LABEL_WIDTH	  2			// (System) Unit
#define PATCH_MEDIA_TYPE	  3			// ZPL=SensorType: 0:Web Sensing|1:Continuous|2:Mark Sensing
										// EPL=SensorType: 0:Continuous|1:Label with marks|2:Label with gaps|3:Label with notch
										// CPCL=0:Journal|2:Gap sense|1:Black mark
#define PATCH_SPEED			  4			// Speed
#define PATCH_PRINT_MODE	  5			// ZPL=MediaType: 0:Direct|1:Transfer|2:Use printer settings EPL=MediaType: 0:Direct|1:Transfer
#define PATCH_ORIENTATION	  6			// (System) 0:Landscape|1:portrait
#define PATCH_MEDIA_HANDLING  7			// ZPL=PrintMode: 0:Tear Off|1:Rewind|2:Peel Off|3:Cutter|4:Applicator|5:RFID EPL=Switch6: 0|None|1|Peel off|2|Linerless
#define PATCH_DARKNESS        8			// Darkness
#define PATCH_ERROR_REPORTING 9			// N/A
#define PATCH_CODE_PAGE       10		// N/A
#define PATCH_TEAR_OFF        11		// ZPL=PaperOut: -100..100
#define PATCH_BACKFEED        12		// ZPL=Switch1: 0:No backfeed|1:Default settings|2:Before printing|3:After printing
#define PATCH_TOF_BACKUP      13		// ZPL=N/A EPL=ContinuousForm 0:No|1:Yes
#define PATCH_PANEL           14		// N/A
#define PATCH_MEDIA_ACTION    15		// ZPL=N/A
#define PATCH_MIRROR_IMAGE    16		// ZPL=Switch5: 0:No|1:Yes
#define PATCH_LABEL_SHIFT     17		// ZPL=Unit1: -9999..9999
#define PATCH_REAL_TIME_CLOCK 18		// N/A
#define PATCH_SET_TIME_DATE   19		// N/A
#define PATCH_FORMAT_TIME     20		// N/A
#define PATCH_FORMAT_DATE     21		// N/A
#define PATCH_MODE            22		// N/A
#define PATCH_OFFSET          23		// N/A
#define PATCH_PRINTER_TYPE    24		// Printer type (see PRTYPE_??? constant)
#define PATCH_DPI			  25		// dots/inch
#define PATCH_SPEEDUNIT		  26		// description for speed unit
#define PATCH_GAP_LEN		  27		// gap length (EPL)
#define PATCH_GAP_OFFSET	  28		// gap offset (EPL)
#define PATCH_MEASURE_UNIT	  29		// 0:inch, 1:cm, 2:system default, 3:mm
#define PATCH_CUTTER          30		
#define PATCH_PAUSE_AFTER     31		// CPCL=0:No pause|3:Wait until FEED pressed|2:Wait for label taken sensor|1:Wait for
#define PATCH_PAUSE_AMOUNT    32		// CPCL=1..9999
#define PATCH_USE_FEED_BUTTON		33	// CPCL=0:Ignore|2:Reprint last printed label|1:Feed label
#define PATCH_FEED_BUTTON_AMOUNT	34	// CPCL=0..9999
#define PATCH_PAPER_FEED		35		// CPCL=0:No feed|1:Feed label after writing
#define PATCH_PAPER_FEED_AMOUNT	36		// CPCL=0..127
#define PATCH_DELAY_BEFORE_FEED	37		// CPCL=0:No|1:Yes
#define PATCH_DELAY_AMOUNT		38		// CPCL=0..9999


#define PRTYPE_ZPL			201
#define PRTYPE_EPL			203
#define PRTYPE_COMTEC		231
#define PRTYPE_KIOSK		508

#ifdef __cplusplus
extern "C" {
#endif

/// <summary>
///	<param>char *PrinterName � Name of the printer to use</param>
/// <returns>Pointer to internal data structure, NULL if error.</returns>
/// </summary>
PATCHER_API void * __stdcall Init(char *PrinterName);

/// <summary>
///	<param>void *Data � pointer to internal data from Init.</param>
///	<param>int Setting � ID representing what setting is getting queried for its value.</param>
///	<param>char *Value � Holds the setting value, can be NULL.</param>
///	<param>int BufferSize � required Size for Value buffer, can be NULL.</param>
/// <returns>0 if OK, error otherwise.</returns>
/// </summary>
PATCHER_API int __stdcall GetCurrentValueForSetting(void *Data, int Setting, char *Value, int *BufferSize);

/// <summary>
///	<param>void *Data � pointer to internal data from Init.</param>
///	<param>int Setting � ID representing what setting is getting queried for its values.</param>
///	<param>char *Values � Semicolon separated available values for queries setting, can be NULL.</param>
///	<param>int BufferSize � required Size for Values buffer, can be NULL.</param>
/// <returns>0 if OK, error otherwise.</returns>
/// </summary>
PATCHER_API int __stdcall GetAvailableValuesForSetting(void *Data, int Setting, char *Values, int *BufferSize);
																	   
/// <summary>
///	<param>void *Data � pointer to internal data from Init.</param>
///	<param>int Setting � ID representing what setting is being set.</param>
///	<param>char *Value � New setting value.</param>
/// <returns>0 if OK, error otherwise.</returns>
/// </summary>
PATCHER_API int __stdcall SetValueForSetting(void *Data, int Setting, char *Value);

/// <summary>
///	Cleanup must be called so that handle to the opened printer is closed. 
///	<param>void *Data � pointer to internal data from Init</param>
/// </summary>
PATCHER_API int __stdcall Cleanup (void *Data);

/// <summary>
///	GetDriverVersion returns dll version of the driver.
///	<param>void *Data � pointer to internal data from Init</param>
///	<param>wchar_t *Version � pointer to a buffer that receives Version string</param>
/// </summary>
PATCHER_API int __stdcall GetDriverVersion (void *Data, wchar_t *Version);

/// <summary>
///	<param>char *PrinterName � Name of the printer to use</param>
/// <param>WORD LanguageId � Language to be used for translations(resources).</param>
/// <returns>Pointer to internal data structure, NULL if error.</returns>
/// </summary>
PATCHER_API void * __stdcall InitEx(char *PrinterName, WORD LanguageId);

#ifdef __cplusplus
}
#endif