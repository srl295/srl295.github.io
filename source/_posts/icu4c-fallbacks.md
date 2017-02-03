---
title: Fallbacks in ICU4C Converters
s: icu4c-fallbacks
date: 2017-02-02 16:05:26
tags: icu4c
---

Unicode’s [ICU](http://icu-project.org) version 59 is well underway at this point. While ideally everything would use Unicode, there still remains many systems — and much content — that is in non-Unicode encodings. For this reason, ICU, in both the C/C++ and the Java flavors, has rich support for codepage conversion.

One of many great features in ICU is the [callback support](http://userguide.icu-project.org/conversion/converters#TOC-Error-Callbacks). A lot can go wrong during codepage conversion, but in ICU, you can control what happens during exceptional situations.

Let’s try a simple sample. By the way, see the end of this post for hints on compiling the samples.

# Substitute, Always

Our task is to convert `black-bird` (but with a `U+00AD`, “Soft Hyphen” in between the two words) to ASCII.

{% include_code lang:cpp substituteTest-0.cpp  %} 

Output:

```
Input String: black­bird length 10
Converted 9 bytes
\x62 \x6C \x61 \x63 \x6B \x62 \x69 \x72 \x64 
blackbird
```

Hm. Ten characters in, nine out. What happened?  Well, `U+00AD` is not a part of ASCII. ASCII is a seven bit encoding, thus only maps code points `\x00` through `\x7F` inclusively. Furthermore, `U+00AD` is [Default Ignorable](http://www.unicode.org/glossary/#default_ignorable), and as of ICU 54.1 (2014) in [#10551](http://bugs.icu-project.org/trac/ticket/10551), the soft hyphen can just be dropped.

But what if, for some reason, you don’t want the soft hyphen dropped? The pre ICU 54.1 behavior can be brought back easily with a custom call back.  So, roll up your collective sleeves, and:

{% include_code lang:cpp alwaysSubstitute.h  %} 

If we `#include` this little header, and set it on the converter before we convert…

```cpp
    LocalUConverterPointer cnv(ucnv_open("us-ascii", &status));
    ucnv_setFromUCallBack(cnv.getAlias(), UCNV_FROM_U_CALLBACK_SUBSTITUTE_ALWAYS, NULL, NULL, NULL, &status);
    …
```

… we get the following result:

```
Input String: black­bird length 10
Converted 10 bytes
\x62 \x6C \x61 \x63 \x6B \x1A \x62 \x69 \x72 \x64 
black?bird
```

Great! Now, we are getting `\x1A` (ASCII SUB). It works.

# When missing goes missing

A related question to the above has to do with converting from codepage to Unicode. That’s a better direction anyway. Convert to Unicode and stay there! One can hope. In any event…

For this task, we will convert `0x61, 0x80, 0x94, 0x4c, 0xea, 0xe5` from Shift-JIS to Unicode.

{% include_code lang:cpp substituteTest-2.cpp  %} 

Output:

```
Input Bytes: length 6
Converted 4 uchars
U+0061 U+001A U+732B U+FFFD 
Or string: 'a猫�'
```

So, the letter "a" byte `\x61` turned into `U+0061`, and then we have an illegal byte `\x80` which turned into `U+001A`. Next, the valid sequence `\x94 \x4c` turns into U+732B which is 猫 (“cat”). Finally, the unmapped sequence `\xea \xe5` turns into `U+FFFD`.  Notice that the single byte illegal sequence turned into (SUB, U+001A), but the two byte sequence turned into `U+FFFD`. This is discussed somewhat [here](http://unicode.org/reports/tr22/#Dual_Substitution_Handling).

So far so good?

But what if you actually want U+FFFD as the substitution character for both sequences? This would be unexpected, but perhaps you have code that is particularly looking for `U+FFFD`s.  We can write a similar callback:

{% include_code lang:cpp alwaysFFFD.h  %} 

Let’s hook it up, as before:

```cpp
    LocalUConverterPointer cnv(ucnv_open("shift-jis", &status));
    ucnv_setToUCallBack(cnv.getAlias(), UCNV_TO_U_CALLBACK_SUBSTITUTE_FFFD, NULL, NULL, NULL, &status);
    …
```

And drumroll please…

```
Input Bytes: length 6
Converted 4 uchars
U+0061 U+FFFD U+732B U+FFFD 
Or string: 'a�猫�'
```

Garbage out never looked so good… 

-----

# Building (or, nothing-up-my-sleeve)

To build these little snippets, I recommend the shell script [icurun](http://source.icu-project.org/repos/icu/trunk/tools/scripts/icurun)

If ICU is already installed in your appropriate paths, (visible to `pkg-config` or at least `icu-config`), you can simply run:

```shell
icurun some-great-app.cpp
```

… and icurun will compile and run a one-off.

If, however, you’ve built ICU yourself in some directory, you can instead use:

```shell
icurun -i path/to/your/icu some-great-app.cpp
```

… where `path/to/you/icu` is the full path to an ICU build or install directory.

If you are on windows… well, there isn’t a powershell version yet. Contributions welcome!

