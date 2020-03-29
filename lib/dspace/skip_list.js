

/*

    http://ticki.github.io/blog/skip-lists-done-right/
    https://brilliant.org/wiki/skip-lists/


    a skip list is a data structure that allows {\displaystyle {\mathcal {O}}(\log n)}{\mathcal {O}}(\log n) search complexity as well as {\displaystyle {\mathcal {O}}(\log n)}{\mathcal {O}}(\log n) insertion complexity within an ordered sequence of {\displaystyle n}n elements.
    Thus it can get the best features of an array (for searching) while maintaining a linked list-like structure that allows insertion- which is not possible in an array

    "We used skiplists in front of a traditional B+tree engine in WiredTiger (the storage engine behind MongoDB).
    One comment: while it's relatively easy to make forward traversal of a skiplist lockless, making reverse traversal lockless is actually quite difficult.
    It would be an interesting addition to the article to show both lockless insert and lockless traversal (especially lockless reverse traversal). There's an implementation of lockless reverse traversal inside WiredTiger that I'd be happy to generalize if you're interested in updating the article.""

    Self-balancing Binary Search Trees often have complex algorithms to keep the tree balanced, but skip lists are easier: They aren’t trees, they’re similar to trees in some ways, but they are not trees.

     Skip lists are wonderful as an alternative to Distributed Hash Tables. Performance is mostly about the same, but skip lists are more DoS resistant if you make sure that all links are F2F.
     Each node represents a node in the network. Instead of having a head node and a nil node, we connect the ends, so any machine can search starting at it self:

    By apply a lot of small, subtle tricks, we can drastically improve performance of skip lists, providing a simpler and faster alternative to Binary Search Trees.

    Flat arrays
If we are interested in compactness and have a insertion/removal ratio near to 1, a variant of linked memory pools can be used: We can store the skip list in a flat array, such that we have indexes into said array instead of pointers.

Unrolled lists
Unrolled lists means that instead of linking each element, you link some number of fixed-size chuncks contains two or more elements (often the chunk is around 64 bytes, i.e. the normal cache line size).

Unrolling is essential for a good cache performance. Depending on the size of the objects you store, unrolling can reduce cache misses when following links while searching by 50-80%.
Searching is done over the skip list, and when a candidate is found, the chunk is searched through linear search. To insert, you push to the chunk (i.e. replace the first free space). If no excessive space is available, the insertion happens in the skip list itself.
Note that these algorithms requires information about how we found the chunk. Hence we store a “back look”, an array of the last node visited, for each level. We can then backtrack if we couldn’t fit the element into the chunk.
We effectively reduce cache misses by some factor depending on the size of the object you store. This is due to fewer links need to be followed before the goal is reached.


https://github.com/gramoli/synchrobench
https://gramoli.redbellyblockchain.io/web/doc/pubs/gramoli-synchrobench.pdf
        * Transactional memory [46] (TM) is appealing for simplifying
concurrent programming. TM offers transactions used to delimit
a region of code that should execute atomically. A
        * CAS allows to develop the fastest algorithms for multicores at the expense of great complexity
        * TM is a mature synchronization technique offering performance that are more consistent across update ratios and programs than locks.
        * the lock-free queue (CAS) is much faster than the reusable queue (TX)
        * copy-on-write and read-copy-update are more sensitive to contention than other techniques, yet they help achieving high performance under read-only workloads
*/
