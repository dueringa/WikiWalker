# Main idea

Given a Wikipedia Article URL, scan the article and follow all encountered article links,
repeat the proccess recursively to a certain depth.

In the end, we have a graph of articles.

Links to other articles should always be scanned, but these articles themselves shouldn't neccessarily.

# Thoughts

I'm not sure whether this "Walker" class is such a good idea.
It was intended as a starting point and have the responsibility of getting links of articles within the article.

But as it is designed now, there would be one global Walker for the program (since the walker holds th article collection).
But a walker would also need to walk through other articles recursively...

Thus, it might not be such a good idea...

# Design

An *article* consists of of title and a collection of other articles it links to.

All articles must also reside in a "global" collection (which might be in *another class*).
A) This is to avoid creating infinite articles because of multiple links (i.e. article A -> B, B -> C, A -> C).
B) Also, there might be cyclic links (i.e. article A links to B, B -> C, C -> D and D -> A).
Using this global collection one could look up if the article instance already exists.

I'm not really happy with the C pointers, regarding resource cleanup.
I need to avoid double freeing in case A.
Using only shared pointers is not possible because of B.

Another class should be responsible for parsing the article. This parser could either add linked articles
to the article itself, or simply return a collection. This is because the result of the parser needs to be
parsed again.

# TODO

 - [ ] Support multiple wikipedias
 - [ ] support other Wikimedia installations+
 - [ ] support local wikipedia dump
 - [ ] support output formats (dot, etc?)
 - [ ] support "routing"?

![overview](design.jpg)