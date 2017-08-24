# WikiWalker
Given a wikipedia article, build a graph of article links

Input can be any Wikipedia URL. The results can be stored in a JSON cache file.
This cache file can be used in successive runs, whereas on each start the
cache file is read, combined with the Wikipedia data, and stored again.

## Building

Cmake is used as build system. You need curl and boost::program_options.

### Note for Debian / Ubuntu(?) users

Debian ships an old version of UniTest++, which also uses a custom pkg-config file
Unfortunately, this is completely incomtatible with e.g. Gentoo, and the current
version on GitHub, see unittest-cpp/unittest-cpp#145.

## Generating graphs with graphviz

Since graphs can get very wide, it's recommended to `unflatten` the graph first:

```
unflatten -l5 file.dot | dot ...
```

Also, have a look at gephi: https://gephi.org.

Where `5` is the "depth" the links get distributed to.

## Features

 - independent of systemd :p
