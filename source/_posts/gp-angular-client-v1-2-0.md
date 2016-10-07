---
title: gp-angular-client v1.2.0
date: 2016-10-07 13:41:47
tags: globalizationpipeline
---

I just pushed out version 1.2.0 of our [Angular Client for Globalization Pipeline](https://github.com/IBM-Bluemix/gp-angular-client)
to the usual places. `gp-angular-client` on bower, `angular-g11n-pipeline` on npm. 

[![Bower version](https://badge.fury.io/bo/gp-angular-client.svg)](https://badge.fury.io/bo/gp-angular-client)    
[![npm version](https://badge.fury.io/js/angular-g11n-pipeline.svg)](https://badge.fury.io/js/angular-g11n-pipeline)

Thanks to IBMer [@ckoberlein (GitHub)](https://github.com/ckoberlein) this SDK now supports variable substitution. So you can have a string such as `Hello {{user}}` and translate and substitute this same string, so that for example in Spanish it will be `Bienvenidos {{user}}`.  So, output would be `Hello Steven` or `Bienvenidos Steven` depending on language.

More details on our [README](https://github.com/IBM-Bluemix/gp-angular-client#variable-replacement) and be sure to connect with us over on [developerWorks Open](https://developer.ibm.com/open/ibm-bluemix-globalization-pipeline-service/angular-sdk/)!
