# WikiWalker
Given a Wikipedia article, build a graph of article links

Input can be any Wikipedia URL. The results can be stored in a JSON cache file.
This cache file can be used in successive runs, whereas on each start the cache
file is read, combined with the Wikipedia data, and stored again.

## Building

CMake is used as build system. You need curl and boost::program_options.

### Note for Debian / Ubuntu(?) users

Debian ships an old version of UnitTest++, which also uses a custom pkg-config
file Unfortunately, this is completely incompatible with e.g. Gentoo, and the
current version on GitHub, see also
[here](https://github.com/unittest-cpp/unittest-cpp/issues/145).

### Building under Windows

Why would you want to do that?

You'll need
* curl from https://curl.haxx.se/ - build from source following the instructions
  in winbuild
* Boost libraries from www.boost.org

By default, cmake wants to use dynamic boost libs, but by default, boost bootstrap
builds static libs only.
So you need to set Boost_USE_STATIC_LIBS=ON in addition to BOOST_ROOT in cmake.

Alternatively, have a look at [vcpkg](https://github.com/Microsoft/vcpkg), a tool
to build and install libraries under Windows, and make them findable by cmake.

## Generating graphs with Graphviz

Since graphs can get very wide, it's recommended to `unflatten` the graph first:

```
unflatten -l5 file.dot | dot ...
```

Also, have a look at [Gephi](https://gephi.org).

Where `5` is the "depth" the links get distributed to.

## Features

 - independent of systemd :p

## Formatting

The root directory contains a .clang-format, which can be used to reformat the
source code with clang-format. BoostPoC.l.p. is, unfortunately, formatted in
some weird way which makes the initialization of the commands pretty
unreadable. So this re-formatting should not be committed.
