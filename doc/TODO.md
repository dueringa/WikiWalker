# TODO

* [x] get unit tests to work without manually specifying .o files
* [x] Support multiple wikipedias
* [x] make local "cache file", so multiple runs can re-use articles fetched
      previously
* [x] support getopt as command line parser
* [x] curl transmit compression?
* [ ] support output formats (dot, etc?)
  * [x] dot
* [x] support other Wikimedia installations
* [ ] support local wikipedia dump
* [ ] support "routing"?
* [ ] make CacheJson... an "input", as well as wikipedia. then, let class ??
      read first from cache file, then from Wikipedia.
* (WONTFIX) handle rate limiting
  * If I understand the docs, single-threaded queries don't matter. (i.e. one
    simultaneous is OK)
* (WONTFIX) handle Wikipedia Bot authentication
  * Probably not possible to register a bot at Wikipedia.
    Wikis can have bots, but these are maintained by the installation owners.
