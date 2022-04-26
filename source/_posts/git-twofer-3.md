---
title: git twofer, the third
s: git-twofer-3
date: 2022-04-26 11:54:38
tags: git
---

Another year, and another twofer. Same theme as {% post_link git-twofer-2 TWOFER^ %} and {% post_link git-twofer TWOFER^^ %}:

> A couple of useful `git` commands. (Meaning: _writing this down so that I don’t forget it!_)

As before, these two are going to be given in the form of custom aliases.  You can add the following to your `~/.gitconfig` file or update the `[alias]` section of it exists.

```ini
[alias]
        skip = update-index --skip-worktree --
        unskip = update-index --no-skip-worktree --
```

## `skip` / `unskip`

I will cut right to the chase here: I put these in because of Eclipse `.settings/` files.

I work on several Java+Maven projects that have historically used the Eclipse IDE. For most of them, Maven is now the source of truth. However, there are a number of settings such as tabs vs. spaces and indent style, which ought to be shared. How do we know which files should be checked in and which shouldn’t? There does not seem to be a good way to tell.

As I noted in [this comment on CLDR-15048](https://unicode-org.atlassian.net/browse/CLDR-15048?focusedCommentId=162936), there are many files, as well as `.classpath`, change quite often, seemingly due to:

- mismatches in Eclipse versions as users upgrade or downgrade
- plugins added/removed on the user’s side
- using Eclipse vs. using VSCode Java (which is Eclipse inside)

### _Ugh, what is all of this? I was just trying to fix a typo in a comment!_

```shell
$ git status
Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   UnicodeJsps/.settings/org.eclipse.core.resources.prefs
        modified:   UnicodeJsps/.settings/org.eclipse.jdt.core.prefs
        modified:   UnicodeJsps/.settings/org.eclipse.wst.common.component
        modified:   UnicodeJsps/.settings/org.eclipse.wst.common.project.facet.core.xml
        modified:   unicodetools/.project
        modified:   unicodetools/.settings/org.eclipse.jdt.core.prefs

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        unicodetools/.settings/org.eclipse.wst.common.component
        unicodetools/.settings/org.eclipse.wst.common.project.facet.core.xml
```

Note that some of these files _don't_ seem to be automatically re-created by Eclipse when importing. So simply `.gitignore`ing some of the files doesn't work. We could ignore the entire `.settings` directory, but then each user could have potentially differing editor and other preferences.

### skip-worktree to the rescue… it seems

Here is how I use `git skip` in practice. There are changes, but I want git to forget-about-them.

```shell
$ git status
On branch cldr-14878/staging
Your branch is up to date with 'srl295/cldr-14878/staging'.

Changes not staged for commit:
  (use "git add <file>..." to update what will be committed)
  (use "git restore <file>..." to discard changes in working directory)
        modified:   tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs
        modified:   tools/cldr-code/.settings/org.eclipse.jdt.core.prefs
        modified:   tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs

no changes added to commit (use "git add" and/or "git commit -a")

$ ## Note: I have to skip files, not directories. git only knows files.
$ git skip tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs tools/cldr-code/.settings/org.eclipse.jdt.core.prefs

$ git status
On branch cldr-14878/staging
Your branch is up to date with 'srl295/cldr-14878/staging'.

nothing to commit, working tree clean
$ ## Clean! Now I can work with the files important to my task.
```

### But there’s a catch

Let’s rebase this particular branch:

```shell
$ git rebase upstream/main
error: Your local changes to the following files would be overwritten by checkout:
        tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs
        tools/cldr-code/.settings/org.eclipse.jdt.core.prefs
        tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs
Please commit your changes or stash them before you switch branches.
Aborting
error: could not detach HEAD
```

Oh no! [Someone](https://github.com/srl295) actually changed these files. OK, no problem, this happens all the time (I say to myself). I’ll just revert these changed files.

```shell
$ git checkout -- tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs tools/cldr-code/.settings/org.eclipse.jdt.core.prefs tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs
error: pathspec 'tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs' did not match any file(s) known to git
error: pathspec 'tools/cldr-code/.settings/org.eclipse.jdt.core.prefs' did not match any file(s) known to git
error: pathspec 'tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs' did not match any file(s) known to git
```

Working as designed. Git doesn’t know anything about these files… 🙀

### `unskip`

This is why there is an `unskip` command. We need to tell git to pay attention to these files again.

```shell
$ git unskip tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs tools/cldr-code/.settings/org.eclipse.jdt.core.prefs tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs
$ ## The files are now 'modified' again. But we can fix that:
$ git checkout -- tools/cldr-apps/.settings/org.eclipse.jdt.core.prefs tools/cldr-code/.settings/org.eclipse.jdt.core.prefs tools/cldr-rdf/.settings/org.eclipse.jdt.core.prefs
```

OK, now the rebase can succeed!

```shell
$ git rebase upstream/main
Auto-merging .github/workflows/ansible-lint.yml
CONFLICT (content): Merge conflict in .github/workflows/ansible-lint.yml
error: could not apply ac17b9bdff... ^ vagrant
```

Oops (this is a live demo!) well, it suceeded enough for me to deal with the substantive merge conflicts.

Of course, I can `git skip` files again when I want to.

### Final notes

`git update-index --skip-worktree` and `--unskip-worktree` work on a single worktree only. So if you have several worktrees checked out, you can skip and unskip them individually. This skippage also is never propagated by a `push` or `fetch`.

## One more thing - worktrees in action

We had a problem to solve.  Some [CLDR](https://github.com/unicode-org/cldr) processes, such as comparing with past versions, needed to see ALL past releases of the project, checked out side–by–side. This was previously done by literally unzipping each release into a special folder. One idea to automate this was to have an automatic unzipper, or to check in all of the versions into yet another repository.

Instead, I [wrote a Java tool](https://unicode-org.atlassian.net/browse/CLDR-15000) which automates the use of `git worktree`.

```shell
org.unicode.cldr.tool.CheckoutArchive
…
Setting up in $ARCHIVE /Users/srl295/src/cldr-archive …
# git worktree add /Users/srl295/src/cldr-archive/cldr-1.1 release-1-1
Preparing worktree (detached HEAD 9964598cd9)
HEAD is now at 9964598cd9 CLDR-15567 v1.1 add a historic tag for release-1-1 including dtd from unicode.org
# git worktree add /Users/srl295/src/cldr-archive/cldr-1.1.1 release-1-1-1
Preparing worktree (detached HEAD 07386be0f3)
HEAD is now at 07386be0f3 This commit was manufactured by cvs2svn to create tag 'release-1-1-1'.
# git worktree add /Users/srl295/src/cldr-archive/cldr-1.2 release-1-2
Preparing worktree (detached HEAD 504f0ecf2a)
HEAD is now at 504f0ecf2a This commit was manufactured by cvs2svn to create tag 'release-1-2'.
…
$ git worktree list
/Users/srl295/src/cldr                       615221b7eb [cldr-14766/userreg]
/Users/srl295/src/cldr-archive/cldr-1.1      9964598cd9 (detached HEAD)
/Users/srl295/src/cldr-archive/cldr-1.1.1    07386be0f3 (detached HEAD)
/Users/srl295/src/cldr-archive/cldr-1.2      504f0ecf2a (detached HEAD)
/Users/srl295/src/cldr-archive/cldr-1.3      fb82e42152 (detached HEAD)
/Users/srl295/src/cldr-archive/cldr-1.4      cb113490f3 (detached HEAD)
```

Since all version history is already in `cldr/.git`, it makes sense to leverage that into these 38 worktrees. Each worktree is checked out as a detached HEAD according to the release tag that produced it. This makes it very convenient to run tests and comparisons against the old versions.

Furthermore, unlike with an unzip, the full version history is available so `git log`, `git blame`, diffs and advanced searching is available to use.

## post post script

Enjoy this git twofer-plus, and as always

1. Don’t panic, `git reflog`
2. [xkcd](https://xkcd.com/1597/) is always right
