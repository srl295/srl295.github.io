---
title: "Announcing \uD83C\uDF32 pino-couch"
tags:
  - node
  - pino
  - opensource
s: pino-couch
date: 2017-06-02 14:55:40
---

[![latest on npm](https://badge.fury.io/js/pino-couch.svg)](https://www.npmjs.com/package/pino-couch)

Today I’m pleased to release [pino-couch](https://www.npmjs.com/package/pino-couch). You can find it on GitHub under [https://github.com/IBM/pino-couch](https://github.com/IBM/pino-couch).

This little module is a [transport](https://github.com/pinojs/pino/blob/master/docs/transports.md#transports) which lets you capture your [pino](https://github.com/pinojs/pino) logs into any [CouchDB](https://couchdb.apache.org) database.

## Why pino?

* Speed: I haven’t independently tested the [benchmarks](https://github.com/pinojs/pino#benchmarks), but I really like logging that doesn’t slow down the application. I want to be able to sprinkle logging generously in the application without slowing it down.

* Simplicity: Take a look at the example below. We go from logging to the console, to logging in a database. The configuration and execution of log processing is entirely _outside_ of the application.

* Sticker: Because it has a logo that looks nice on a hex sticker? OK, not really. But [@matteocollina](https://twitter.com/matteocollina) presented this logger so effectively at NodeSummit, I asked for a sticker. Today, I’m glad to give something back to the community.

## Taking it for a spin

### First steps with pino

Let’s do a quick demo here, with a simple app that emits some logs:

```
$ cd somewhere
$ npm init -q -y
$ npm install --save pino
```

And for `index.js`:

```js
const pino = require('pino')();
pino.error('Something bad happened!');
pino.warn({ iToldYou: [ 'once', 'twice', 'thrice' ]});
pino.info({ msg: "Hey, check out these versions", versions: require('process').versions });
pino.trace('ALL THE DETAILS');
```

With the nice [pino API](https://github.com/pinojs/pino/blob/master/docs/API.md) you have lots of options for emitting logs.

```js
$ node index.js 
{"pid":54534,"hostname":"filfla.local","level":50,"time":1496436803976,"msg":"Something bad happened!","v":1}
{"pid":54534,"hostname":"filfla.local","level":40,"time":1496436803979,"iToldYou":["once","twice","thrice"],"v":1}
{"pid":54534,"hostname":"filfla.local","level":30,"time":1496436803979,"msg":"Hey, check out these versions","versions":{"http_parser":"2.7.0","node":"8.0.0","v8":"5.8.283.41","uv":"1.11.0","zlib":"1.2.11","ares":"1.10.1-DEV","modules":"57","openssl":"1.0.2k","icu":"59.1","unicode":"9.0","cldr":"31.0.1","tz":"2017b"},"v":1}
```

Notice the `trace()` details were below the current [level](https://github.com/pinojs/pino/blob/master/docs/API.md#level), so were omitted. 
This is detailed, but not super readable. If you are running something from the commadn line, the `pino` global utility tidies up the output nicely—in color, even, if your console supports it.

```
$ npm install -g pino
$ node index.js | pino
[2017-06-02T20:56:12.125Z] ERROR (56035 on filfla.local): Something bad happened!
[2017-06-02T20:56:12.128Z] WARN (56035 on filfla.local): 
    iToldYou: [
      "once",
      "twice",
      "thrice"
    ]
[2017-06-02T20:56:12.128Z] INFO (56035 on filfla.local): Hey, check out these versions
    versions: {
      "http_parser": "2.7.0",
      "node": "8.0.0",
      "v8": "5.8.283.41",
      "uv": "1.11.0",
      "zlib": "1.2.11",
      "ares": "1.10.1-DEV",
      "modules": "57",
      "openssl": "1.0.2k",
      "icu": "59.1",
      "unicode": "9.0",
      "cldr": "31.0.1",
      "tz": "2017b"
    }
```

### Persistence without Perspiration: Relax!

Here’s where `pino-couch` comes in. I’m going to set up a [https://cloudant.com](Cloudant) database
to store these logs (as I do in production), but you can also use a local or any other `couchdb` instance (as I do when developing locally).

* First, create a database

{% asset_img createdb.png %}

* Next, give appropriate permissions.

`pino-couch` only needs to write to the database, it doesn’t need to read. Click the _Permissions_ tab, then _Generate API Key_.  Choose only the `_writer` column for our new API key.

{% asset_img permissdb.png %}

That’s actually it for configuration.

* Start up our app, but using `pino-couch`.   Use the _APIKEY_ and _PASSWORD_ that were generated above. And of course, your own _ACCOUNT_.

```
$ npm install -g pino-couch
$ node index.js | pino-couch -U https://APIKEY:PASSWORD@ACCOUNT.cloudant.com -d slogging | pino
[2017-06-02T21:16:22.511Z] ERROR (68283 on filfla.local): Something bad happened!
( etc… )
```

* The output is about the same.  We chained on a ` | pino` at the end to keep the output human-readable— that's optional.

Let’s take a look at the Cloudant dashboard again:

{% asset_img readdb.png %}

There’s our data!

## So now what?

Here are a couple of things you might do with your new logging pipeline:

### Write a clever design document to mine your app logs for important stuff.

Even something as simple as the following will get you timestamp-ordered documents.
```js
function (doc) {
    emit([new Date(doc.time).toISOString(),doc._id], doc.msg);
}
```

Note that besides the `time` field with epoch time, `hostname` contains the current hostname. This is really useful for distinguishing logs from among a cluster of servers.

### Ingest the data into Elasticsearch/Kibana

We’ve done this with great success. We were already pulling from another Cloudant DB, so it was easy to add the application logs.

And of course:

### Relax

{% asset_img pinohex.jpg "🌲: +1!" %}
