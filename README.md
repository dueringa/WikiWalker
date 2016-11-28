# WikiWalker
Given a wikipedia article, build a graph of article links

Input can be any Wikipedia URL. The results can be stored in a JSON cache file.
This cache file can be used in successive runs, whereas on each start the
cache file is read, combined with the Wikipedia data, and stored again.

## Building

Cmake is used as build system. You need curl and boost::program_options.

## Generating graphs with graphviz

Since graphs can get very wide, it's recommended to `unflatten` the graph first:

```
unflatten -l5 file.dot | dot ...
```

Where `5` is the "depth" the links get distributed to.

## Features

 - independent of systemd :p
