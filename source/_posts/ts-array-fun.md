---
title: TypeScript Array fun
s: ts-array-fun
date: 2023-10-07 09:19:34
tags: typescript
---

What’s in an Array? I ran into an unexpected TypeScript (or perhaps JavaScript) issue. Yes, I know, yet another.

Here’s how it works. _Names have been changed to protect the author of [this pr](https://github.com/keymanapp/keyman/pull/9688)._

### Starting out

Let’s say you want an array class. Not just any array, a _special_ array.

```js
import "node:console";

class SplitArray extends Array<String> {
    constructor(strs : string, sep : string) {
        super();
        for(const s of strs.split(sep)) {
            this.push(s);
        }
    }
}
```

Not a lot to see here. It's a special array class, we have a special constructor to get the data just right.

```console
> const q = new SplitArray("this-and-that", "-");
> console.dir(q);
SplitArray(3) [ 'this', 'and', 'that' ]
```

So far so good. Seems to be working as intended, we can add all sorts of other functions and such.

### Just one more thing

Let’s add some debugging. Why not.

```js
class SplitArray {
    …
    toString() : string {
        return this.map(q => `${q}(length: ${q.length})`).join(',');
    }
}
```

A little excessive, but mostly harmless.  (right?)

```console
…
> q.toString()
/dev/null/arrays.ts:6
        for(const s of strs.split(sep)) {
                            ^


TypeError: strs.split is not a function or its return value is not iterable
    at SplitArray (/dev/null/arrays.ts:6:29)
    at SplitArray.map (<anonymous>)
    at SplitArray.toString (/dev/null/arrays.ts:12:21)
    at Object.<anonymous> (/dev/null/arrays.ts:21:15)
    …
```

Huh? Line 6 is the constructor. Why are we constructing this object, yet again? I was just trying to add a little toString()…

### The insanity check bounces

Maybe the `map` function is too complicated.  Let’s just change it to `this.map(q => q)` to see if it works at all. Nope.

Debugging shows that `strs` has the value of `3`. As in, the number.

### Reading the docs

[Array.prototype.map()](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/map), and I quote,
> creates a new array populated with the results of calling a provided function on every element in the calling array

That’s what we’re trying to do, alright.

### Working as Designed. Or, *Don’t Do That.*

The problem here is that my little class doesn’t just have an array, it *is* an array. And so it is expected to act like one.

The [`Array()` constructor](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/Array) has a few overloads:

- new Array()
- new Array(element1, element2?, /* …, */ elementN)
- new Array(arrayLength)

arrayLength… such as, perhaps, 3?

The call to `super()` should have been a clue here. What’s happening internally is that map(q => q) is doing something like:

`n = new SplitArray(3);  n[0] = q[0];  n[1] = q[1]; n[2] = q[2];`

That's all fine, except for the constructor.

### Super. Back to the factory

To fix this, we can use a factory static instead of our “helpful” constructor, which turned out to be less than helpful to the map()

```js
class SplitArray extends Array<String> {
    static split(strs : string, sep : string) {
        const a = new SplitArray();
        for(const s of strs.split(sep)) {
            a.push(s);
        }
        return a;
    }

    toString() : string {
        return this.map(q => `${q}(length: ${q.length})`).join(',');
    }
}
```

Now, finally, `toString()` will work properly. For some definition of properly.

```console
> q.toString()
this(length: 4),and(length: 3),that(length: 4)
```

We also could have contained the Array as a property, instead of subclassing.

The tl;dr (now that you’ve perhaps already read it) is: Use caution when overriding Array constructors. Consider having an array instead of being one.
