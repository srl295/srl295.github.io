---
title: ICU Infrastructure Migration
tags:
  - icu
  - node
  - scm
  - perl
  - unicode
s: icu-infra
date: 2018-07-02 18:24:54
---

Well, it’s done. As of [the end of the first half of 2018](https://time.is/2359_30_Jun_2018_in_UTC?SRL_icu-infrastructure_EOL) I have exitted the role of primary infrastructure support for [Unicode ICU](https://ssl.icu-project.org), which I have had on and off since about 2002. (ICU has been open source since 1999, and the source code has roots going back even further.)

Keep your eye on https://ssl.icu-project.org for new links/instructions to the new structure. It’s not my intent to walk you through the new ICU development process, mostly because some of it is still being decided!  What I want to cover in this post is the actual migration process.

{% asset_img iculogo_dev.png "ICU Development" %}


>**Note:** In the first edition of this post, I didn't make a couple of things clear enough:
>
>1. **Teamwork** — I did not accomplish all of the steps below alone. Thanks to all of the ICU-TC colleagues for helping with review and engineering tasks (that are still ongoing as I write this).
>
>2. **I’m not done with ICU** — I remain a member of ICU-TC and I hope to actually contribute something again, now that my time isn’t spent “keeping the lights on.”

# scoping the problem

The major two apects that needed migration were:

- source code: 42,000 commits from Subversion to Git - hosted on GitHub (thanks!)
- bugs: 13,000 bugs from [trac](http://trac.edgewall.org/) to JIRA, hosted on the Atlassian cloud (thanks!)

Notice a repeated key word above:  hosted. 

{% twitter https://twitter.com/zenodotus/status/1004435078854922240 %}

Hosted mean that this role _goes away_. This is a continuation of a trend started a few years ago when I recycled 1,500+ pounds (680+ kg) of server equipment that used to be the ICU continuous build farm.

## svn to git

Subversion to git may not sound like it should be particularly difficult, using [subgit](https://subgit.com/) (thanks for the OSS license!) and others. However there are a number of mitigating factors.

### Factor 1: Nonstandard svn tree use

If you have ever set up your own Subversion repo, you will be familiar with the top level trunk/branches/tags structure. You may also be aware that in svn (as is the UNIX way) “everything is a directory.” ICU had started with [_separate projects_](http://svnbook.red-bean.com/en/1.6/svn.reposadmin.planning.html) for icu4c and icu4j like so:

```
    icu/
        trunk/
        tags/
            release-1-2-3/
        branches/
            srl/
                someotherfeature/
    icu4j/
        trunk/
        tags/
            release-1-2-3/
        branches/
            srl/
                somefeature/
```


### Factor 1b: _changing_ nonstandard tree use

At [some point in 2016](https://unicode-org.atlassian.net/browse/ICU-12800) we decided that it was a good idea (and it was) to merge the trees. ICU for C and J are really developed together, and there is important interlock between the two regarding generated data files.

```
    trunk/
        icu4c/
        icu4j/
    tags/
        release-1.2.3/
            icu4c/
            icu4j
    branches/
        srl/
            coolFeature/
                icu4c/
                icu4j/
```

So far so good, but this point of discontinuity confuses the standard migration tools.

### Factor 1c: accidental tree deletion

Mistakes happen. But, this one means it looks like all source was deleted and replaced.

### Factor 2: large .jar files

Each .jar file ins't very big by itself. But ICU4J has a binary copy of its data file checked in. But there are thousands of copies of the icudata and other jars in the svn history. When all the dust settled, we probably end up with 2.3G of [git lfs](https://git-lfs.github.com) in 600 objects.

## trac to JIRA

The trac to JIRA importer was not available to us (not available in JIRA cloud anymore). CSV import seemed very unwieldy, as we needed to be able to incrementally update the issues a we were developing the mapping. Plus, our trac instance has many customizations, with source patches (yes, contributed back where they made sense) and [a custom plugin](https://github.com/unicode-org/icu-trac-tools) that powered our workflow.

# solutions, tries, retries…

- The ICU team has been seriouly discussing a move to some form of DVCS since early 2016.
- in December 2017, with management approval to spend the necessary engineering time in 2018, I informed the ICU-TC:

> By the end of 2Q2018, let's call it 2018-06-30T23:59:59.999Z, my infrastructure involvement in ICU needs to go to zero. This means no root logins…
>
> Note, I'm only talking about infrastructure, not other project involvement.

- By May, 2018 we had narrowed down what the future direction should look like.
- In June 2018, the ICU-TC decided to go ahead with the migration as planned.

## svn to git

Subgit works quite well. It takes some time, but it is worth it for a configurable conversion.  However, it would not handle the discontinuities mentioned above.

I knew that Subversion has a [dump format](https://stackoverflow.com/a/1579482/185799). Perhaps it would be possible to manipulate the dump, to make it look like ICU had _always_ had a 'merged tree', and then import from there?  ICU’s dumpfile is about 20 Gb.

{% twitter https://twitter.com/srl295/status/1006629782337929216 %}

I found some stack overflow questions that didn't quite match what I needed. I ran across [SVN::DumpReloc](https://metacpan.org/release/SALVA/SVN-DumpReloc-0.02) in CPAN, and noted it for future reference. It didn’t work out of the box.

The challenge is that the svn dump is just a simple dump of the internal binary deltas. It does not take well to mkdir or copies with no intermediate dirs. So, simply renaming `/icu/trunk/source/common/uloc.cpp` to `/trunk/icu4c/source/common/uloc.cpp` in old revisions won’t work, because `/trunk` didn't exist until 2016.

### node all the things?

As usual, I reached for `npm init -q -y` and started off to write a processor for the svn dump. I learned how to implement a Duplex stream, and got [a little ways](https://github.com/unicode-org/icu-remunge-svndump/tree/984e51003675d6c16201d74fd82ba1a5110b9e7f#theory) but definitely not far enough:

- I tried to load the entire dumpfile into memory
- I started in String space, assuming utf-8, whereas the svn dump is a mix of ASCII control headers (RFC 822) and binary blobs of arbitrary size.
- And, it just plain didn’t work without making the dumpfile unloadable.

### perl comes through (again)

{% asset_img perlref.jpg "Vromans, Johan. Perl: Pocket Reference. Sebastopol: O’Reilly, 2002." %}

I dusted off my [perl pocket reference](http://www.worldcat.org/title/perl-pocket-reference/oclc/474715752) and even-dustier perl skills and set out to update [SVN::DumpReloc](https://metacpan.org/release/SALVA/SVN-DumpReloc-0.02).   Unlike my js code, the perl actually worked. And working is good here.

I ended up adding a simple [JSON configuration structure
(https://github.com/unicode-org/icu-remunge-svndump/blob/master/icureloc.json) that would do three things:

- create `/trunk`, `/branches`, `/tags` in revision `r1` where we should have created them.
- make sure the future attempts to create those dirs gets ignored - by changing `add` to `change` (no-op property change).
- run all paths through some regexes to fix the paths up just right.

```json
{
    "map": [
        [ "^(icu4j|tools)\\/(trunk)\\/(.+)$", "\"$2\\/$1\\/$3\""],
        [ "^(icu4j|tools)\\/(trunk)$", "\"$2\\/$1\""],
        [ "^(icu)\\/(trunk)\\/(.+)$", "\"$2\\/icu4c\\/$3\""],
        [ "^(icu)\\/(trunk)$", "\"$2\\/icu4c\""],
        [ "^(icu|icu4j|tools)\\/(tags|branches)\\/(.*)$", "\"$2\\/$1-$3\""]
    ],
    "r1": {
        "mkdir": [
            "/trunk",
            "/branches",
            "/tags"
        ]
    }
}
```

In the end, it worked.  A few bugs remained:  branches and tags pre/post merge aren't quite where we want them. But the bulk of the svn history is kept.

## JIRA and, bugs everywhere. 

Given the above restrictions, I created a new node.js tool, https://github.com/unicode-org/icu-trac2jira to migrate a trac .sqlite3 dump to a JIRA database— by using the REST API. With a [minimum of configuration](https://github.com/unicode-org/icu-trac2jira/blob/master/config.json) it is able to map all of the fields, wiki syntax, and attachments needed to preserve our issue history. It's not perfect, and there's work to be done to fix some of the values, but I think it got the job done as far as initial migration.

The interesting thing, process-wise, is that I ended up with something that could run _incrementally_ to update JIRA to match trac. So as there was feedback on errors in the wiki syntax conversion, I could re-run the tool over a subset of the tickets and it would either update a ticket, comment, etc. or cause no change depending on whether JIRA matched the expected results.

A separate script created 20,000 empty tickets in a block, before running this converter. This allowed us to keep the same ticket IDs between trac and jira.

# Haven’t we done this before?


http://icu-project.org was already the third external web host for ICU, after [jtcsv.com](http://web.archive.org/web/20011104161753/http://www.jtcsv.com:80/) (2001 mirror) and [oss.software.ibm.com/icu](http://web.archive.org/web/20040209035756/oss.software.ibm.com/icu) (2004 mirror).

In 2006 I migrated ICU from cvs and [JitterBug](https://www.samba.org/jitterbug/) to svn and trac. So yes, we've done this before!

{% asset_img icubugtracking.png "ICU Trac logo by Syn Wee" %}

JitterBug (which I also customized extensively and added new report CGIs to) had a very simple hierarchical file structure which was very hackable. Since trac used a sqlite database, I wrote source to read this file structure and emit SQL to recreate the bugs in the new form.


```c
// jb2svn.c
    switch(tolower(s[0])) {
    case '[': return "";
    case 'a': return "assess"; // @@ assess?
    case 'c': 
        switch(tolower(s[1])) {
                case 'r': return "critical";
                case 'o': return "critical"; // @@ Committed?
                default: return def?def:s;
        }break;
    case 'e': return "major"; // expected
    case 'm': return "minor"; // medium
    case 'l': return "trivial"; // medium
    case 'h': return "major"; // high
```

An oddity of that conversion is that I sort of punted on converting the date fields at all. Maybe there either wasn't a ticket-creation time, or the files had all been re-`touch`ed at some point. Or maybe it was just… laziness. Or whatever the other two are (I'd have to look it up).

{% asset_img tractime0.png "Changed 49 years ago by auditor" %}


Of course, our conversion process faithfully preserves this history. I think `1970-01-01T00:00:28.000Z` is due to wanting a unique timestamp for some reason, thus (epoch time + 1 second per bug)-ish?

{% asset_img tractime1.png "Changed in 1970 by auditor" %}

