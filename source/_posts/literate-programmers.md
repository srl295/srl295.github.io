---
title: Literate Programmers
s: literate-programmers
date: 2017-02-07 08:45:44
tags: i18n
---

Besides the Globalization Pipeline mug, one of my favorite coffee mugs says:

{% asset_img IMG_0010.jpg 03: MAKE IT POSSIBLE FOR PROGRAMMERS TO WRITE IN ENGLISH AND YOU WILL FIND OUT THAT PROGRAMMERS CANNOT WRITE IN ENGLISH. %}

On the serious side, we need to emphasize communication skills in the technology industry. Even if I have a great idea, if I can’t communicate it, it will go nowhere. And neither will I.

Just to be clear, by “communication” I mean “talking with other humans”. Which brings me to today’s topic on the lighter side, and that is the overloading of English.
Words such as _function_, _overload_, _network_, _build_ all have specific meanings that weren’t originally found in [Webster’s](https://en.wikipedia.org/wiki/Webster's_Dictionary#First_edition_1828).
The 1828 definition of _computer_, for example, is:

> _One who computes; a reckoner; a calculator._

In i18n, there are other words that have very specific meanings: _global_, _globalization_, _collation_, _contraction_, and of course _locale_, just to name a few. 

To that end, I have started to add some tongue-in-cheek “redefinitions” to the [bottom of the blog](#rdfooter) just to remind us all that these words have non-software meanings.

If you want to see them all without hitting reload an infinite number of times, you can see the original source [here](https://github.com/srl295/srl295.github.io/blob/hexo/themes/landscape/source/js/retrodefinitions.js).

Speaking of i18n, this overloading doesn’t apply to English only. Most of my devices are set to `es-US` as their locale, so I see a lot of translated error message.  `gcc` for example has a thriving [translation project](http://translationproject.org/domain/gcc.html) where dedicated persons cause “English” to be translated into, for example, “Spanish” such as:


> `#~ msgid "function ‘%D’ declared overloaded, but no definitions appear with which to resolve it?!?"`

> `#~ msgstr "¿!¿se declaró la función ‘%D’ sobrecargada, pero no aparece ninguna definición con la cual resolverlo?!?"`

Not sure why that’s `¿!¿` where I might expect `¿¡¿` — perhaps the initial `!` just shows the compiler’s incredulity. In any event, _sobrecargada_ seems to be a great cognate for _overloaded_. And with that, I will let you goto whatever you were doing before you started reading.

PR’s are welcome on my little list, or leave comments below. What are your favorite examples of overloaded terms, in any language?