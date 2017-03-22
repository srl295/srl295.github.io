---
title: Translating Kibana with the Globalization Pipeline
s: translating-kibana
date: 2017-03-17 10:25:41
tags:
- globalizationpipeline
- kibana
---

{% youtube fI9iuXWYHfI %}

# Introduction

{% asset_img kibana0.png %}

This post (and video) will explain how to translate Kibana using the Globalization Pipeline service on Bluemix. Note that some of the steps shown here depend on [kibana:8766](https://github.com/elastic/kibana/pull/8766) which was not merged as this article went to press. (Portions are based on the `development-internationalization.asciidoc` document from that PR.)

# Prerequisites

* Kibana — for now, I am using the [i18n_phase2](https://github.com/hickeyma/kibana/tree/i18n_phase2) branch from [kibana:8766](https://github.com/elastic/kibana/pull/8766). - [91f27f69a03eb74f4a84d2f628b8f5584b9d2a70](https://github.com/elastic/kibana/commit/91f27f69a03eb74f4a84d2f628b8f5584b9d2a70) to be precise.  See Kibana’s READMEs for detailed setup instruction.

* A [Bluemix](https://bluemix.net) account to access Globalization Pipeline. It's free to sign up!

* Java and the latest [gp-cli.jar](https://github.com/IBM-Bluemix/gp-java-tools/releases) (Globalization Pipeline tools).

# Setting up Globalization Pipeline

* Follow the GP [Quick Start Guide](https://github.com/IBM-Bluemix/gp-common#quick-start-guide) to create a service instance. Copy down the "credentials" into a new file, `gp-credentials.json` which should look something like the following:

```json
{
 "url": "https://gp-rest.bluemix.example.com/translate/rest",
 "userId": "c2lnbiB1cCBAIGJsdWVtaXgubmV0ISEK",
 "password": "aHVudGVyNDIK",
 "instanceId": "aHR0cHM6Ly9ibHVlbWl4Lm5ldCA8PDwK"
}
```

* Create the bundle on the GP instance. The example below uses English (`en`) as the source langage and requests Spanish, Japanese, and French targets (`es,ja,fr`).

```bash
$ java -jar {wherever}/gp-cli.jar create-bundle  -j {wherever}/gp-credentials.json  -b 'kibana_core'  -l en,es,ja,fr
A new bundle 'kibana_core' was successfully created.
```

* The bundle will show up in the Bluemix dashboard under the service’s console, but as empty.

* We are going to translate the `src/core_plugins/kibana/translations/en.json` file in Kibana. Upload that file to the Globalization Pipeline service using the command line:

```bash
$ cd ~/src/kibana
$ java -jar {wherever}/gp-cli.jar import  -j {wherever}/gp-credentials.json  -b 'kibana_core' -l en -f src/core_plugins/kibana/translations/en.json -t json
Resource data extracted from src/core_plugins/kibana/translations/en.json was successfully imported to bundle:kibana_core, language:en
```

* If you head back over to the Bluemix dashboard, you can now see the populated bundle with translated content:

{% asset_img gp-dash.png %}

What you see was done with machine translation, hence the red “U” (Unreviewed). The content here can be corrected manually by clicking the Pencil icon, or marked as manually reviewed by clicking the Checkmark. It’s also possible to download the translated content for offline review or use, or to upload a corrected version of one of the translations.

Head back over to the command line, though, because it is time to create our plugin.

# Creating the plugin

* See https://github.com/elastic/generator-kibana-plugin - create a `kibana-YOURNAME-translation-plugin` next to your `kibana` directory.

Something like this:

```bash
$ npm install -g yo generator-kibana-plugin
…
Everything looks all right!

$ yo kibana-plugin
? Your Plugin Name gp srl kibana plugin
? Short Description An awesome Kibana translation plugin
? Target Kibana Version master

I'm all done. Running npm install for you to install the required dependencies. If this fails, try running the command yourself.
```

* You will notice that the generator has created a `translations/es.json` file. We will replace this with our translated content.

```
$ rm translations/es.json
```

* Now, download the translated content into the correct files:

```bash
$ java -jar {wherever}/gp-cli.jar export  -j {wherever}/gp-credentials.json -b 'kibana_core' -t json -l es -f translations/es.json
Resource data exported from bundle:kibana_core, language: es was successfully saved to file translations/es.json

$ java -jar {wherever}/gp-cli.jar export  -j {wherever}/gp-credentials.json -b 'kibana_core' -t json -l fr -f translations/fr.json
Resource data exported from bundle:kibana_core, language: fr was successfully saved to file translations/fr.json

$ java -jar {wherever}/gp-cli.jar export  -j {wherever}/gp-credentials.json -b 'kibana_core' -t json -l ja -f translations/ja.json
Resource data exported from bundle:kibana_core, language: ja was successfully saved to file translations/ja.json
```

* Update the `index.js` file in the plugin to mention the updated `translations` files.

You will see a section like this:

```js
 translations: [
  resolve(__dirname, './translations/es.json')
 ],
```

Change it to mention all of the language files we have just downloaded:

```js
 translations: [
  resolve(__dirname, './translations/es.json'),
  resolve(__dirname, './translations/ja.json'),
  resolve(__dirname, './translations/fr.json')
 ],
```

* _That's all the coding we'll need for today…_

* Copy your entire translations plugin directory to the Kibana plugins (`<kibana_root>/plugins/`) directory

# Trying it out

Fire up Kibana and you should see the translated content!

{% asset_img kibana1.png %}

# More steps

* By the way, French isn’t included in the video or images becuase I ran into [kibana#10580](https://github.com/elastic/kibana/issues/10580) during the production of this video. When this is fixed I will come back and edit this video, but until then, beware single quotes (`'`) in your translated strings.

* Note that if you repeat the `import` and `export` steps of the `gp-cli` tool, the Globalization Pipeline will automatically manage translation changes if, for example, translated keys are added or removed, or translated content changes.

* Follow the progress of Kibana Globalization on Github: [(kibana#6515)](https://github.com/elastic/kibana/issues/6515).

* Read more about [Globalization Pipeline](https://console.ng.bluemix.net/docs/#services/GlobalizationPipeline/index.html)

* Connect with the [Globalization Pipeline Open Source Community](https://developer.ibm.com/open/openprojects/ibm-bluemix-globalization-pipeline/)


# Acknowledgements

* Thanks to fellow IBMers Martin Hickey, Shikha Srivastava, and Jonathan Lo for the Kibana G11n work [(kibana#6515)](https://github.com/elastic/kibana/issues/6515), also the [elastic/kibana](https://github.com/elastic/kibana/) team for being a great OSS community, and last but not least the entire Globalization Pipeline team.