---
title: git twofer, again
s: git-twofer-2
date: 2021-03-29 11:54:38
tags: git
---

Time for another twofer on git. As I wrote in the {% post_link git-twofer last twofer %}:

> A couple of useful `git` commands. (Meaning: _writing this down so that I don’t forget it!_)

Of course, I did end up forgetting _even that I had posted it to the blog_.  Oh well.

These two are going to be given in the form of custom aliases.  You can add the following to your `~/.gitconfig` file or update the `[alias]` section of it exists.

```ini
[alias]
    ingredients = diff-tree --no-commit-id --name-only -r
    columbo = commit --amend --date=now
```

## ingredients

_What’s inside?_ That’s always something I ask myself when contemplating a grocery store purchase. I find myself asking the same question when I’m about to rattle off `git commit ; git push -f HEAD:prod` and deploy some hopefully-tested code—or, at least send it off for PR review.

`git diff-tree --no-commit-id --name-only -r` fits the bill… try it out:

```shell
$ git ingredients HEAD     # What's on the latest commit that I'm about to push?
$ git ingredients fe4b8570 # What's on some specific commit?
```

`git ingredients`  shows you a simple list of all of the files changed in the specific commit.

## columbo

“…Just one more thing.” Before Job’s famous keynote-ender, there was [Peter Falk](https://en.wikipedia.org/wiki/Columbo).

Anyway, sometimes you think you’re done with a commit… but there’s just one more thing. A test you forgot to run. Whitespace. Rewrite the whole thing—it’s up to you.

`git commit amend` takes whatever is staged for commit, and merges it into the HEAD commit. Type it often enough, and an alias is in order.  And just one more thing—if you just use `amend`, you will end up _retaining_ the original date and time.  Me, I like to bump the date to the time I last touched it.

- `git columbo` will commit the currently staged files (if any), and give you the opportunity to edit the commit message.

- `git columbo -m 'fix all the things'` lets you specify that message from the command line

- `git columbo somefile.p` will only commit `somefile.p` among all the available files

- `git columbo -a` One more thing: All the things! Commit everything you’ve changed.

- I also run `git columbo -S` sometimes if I decide I want to sign the commit.

Credit here goes to the tweet/thread below. I added the `--date=now` part.

{% twitter https://twitter.com/ceejbot/status/1128092331616301056 %}