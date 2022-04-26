// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html

#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include <unicode/ucnv_cb.h>

static const UChar kFFFD[] = { 0xFFFD };

/**
 * This is a modified version of ICU’s UCNV_TO_U_CALLBACK_SUBSTITUTE
 * it unconditionally substitutes U+FFFD.
 *
 * Usage:
 *   ucnv_setToUCallBack(c, UCNV_TO_U_CALLBACK_SUBSTITUTE_FFFD, NULL, NULL, NULL, &status);
 */
U_CAPI void    U_EXPORT2
UCNV_TO_U_CALLBACK_SUBSTITUTE_FFFD (
                 const void *context,
                 UConverterToUnicodeArgs *toArgs,
                 const char* codeUnits,
                 int32_t length,
                 UConverterCallbackReason reason,
                 UErrorCode * err)
{
    (void)codeUnits;
    (void)length;
    if (reason <= UCNV_IRREGULAR)
    {
		*err = U_ZERO_ERROR;
		ucnv_cbToUWriteUChars(toArgs, kFFFD, 1, NULL, err);
		// see ucnv_cbToUWriteSub()
    }
    /* else ignore the reset, close and clone calls. */
}

