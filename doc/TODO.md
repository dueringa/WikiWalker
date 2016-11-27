# TODO

 - [x] get unit tests to work without manually specifying .o files
 - [x] Support multiple wikipedias
 - [x] make local "cache file", so multiple runs can re-use articles fetched
       previously
 - [ ] support other Wikimedia installations
 - [ ] support local wikipedia dump
 - [ ] support output formats (dot, etc?)
 - [ ] support "routing"?
 - [ ] support getopt as command line parser
 - [ ] make CacheJson... an "input", as well as wikipedia. then, let class ??
       read first from cache file, then from Wikipedia.
 - [ ] handle rate limiting
   * [ ] depth with generators
   * [ ] Wikipedia Bot authentication