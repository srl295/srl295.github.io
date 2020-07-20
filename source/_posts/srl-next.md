---
title: srl.next
s: srl-next
date: 2020-07-16 00:00:00
tags: 
 - job
 - unicode
 - cldr
---

I’m a 2nd generation IBMer, from back when this meant “I’ve Been Moved.”

<!-- more --> 

Fast forward to 1993. I knew that [Taligent](https://en.wikipedia.org/wiki/Taligent) (also where my father was at that time) was working on a new OS of some kind, but that’s all I knew. ([Pink](https://en.wikipedia.org/wiki/Taligent#Pink_system)). I just wanted to write some games for it<sup>*</sup>, and that needed an NDA. Instead of an NDA, I got a job: an internship, using my bran-new C++ skills.

Trying to do pre-build integration on an amazingly complex subsystem which somehow meshed with a dozen other subsystems was an interesting challenge, to say the least! (There were multi core builds all right—one keyboard and ADB mouse per core.)

I learned a lot about how to, and sometimes how _not_ to, design OO systems, rubbing shoulders (and trading bug reports) with the best in the industry. Learned a lot, launched a lot of Nerf™ product, accidentally burned an OID (sorry)! … and, shipped some good features and even products.  I also received plenty of constructive criticism.

I think my first job title was "Technical Specialist", probably because some form needed a title. Since the Apple tradition of design-your-own-businesscard was followed, I styled this as “TechnoSpecialist.”  Later I graduated to Software Engineer or something, so I wrote “Code Sculptor.” I had in mind the idea of those who brought a certain _craft_ to the field—Wozniak, Hertzfield, Atkinson, and so on.

I also ran into something called Unicode. I discovered it quite by accident.  I still was trying to make some kind of game, but I decided (since I was in the NetComm group) to write a networked chat program. Sounds simple enough.  The base text class was called [TStandardText](http://cortex.p.gen.nz/taligent/Docs/classes/TStandardText.html). To my surprise and annoyance, when I [streamed](http://cortex.p.gen.nz/taligent/Docs/classes/TStandardText.html#operator_OUT__OUT_=) a TStandardText (using `operator>>=` of course!), the other side received a bunch of NULLs (`\0x00`). A null, then a letter, a null, then another letter. And finally, TWO nulls to end it. For example, streaming `ABC` showed up on the wire not as ABC but as

```
\x00 \x41 \x00 \x42 \x00 \x43 \x00 \x00
```

When I read this into a `char*` on the server side, it had `strlen()` of `0`. Huh? I plugged in a network analyzer (Network General Sniffer 10Base2), this being the NetComm lab, and it was confirmed.  What happened to my string?  Answer:  [Unicode](https://www.unicode.org).  UCS-2 BE, to be precise.

I was not impressed.  What was this Unicode® and why was it scrambling my text?? So, I just changed my chat program to ++ over every other null. Problem solved.

However, my attention was focused on networking and communications, protocol abstractions for streams and RPC, Object storage and network discovery (tcp port 6149, `tal-pod`).

I went on to work with email protocols, and then web services. I had run a very early departmental web server, and had done some prototyping.  Around the time (1996-1997-1998) that Taligent was being folded into **IBM** as a wholly owned subsidiary, this turned into WebRunner ServerWorks and [US 6,233,622 B1](http://patft1.uspto.gov/netacgi/nph-Parser?patentnumber=623622).

Also around that time, I took my first trip to [Malta](https://www.gov.mt/en/). Just before I went, I heard there was need to help on a “temporary assignment” to do some language related stuff. But it would be done by the time I got back from Malta, or so it was thought.

The work wasn't done, so I jumped in when i came back: A small team working on Bidi. Arabic and Hebrew text enablement. Somehow I understood a little more about Unicode by that time, and we got actually working worked.

If you haven't ever tried to implement Bidi text selection and rendering, try it. You'll either run away screaming, or… find a new favorite.

I had found my field. This “temporary” assignment (which wasn’t in itself as temporary as it was supposed to be) turned into a change of department at my request—a career change.

1997 also brought Java 1.1, including Taligent-contributed Unicode technology. I wasn't any part of that. (In 1996 I remembering handing out flyers for some “Taligent Analytics for Unicode”,  Networld+interop NYC, but I'm sure I couldn't explain what it was for.)

However, soon after, I was helping out with what was released, in 1999 as the open source [International Components for Unicode](https://icu-project.org), partly based on the Java and C++ work, and available in C++ and Java.

Now, all of this global stuff needs some local data. The data directory of ICU kept getting bigger, and the {% post_link icu-infra bugs %} kept coming in.  The data for ICU (and the JDK, and 100 Linux Locales) had all come from the ICIR data, from IBM Toronto. In 2003 a project was started, first under l18nux/LSB/FSG (what you now know and love as the [Linux Foundation](https://linuxfoundation.org/) called CLDR.

It's an oversimplification, but from a pragmatic perspective, we took the ICU data subdirectory as a _seed_, split it off, put it in XML, and put a lot of process and tooling so that it could be easily compared with what all the other non-CLDR platforms were doing. And the comparisons showed that localization was all over the map… so to speak.

{% twitter https://twitter.com/srl295/status/1283273224256544768 %}

Anyway, a lot happened with ICU and CLDR over the years. I wrote the original Survey Tool to collect data for it, which today looks and acts nothing like my original version (and that's a very good thing).

I also eventually got involved with translation, first by joining the XLIFF-TC (when I didn't actually do any translation nor use XLIFF), and only years later (in 2015) by being part of the team bringing you the [Globalization Pipeline](https://www.ibm.com/cloud/globalization-pipeline). <sup>**</sup> on Bluemix (now IBM Cloud).

{%asset_img gp-icon.png %}

Learning about translation, actually using XLIFF, has been great. But, I found less and less time to work on core globalization. I had a short stint trying out the Project Management role, and also worked to bring OpenWhisk adoption up to speed in our group.

I mentioned being a 2nd generation IBMer.  Not only that, I’m a second generation occupant of the same tower at the San Jos&eacute; site that my father was in (not concurrently).

~23 years is a long time. One of our kids had said years back, “I want to help you work on Bluemix someday!” Well, Bluemix is now the [IBM Cloud](https://cloud.ibm.com) (go check it out!). That would be a third generation. I’m still proud to be an IBMer. However, come this Tuesday—right on the schedule that was presented to me—I won't be.

-----

So what's next? Starting that same Tuesday, I'm going the independent contractor route. And my first contract is a short term one with **Unicode itself**, to work on **CLDR**!

So yes, I suppose the tweet above may count as a subtweet. CLDR is extremely important to how humans use computers in
their very own language and culture today, and I am excited to jump back in. What has become a “spare time project” will become literally my front burner work.

What's next?

-----

Endnotes:

- <sup>*</sup> _I've never been much for playing or authoring games._ 
- <sup>**</sup> GP icon under the [Apache 2.0 license](https://github.com/IBM-Cloud/gp-common/blob/master/LICENSE.txt)