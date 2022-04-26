#include <unicode/utypes.h>
#include <unicode/ustdio.h>
#include <unicode/ucnv.h>
#include "alwaysSubstitute.h"

int main(int /*argc*/, const char * /*argv*/ []) {
    UErrorCode status=U_ZERO_ERROR;
    
    LocalUConverterPointer cnv(ucnv_open("us-ascii", &status));
    ucnv_setFromUCallBack(cnv.getAlias(), UCNV_FROM_U_CALLBACK_SUBSTITUTE_ALWAYS, NULL, NULL, NULL, &status);
    if(U_FAILURE(status)) {
        u_printf("Error opening: %s\n", u_errorName(status));
        return 1;
    }
    UnicodeString str("black-bird");
    str.setCharAt(5, 0x00AD); // soft hyphen
    const UChar *uch = str.getTerminatedBuffer();
    u_printf("Input String: %S length %d\n", uch, str.length());

    char bytes[1024];
    int32_t bytesWritten =
     ucnv_fromUChars(cnv.getAlias(), bytes, 1024, uch, -1, &status);

    if(U_FAILURE(status)) {
        u_printf("Error converting: %s\n", u_errorName(status));
        return 1;
    }

    u_printf("Converted %d bytes\n", bytesWritten);
    for(int32_t i=0; i<bytesWritten; i++) {
        u_printf("\\x%02X ", bytes[i]&0xFF);
    }
    u_printf("\n");
    // try to print it out on the console
    bytes[bytesWritten]=0; // terminate it first
    puts(bytes);

    return 0; // LocalUConverterPointer will cleanup cnv
}