# Remove memory allocations on all intermediate stages

1.  [ ] The parser module should work on a given buffer and return result
    in it.
2.  [ ] ~~The kind module should precompile all regular expressions on
    the initialization stage.~~ Do not use regular expressions at all — use
    hand-crafted procedures. Limit allowed name lenght (for example to 32
    characters).
3.  [x] Allocate all nodes on a given buffer. Remove node references as it
    will be not needed with new interface.

    Buffer model:
    1.  padding to aling to pointer boundary ~~(not needed if the buffer
        is an array of pointers)~~;
    2.  pointer to the top ~~(this prevents usage of substacks)~~;
    3.  sequence of nodes.

    NOTE: ~~In any way with this model you cannot push into substack without
    destroying top nodes.~~ Substaks does not supported.

    Node model:
    1.  pointer to parent node;
    2.  embedded value — null-terminated character string;
    3.  padding to aling to pointer boundary.

    NOTE: With this model node printing and validation procedures need
    not call itself recursively — just iterate from bottom node (buffer
    head) up to top.
4.  [ ] Q: Can we use built-in buffer in node printing, validation and
    reading procedures?
