#include <windows.h>
#include "AmtFwLoader.h"

/* The two macros below are used as error return codes */
/* in case the DLL does not load, or is missing one or */
/* more functions, respectively.  You must define them */
/* to whatever values are meaningful for your DLL.     */
#define kFailedToLoadDLLError     -1
#define kCouldNotFindFunction     -1

static HINSTANCE DLLHandle;

/* Declare the variables that hold the addresses of the function   */
/* pointers.                                                       */
static int (*amt_load_firmware_Ptr)(char *ftdi_serial_number,
                                    char *file_name);


/* Load the DLL and get the addresses of the functions */
static int LoadDLLIfNeeded(void)
{
    if (DLLHandle)
        return 0;

    DLLHandle = LoadLibrary("AmtFwLoader.dll");
    if (DLLHandle == NULL) {
        return kFailedToLoadDLLError;
        }

    if (!(amt_load_firmware_Ptr = (void*) GetProcAddress(DLLHandle, 
         "amt_load_firmware")))
        goto FunctionNotFoundError;

    return 0;

FunctionNotFoundError:
    FreeLibrary(DLLHandle);
    DLLHandle = 0;
    return kCouldNotFindFunction;
}


/* Glue Code for each of the DLL functions */



int amt_load_firmware(char *ftdi_serial_number, char *file_name)
{
    int dllLoadError;

	dllLoadError = LoadDLLIfNeeded();
	
    if (dllLoadError)
        return dllLoadError;
	
    return (*amt_load_firmware_Ptr)(ftdi_serial_number, file_name);
}

