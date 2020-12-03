---
layout: post
title:  "Introduction post"
date:   2020-12-03 13:28:12 +0100
categories: tleilax
author: Bredlej
---
I started the development of TleilaxRL_modern a couple of months ago, but only now decided to run a blog alongside of it.

My intention for this blog is to write down some of my thoughts and musings about the project and the challenges 
I'm facing when programming it.

## What is TleilaxRL_modern?

**TleilaxRL_modern** is a hobby project where I try to write a roguelike 
about deep-space exploration in the C language. 

Something like [Elite](https://en.wikipedia.org/wiki/Elite_(video_game) "Elite (Wikipedia)") 
or [No Man's Sky](https://en.wikipedia.org/wiki/No_Man%27s_Sky "No Man's Sky (Wikipedia)") 
meets [Nethack](https://en.wikipedia.org/wiki/NetHack "Nethack (Wikipedia)"). 

The *_modern* part in the name comes from this being a rewrite of another project that I started before, 
called just [TleilaxRL](https://github.com/bredlej/TleilaxRL).

So the background is that I really enjoyed 
One Lone Coders [video about procedural generation](https://www.youtube.com/watch?v=ZZY9YE7rZJw) and decided to try it out myself.

I started the first project with ASCII chars in Ncurses and it looks like this:

![TleilaxRL screenshot]({{ site.url }}/assets/tleilaxrl.gif)

I think it's pretty cool for a proof of concept, but there are some issues with it where I decided to start it new:

### 1. Too much Lua scripts

When I first approached programming of this project my main intention was to learn C. 

Coming from over a decade of Java programming in my career I wanted to get back to the roots and learn some old-school programming
that to me was fascinating and scary when I was younger.

Now it's not scary anymore but it takes some work to be able to use a lot of data structures like Lists and Maps and such, 
which are used with no effort at all in higher level languages like Java, or you could say most modern languages.

So I knew about integration of C with scripting languages, decided to try it out - and found it to be pretty awesome!

Suddenly I could do fast prototyping of everything I wanted in a script language, interface with C functions and not
worry anymore about memory allocation and things like that.

The result of this is the projects language statistics:

![TleilaxRL languages]({{ site.url }}/assets/tleilaxrl_languages.png)

I wanted to learn C and not Lua, and I had plans to rewrite everything in the former but they never came to be.

But fun fact is that in the current state of **TleilaxRL_modern** Lua bindings are also present and ready to use, just 
haven't had a need for this yet. I'm reserving this for some special occasions and keep at doing everything in C for now.

### 2. NCurses frontend

I started writing the original **TleilaxRL** project in Vim connected through SSH to a headless Raspberry Pi,
so there were not many choices available for a graphical representation of the game state.

I don't mind ASCII graphics in roguelikes and love to play **Nethack** this way, but still found it to be annoying.

One thing was that while it ran nicely on Linux, there was no way (I think) to run it on Windows too without hassle.

I think Vim is pretty an awesome editor, but I found myself not to be that old-school to enjoy programming in it 
instead of using a modern IDE or something like VS Code.

Nowadays I use [CLion](https://www.jetbrains.com/clion/ "CLion") for programming and it's perfect for my tastes. 
A pretty cool function of it is to develop and deploy remote software through SSH, so I'd rather use that now.

But another thing that I found annoying with NCurses is it's really limited and hard to use color pallete.

Games like [Brogue](https://en.wikipedia.org/wiki/Brogue_(video_game)), [Caves of Qud](https://www.cavesofqud.com/) 
or [CDDA](https://cataclysmdda.org/) can be played too with ASCII characters, but IMO they look and play better with 
tilesets.

So I found NCurses a bit limited and annoying to work with and decided to use "real" graphics in **TLeilaxRL_modern**.

### 3. CMake and new insights about C programming

After taking a break from **TleilaxRL** I got interested in emulation development and have been wondering about how 
it's done. 

I found in threads on Reddit that it's recommended to start with writing a simple emulator of a Chip8 system, which gives 
a nice overview on how emulators work in general.

I decided to [try it out](https://github.com/bredlej/chip8-emu), but wanted to write it more structured than my usual C 
approaches, and found out about [CMake](https://cmake.org/).

While I still don't grasp many things it has to offer, I find it to be a very valuable tool that makes your projects 
more managable and let's say mature, than just having random sources laying in folders with simple Makefiles if you even 
bother to write them.

The current structure of **TleilaxRL** looks ugly to me now without CMake, and I guess that if I went back to developing it,
the first thing I'd do is to create CMakeList.txt files and declare the project in an orderly way.

### 4. 2d vs 3d graphics

The original project had the universe generated in a 2D plane, but I had some nostalgia recently when I remembered dabbling
in OpenGL as a student and wanted to try it out again.

I thought doing this procedural universe generation would be a perfect challenge to render in three dimensions, 
so I started working on it.

At first I thought about using [SDL2](https://www.libsdl.org/index.php) for it, 
but found [Raylib](https://www.libsdl.org/index.php) to be more enjoyable to use - it's really great!


So those are four main reasons for me to have started a new fresh project.

## The current state

![TleilaxRL_modern]({{ site.url }}/assets/tleilaxrl_modern.gif)

The first commit of **TleilaxRL_modern** to my GitHub repository was in the 24th October 2020 (my birthday!).

Currently the following features are implemented:

1. Project structure and build definition in CMakeLists.txt files
2. Procedurally generated stars rendered in a 3D environment
3. Finite state machine implementation used for switching between game states
4. Entity-component-system with a couple of definitions of the relationships between stars and celestial bodies orbiting it
5. Unit tests

I'll leave the detailed description and explanation of the mentioned features for another blog post.

I'm pretty happy what I've already got with the implementation but there's a lot of stuff yet to do, which I'll also 
write down in another post.

That's it for the introduction, thanks for reading!