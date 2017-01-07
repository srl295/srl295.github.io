---
title: GP Client for JavaScript v1.3.0 released
s: g11n-pipeline-1.3.0
date: 2016-12-07 16:43:08
tags: globalizationpipeline
---

It’s time for a refresh on the Globalization Pipeline Node.js client.
I’ve just released [v1.3.0](https://github.com/IBM-Bluemix/gp-js-client/releases/tag/v1.3.0) of this SDK.
You can update your package.json the usual way, with `npm install --save g11n-pipeline`

[![npm version](https://badge.fury.io/js/g11n-pipeline.svg)](https://badge.fury.io/js/g11n-pipeline)

I managed to close about [13 issues](https://github.com/IBM-Bluemix/gp-js-client/milestone/7?closed=1) since v1.2.x 

Quality
--

I was able to increase _function_ coverage to 100% thanks to the [VSCode coverage plugin](https://marketplace.visualstudio.com/items?itemName=bradleymeck.codecover),
and increase line coverage to 91%. Of course, when you test, you find bugs. Bugs such as
realizing that `updateResourceStrings()` was unusable because there was
[no way to pass the languageId parameter](https://github.com/IBM-Bluemix/gp-js-client/pull/49/files#diff-50cfa59973c04321b5da0c6da0fdf4feR757).

Features
--

First of all, I [synchronized the client](https://github.com/IBM-Bluemix/gp-js-client/pull/46) with the latest current REST API. So take a peek at the [docs](https://github.com/IBM-Bluemix/gp-js-client#api-reference) again
and see if there are any new features or fields.

I also tried to add some convenience functions. For example, getting the full
list of language IDs supported used to require concatenating the source and target
lists. Now, with [#40](https://github.com/IBM-Bluemix/gp-js-client/issue/40)
you can call `.languages()` on the `Bundle` object and it will build this
list for you. There is also a `bundle.entries()` accessor as of [#14](https://github.com/IBM-Bluemix/gp-js-client/issues/14)
which returns `ResourceEntry` objects.

Speaking of convenience, most places where you used to call `.someFunction({}, function callback(…){});`
the `{}` are optional. If it worked with `{}` before, it's now optional.

The sample PR where I [updated the sample code](https://github.com/IBM-Bluemix/gp-nodejs-sample/pull/10/files)
shows some of the code improvements.

There are more features to add here, but I hope you like the changes in v1.3.0!
