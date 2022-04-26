// © 2016 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html

#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include <unicode/ucnv_cb.h>

/**
 * This is a modified version of ICU’s UCNV_FROM_U_CALLBACK_SUBSTITUTE
 * it unconditionally substitutes on irregular codepoints.
 *
 * Usage:
 *   ucnv_setFromUCallBack(c, UCNV_FROM_U_CALLBACK_SUBSTITUTE_ALWAYS, NULL, NULL, NULL, &status);
 */
U_CAPI void    U_EXPORT2
UCNV_FROM_U_CALLBACK_SUBSTITUTE_ALWAYS (
				 const void *context,
				 UConverterFromUnicodeArgs *fromArgs,
				 const UChar* codeUnits,
				 int32_t length,
				 UChar32 codePoint,
				 UConverterCallbackReason reason,
				 UErrorCode * err)
{
    (void)codeUnits;
    (void)length;
    if (reason <= UCNV_IRREGULAR) {
      *err = U_ZERO_ERROR;
	  ucnv_cbFromUWriteSub(fromArgs, 0, err);
      /* else the caller must have set the error code accordingly. */
    }
    /* else ignore the reset, close and clone calls. */
}

