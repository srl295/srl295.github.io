---
title: git twofer
s: git-twofer
date: 2019-09-09 09:36:01
tags: git
---

A couple of useful `git` commands. (Meaning: _writing this down so that I don’t forget it!_)

## remotes.group

I seem to have added a lot of remotes to some of my repos. Remotes are helpful for checking on the progress
of different branches people are working on. But what if I want to `git fetch --all` but not really fetch
_all_ remotes? There has to be.. a better way.

```sh
$ git config remotes.dev "srl295 upstream"
```

Aha. Now I can do:

```sh
$ git fetch -p dev
```

And only the two “dev” remotes (`srl295`, my work, and `upstream`, the upstream fork) get updated.

## worktree

I sometimes have several [icu](http://icu-project.org) branches going at once. A git worktree allows
me to work on several branches without having to have unrelated (several gig of metadata) directories.

```sh
$ cd ~/src/icu
$ git worktree add ../icu-64.2 release-64-2
```

Now `~/src/icu-64.2` contains a separate branch (or tag or whatever), but the objects (and LFS!) are
all shared. When I’m done with it,

```sh
$ cd ~/src/icu
$ git worktree remove ../icu-64.2
```

If I want to see what worktrees I have open, I can do this:

```sh
$ git worktree list
/Users/srl/src/icu           7cd02ed29e [cldr-prod-data]
/Users/srl/src/c-3-8         3e38a82d1c [c-maint-3-8]
/Users/srl/src/icu-4.8       ca6a2dd032 [maint/maint-4-8]
/Users/srl/src/icu-maint-50  52a662e0df (detached HEAD)
/Users/srl/src/icu-maint-62  7c7b8bd570 [maint/maint-62]
/Users/srl/src/icu-maint-63  4f715ae124 [maint/maint-63]
/Users/srl/src/icu-maint-64  b8db662f91 [fix-rpath-642]
/Users/srl/src/icu.aixgcc    b40f8b8b16 [aixgcc]
```

You can run the `worktree` commands from any of the worktrees. It’s like a happy git family of
worktrees!

Credit to Myles here: {% twitter https://twitter.com/MylesBorins/status/1080892685936967681 %}
