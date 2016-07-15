---
title: Translating ICU4C with Globalization Pipeline
tags: 
    - globalizationpipeline
    - icu
    - wip
date: 2016-07-14 16:57:01
---


Disclaimer
===

This is a work in progress. If you read to the end, you’ll see we
*almost* reached our goal here. 

Background
===

I work on [ICU4C](http://icu-project.org) (the premier C/C++ library for Unicode support).
And I work on [Globalization Pipeline](https://developer.ibm.com/open/ibm-bluemix-globalization-pipeline-service/).
These two haven’t really crossed paths… until now.

What we’ll do
===

This blog will cover how to use the Globalization Pipeline to translate
`uconv`, one of ICU’s sample command line apps. We'll be translating the resource
files you can see in [`source/extra/uconv/resources`](http://bugs.icu-project.org/trac/browser/icu/trunk/source/extra/uconv/resources)

Gathering the tools
===

* First, [Download ICU4C](http://site.icu-project.org/download) source code (as a tarball or from the SVN repository) and compile it. See its readme for more details.

* Now, set up Globalization Pipeline. See our [Quick Start Guide](https://github.com/IBM-Bluemix/gp-common#quick-start-guide)
for getting your Globalization Pipeline instance created and set up. 

  * In the GP dashboard, create a bundle named `uconv`. Select which languages you want to translate into, but don’t upload any strings.  Click Save.

  * Also from the Bluemix dashboard, get the service credentials for your service.  Save these in a file called `mycreds.json` that should look like the
  example in [this document](https://github.com/IBM-Bluemix/gp-java-tools/blob/master/gp-cli.md#command-reference).

* We’ll also need the `gp-cli` java tool, so download the latest jar from [gp-java-tools](https://github.com/IBM-Bluemix/gp-java-tools/releases).

Into the Pipeline
===

Now, let's get some translated content 

* `cd ` [`source/extra/uconv/resources`](http://bugs.icu-project.org/trac/browser/icu/trunk/source/extra/uconv/resources/)

Hm. These files are in icu4c resource format, which isn't (yet?) supported by Globalization Pipeline… directly. Let's try an interim step.

* `genrb -x root root.txt`
* `genrb -x fr fr.txt`

Now we have `root.xlf` and `fr.xlf` (for good measure).

Here's a snippet of `root.xlf`:

```
            <group id = "root" restype = "x-icu-table">
                <trans-unit id = "U_BUFFER_OVERFLOW_ERROR" resname = "U_BUFFER_OVERFLOW_ERROR">
                    <source>Buffer overflow</source>
                </trans-unit>
                …
```

OK. The `gp-cli` tool says it handles XLIFF as a file format. Let's get that set up.

* `java -jar gp-cli-1.1.0.jar import -b uconv -f root.xlf -l en -t xliff -j mycreds.json`

Note that we use the language tag `en` for English here, while the file was originally entitled `root`.
This is because Globalization Pipeline works with the explicit source language, whereas for ICU, `root`
is what will be consulted as a fallback if no other languages are available.

It says it uploaded… but let’s check in the Globalization Pipeline dashboard:

{% asset_img uploadeng.png English content uploaded %}

OK! That’s great.  Browsing over to the other 
language translations, we can see that the MT engines are hard
at work. However, we happen to already have some French translations
in the ICU source base. We'll upload this, to overwrite some of the Machine-translated
entries for French:

* `java -jar gp-cli-1.1.0.jar import -b uconv -f fr.xlf -l fr -t xliff -j mycreds.json`

Great. Now we have some human translated content as well. We can
now correct, upload/download content in the dashboard until we are
happy with the translations there.

Out of the Pipeline
===

OK, now for the next step- getting those translations back into ICU4C.

We can list the bundle status from the command line:

* `java -jar gp-cli-1.1.0.jar show-bundle -b uconv -j mycreds.json`

```
{
  "sourceLanguage": "en",
  "targetLanguages": [
    "de",
    "es",
    "fr",
    "it",
    "ja",
    "ko",
    "pt-BR",
    "zh-Hans",
    "zh-Hant"
  ],
  "readOnly": false,
  "updatedBy": "…srloomis@us.ibm.com",
  "updatedAt": "2016-07-14T15:22:40.233-07"
}
```

Now, we’ll download the files in XLIFF format again:

* `java -jar gp-cli-1.1.0.jar export -b uconv -f fr.xlf -l fr -t xliff -j mycreds.json`
* `java -jar gp-cli-1.1.0.jar export -b uconv -f es.xlf -l es -t xliff -j mycreds.json`
* `java -jar gp-cli-1.1.0.jar export -b uconv -f de.xlf -l de -t xliff -j mycreds.json`
* `java -jar gp-cli-1.1.0.jar export -b uconv -f zh.xlf -l zh-Hans -t xliff -j mycreds.json`

… and so on. Repeat for each language you wish to download. Note that we’ve used `zh` for Chinese instead of `zh-Hans`.

OK, we have XLIFF format. How to convert it to ICU format? `genrb` only writes XLIFF, it can’t read it.

And back again… almost.
===

We need the XLIFF2ICU Converter as is noted [here](http://userguide.icu-project.org/locale/resources#TOC-Using-XLIFF-for-Localization).

To build it, at present, this worked for me:

* download [ICU4J](http://site.icu-project.org/download) source (yes, J)
* run `ant xliff` from the top level
* you will end up with an `out/xliff/lib/xliff.jar` 

Still with me? Head back to the `uconv/resources` directory, and now run:

* `java -jar xliff.jar  -s . -d . fr.xlf`

And that brings us to… 
```
Processing file: ./fr.xlf
The XLIFF document is invalid, please check it first: 
Line 3, Column 81
Error: cvc-elt.1: No se ha encontrado la declaración del elemento 'xliff'.
```

Hrm.  Seems like the XLIFF output isn't quite ready to be consumed.
I [filed a bug](https://github.com/IBM-Bluemix/gp-java-tools/issues/5) on this,
of course.

Plan B 
===

We're so close… let's see what we can do.
What if we fetch the data in JSON format, and then hack up something
to convert it to ICU format? It might suffice for this blog post.

Let's run the fetches again, but get JSON this time:
* `java -jar gp-cli-1.1.0.jar export -b uconv -f fr.json -l fr -t json -j mycreds.json`
…

Now, run the following [Node.js](https://nodejs.org) script over the JSON files:
* `node js2icu.js fr.json es.json …`


```js
// js2icu.js
const fs=require('fs');
const args = process.argv.slice(2);
for (var i in args) {
    const f = args[i];
    console.log('# read ' + f);
    const loc = f.split('.')[0];
    const json = JSON.parse(fs.readFileSync(f));
    var s = '\ufeff// -*- Coding: utf-8; -*-\n//auto converted\n' + loc + '\n{\n';
    for (var k in json) {
        s = s + '   ' + k + ' { "'  + json[k].replace(/"/g,'\\"') + '" }\n';
    }
    s = s + '}\n';
    console.log('# wrote ' + loc + '.txt');
    fs.writeFileSync(loc+'.txt', s);
}
```

You should be the proud owner of `.txt` files matching all of the languages you are using.

We're almost there. 
Let's go up and build `uconv`:

* `cd ..` 

Now edit `resfiles.mk` and change the `RESSRC` line to reference the new translations:

```
RESSRC = $(RESOURCESDIR)$(FILESEPCHAR)root.txt $(RESOURCESDIR)$(FILESEPCHAR)fr.txt $(RESOURCESDIR)$(FILESEPCHAR)es.txt $(RESOURCESDIR)$(FILESEPCHAR)zh.txt
```

Build `uconv`…

* `make`

Done
===

Let’s test it. I know `uwmsg.o` isn't really utf-8, that's why this is a test.

* `env LC_ALL=es ./../../bin/uconv -f utf-8 < uwmsg.o`

```
La conversión a Unicode de página de códigos falló en posición de byte de entrada de 0. Bytes: Error de cf: El carácter ilegal encontró La conversión a Unicode de página de códigos falló en posición de byte de entrada de 1. ……
```

Looks like we have a (more) translated `uconv` now.
Some of the messages don’t quite work correctly due to ICU4C message conventions.
Perhaps we will investigate this in the future.

