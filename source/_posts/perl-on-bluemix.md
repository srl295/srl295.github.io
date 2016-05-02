---
title: Perl on Bluemix
tags: bluemix
date: 2016-05-02 16:17:14
---


Quick Start
---

1. Marcus DelGreco at #FluentConf said something about perl support on platforms.
I mentioned Bluemix allowed [bring your own buildpack](https://console.ng.bluemix.net/docs/cfapps/byob.html)

2. Looking through the [buildpack lists](https://docs.pivotal.io/pivotalcf/buildpacks/) didn't turn up Perl *per se* but…

3. … enter [sourcey-buildpack](https://github.com/oetiker/sourcey-buildpack). 
It's a generic buildpack!
 From its README I knew I was in the right spot:
>Isn't it simply amazing to see these demos, where they throw a 
>bunch of php, ruby, Java or python code at a Cloud Foundry site
> and it gets magically turned into a running web applications. 
>Alas for me, life is often a wee bit more complicated than that.
> My projects always seem to required a few extra libraries or they
> are even written in an dead scripting language like Perl.

4. And now for that `tl;dr`-inspiring moment:

Let's see if the canned sample works. Hint: yes.

First, `cf login` into Bluemix, and then:

    $ git clone https://github.com/oetiker/sourcey-buildpack
    $ cd sourcey-buildpack/example
    $ cf push MYAPPLICATION$$ -m 128M -b https://github.com/oetiker/sourcey-buildpack

The above builds perl (takes a while the *first* time) and deploys a 
little app that just dumps the deserialized JSON out.

Improving
---

But wait! It could be even simpler.
So, I opened PR
[oetiker/sourcey-buildpack#2](https://github.com/oetiker/sourcey-buildpack/pull/2)
which adds a manifest file to the example. Then, only `cf push` is needed,
the `-b …` option is now unnecessary.


