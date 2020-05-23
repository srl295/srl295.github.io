#include <unicode/utypes.h>
#include <unicode/ustdio.h>
#include <unicode/ucnv.h>
#include "alwaysFFFD.h"

int main(int /*argc*/, const char * /*argv*/ []) {
    UErrorCode status=U_ZERO_ERROR;

    LocalUConverterPointer cnv(ucnv_open("shift-jis", &status));
    ucnv_setToUCallBack(cnv.getAlias(), UCNV_TO_U_CALLBACK_SUBSTITUTE_FFFD, NULL, NULL, NULL, &status);
    if(U_FAILURE(status)) {
        u_printf("Error opening: %s\n", u_errorName(status));
        return 1;
    }
    #define NRBYTES 6
    const uint8_t bytes[NRBYTES] = { 0x61, 0x80, 0x94, 0x4c, 0xea, 0xe5 };

    u_printf("Input Bytes: length %d\n", NRBYTES);

    #define NRUCHARS 50
    UChar uchars[NRUCHARS];

    int32_t ucharsRead =
     ucnv_toUChars(cnv.getAlias(), uchars, NRUCHARS, (const char*)bytes, NRBYTES, &status);

    if(U_FAILURE(status)) {
        u_printf("Error converting: %s\n", u_errorName(status));
        return 1;
    }

    u_printf("Converted %d uchars\n", ucharsRead);
    for(int32_t i=0; i<ucharsRead; i++) {
        u_printf("U+%04X ", uchars[i]);
    }
    u_printf("\n");
    // try to print it out on the console
    u_printf("Or string: '%S'\n", uchars);

    return 0; // LocalUConverterPointer will cleanup cnv
}