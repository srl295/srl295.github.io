---
title: Full Stack Language Enablement
s: full-stack-enablement
date: 2017-06-06 09:04:49
tags:
- g11n
- locales
---


_This has been a working document for a while. I am publishing it here so that it can serve for more public discussion. Thank you to contributors:  Anshuman Pandey, Martin Raymond, Isabelle Zaugg, and others who have discussed these items over the years._

{% asset_img stackbooks.jpg "a “full stack” of resources" %}

**Introduction**

There are a lot of steps to be taken in order to ensure that a language is fully supported. The objective of this document is to collect the steps needed and begin to plan how to accomplish them for particular languages. The intent is for this to serve as a guide to language community members and other interested parties in how to improve the support for a particular language.

The diagram below shows languages in one axis, and the “stack” of support tasks on the other. 

{% asset_img support.png "Language vs the “Support Stack”" %}

Coordination is key. Finding and communicating with the right people is often at least as difficult as the technical aspects.  [ScriptSource][scriptsource] can be a good “central hub” to collect/publish information and needs for a user community.

**Encoding**

A critical step is of course [Unicode](http://unicode.org) encoding, but that is only the first step. Also, there can be (through no fault of anyone’s) a long gap between the first contact with a user community and the publication of a Unicode version supporting that language, not to mention other steps. 

In the course of the encoding process, a lot of information is gathered which is relevant to other steps, such as, grammatical considerations, best practices around font and layout support, etc.

- Standardizing of the script (ideally/typically this happens before Unicode inclusion, but sometimes this can hold up Unicode inclusion, or be an ongoing challenge if it is incomplete after Unicode inclusion.  (perhaps this should be in its own category before “Encoding.”  Standardization of the script, as well as the orthography, are very helpful for digital life in general, as a standardized orthography helps “search” to work well, for example.  


**Font**

Martin Raymond recommends to: _“…split drawing [of the] glyphs from making the fonts work. Anyone can draw the glyphs, realize that font designers will work later to connect attachment points etc. later.”_

In other words, the critical _initial_ step is to get the correct glyphs from the user community.

Note that there are fonts for different purposes:  aesthetic, low resolution, small devices, etc.

**Layout**

Determine if layout requirements are “complex” or not. (See the “shaping required” field of [CLDR Script Metadata](http://unicode.org/repos/cldr/trunk/common/properties/scriptMetadata.txt)).

Support through W3C’s Layout & Typography project:  [https://www.w3.org/International/layout][w3clayout]
  - From website:  “The W3C needs to make sure that the text layout and typographic needs of scripts and languages around the world are built in to technologies such as HTML, CSS, SVG, etc. so that Web pages and eBooks can look and behave as people expect around the world.”

**OS-level support**
- Desktop support
- Mobile support (possibly even more important than desktop for global minority scripts)

**Input**
* Keyboard
  - Virtual keyboards for mobile devices
  - managing repertoire (Unihan, etc)
  - Transliteration standard into Latin   

**Locale Data**

[CLDR][cldr] seed/initial data (see [CLDR Minimal Data][cldrminimaldata])

* Needed: an app to collect initial data (a true “Survey Tool”)
* Within CLDR: Promote from “seed” to “common” as data matures
* Verify deployment (inclusion in JSON data, ICU, Globalize, etc.) 
 - Code changes may be needed, such as calendar and new date/time support, line breaking, etc.

**Software Translation**

- Various parts of [Wikimedia](https://www.wikimedia.org) community.
- [http://translatewiki.net](http://translatewiki.net), others - translate open source software.
- Community translation of sites (Various OSS/commercial items which support crowdsourced data…)
- Commercial translation environments such as [Globalization Pipeline](https://developer.ibm.com/open/openprojects/ibm-bluemix-globalization-pipeline/)

**Advanced NLP**

(Might be worth mentioning that the development of much NLP applications requires large digital corpora, the collection of which is a project in itself.  Even when corpora are collected, say through web crawling, when they are not available publicly, other developers cannot benefit from them as a resource.)

- OCR… 
- Spell checking
- Auto-correction, Auto-suggestion, Auto-fill
- Parsing & Stemming (helps search to happen with related terms)
- Language glossaries/dictionaries/thesauri
- Search capacity within word documents & pdfs
- Translation:  Ideally not just dominant language to minority language, but also minority to minority language (for maximum use within countries that enjoy a high level of language diversity)
- Natural language queries and conversation

**App-Level Support**

This means going beyond:
- Multilingual readiness (Unicode support: “Don’t garble my text”)
- Leverage locale data and implementations (ICU, etc.)
- Translation (above)

…to truly supporting language specific features. Some examples:
- Arabic and East Asian advanced typography
- NLP support as above

**ICANN / IDN support**

Support for a language within top-level domains will allow an important level of localization online that breaks from the historically Latin-only top level domains and reflects the truly international nature of the Internet.

**Computer programming language in mother tongue**

While this may seem a far-fetched dream today, the fact that programming languages are in English is a barrier to the full use of digital tools by much of the world’s population.  This might be the final frontier for the internationalization/localization of digital technologies. “[قلب][qalb]” is an example of a programming language entirely Arabic.

**References**

- [CLDR][cldr] and [CLDR Minimal Data for New Locales][cldrminimaldata]
- [ScriptSource][scriptsource]
- [W3C International and Typography][w3clayout]

[cldr]: http://cldr.unicode.org
[cldrminimaldata]: http://cldr.unicode.org/index/cldr-spec/minimaldata
[scriptsource]: http://scriptsource.org
[w3clayout]: https://www.w3.org/International/layout
[qalb]: https://en.wikipedia.org/wiki/Qalb_(programming_language)
