

## dd

Non-blocking Hash Table in Rust
    - https://github.com/ezrosent/wf_hash_writeup/blob/master/writeup.md
    - https://users.rust-lang.org/t/non-blocking-hash-table-in-rust/9297

https://github.com/share/sharedb

SPMP: A JavaScript Support for Shared Persistent Memory on Node.js
    http://www.bookmetrix.com/detail/chapter/795f1e7f-efef-421a-8421-26dd691a61b8#downloads

TOWARDS IMPLICIT PARALLEL
PROGRAMMING FOR SYSTEMS
    https://cfaed.tu-dresden.de/files/Images/people/chair-cc/dissertations/1912_Ertel_PhD.pdf

What is the fastest in-memory key-value store with support for shared memory?
- sharedhashfile ? “example on an 8 core Lenovo W530 laptop showing a hash table with 100 million keys, and then doing 2% delete/insert and 98% read at a rate of over 10 million operations per second”
- SharedHashFile [1] which is a shared memory hash table implementation that manages about 2.5 million gets per second, and between 1 & 2 million puts per second across 16 processes on a 16 vCPU Rackspace server.
- There's also LMDB [2] which is a shared memory B+tree implementation optimized for puts that manages [3] about 6.x million gets per second, but only 0.1 million puts per second -- even when using /dev/shm and no disk is involved -- across 16 processes on the same 16 vCPU Rackspace server.
- http://whitedb.org/

 Go Proverbs is “Don’t communicate by sharing memory, share memory by communicating.”
     https://www.quora.com/What-is-the-actual-meaning-of-Gos-Dont-communicate-by-sharing-memory-share-memory-by-communicating

Concurrency is not Parallelism by Rob Pike
    https://www.quora.com/What-is-the-actual-meaning-of-Gos-Dont-communicate-by-sharing-memory-share-memory-by-communicating

Shared mem vs msg pass
    http://courses.csail.mit.edu/6.888/spring13/lectures/L5-comm_models.pdf

distributed, replicated data structures for Sharing Actors
    https://github.com/cafjs/caf_sharing

Generic Messages: Capability-based Shared
Memory Parallelism for Event-loop Systems
    https://stefan-marr.de/downloads/ppopp-salucci-et-al-generic-messages-capability-based-shared-memory-parallelism-for-event-loop-systems.pdf

HashMap Vs. ConcurrentHashMap Vs. SynchronizedMap – How a HashMap can be Synchronized in Java
    https://crunchify.com/hashmap-vs-concurrenthashmap-vs-synchronizedmap-how-a-hashmap-can-be-synchronized-in-java/



### AOL / LSMTree / CRDT

https://github.com/alangibson/awesome-crdt

https://martin.kleppmann.com/

OpSets: Sequential Specifications for Replicated Datatypes (Extended Version)
    https://arxiv.org/pdf/1805.04263.pdf

    Whenever a node makes a modification to that structure, it records the change as an operation. For example, an operation may describe an insertion at a particular position in a text document. Each node locally maintains a set of operations, the OpSet. Whenever a node makes a change to the shared data, it adds the corresponding operation to its OpSet, and also sends messages containing the operation to other nodes. Whenever a node receives a message from another node, the operation in that message is added to the recipient’s local OpSet. Operations remain immutable throughout this process.

    We assume that each operation has a unique identifier (ID), that new IDs can be generated by any node without communication with other nodes, and that we have a total ordering on operation IDs. These requirements can easily be met by using Lamport timestamps [35] as IDs. A Lamport timestamp is a pair (counter, nodeID) that is constructed as follows:

        * counter is an integer. To generate a new ID, find the maximum counter of any existing operation ID in the local OpSet, and increment that number.
        * nodeID is a string that uniquely identifies the node generating the ID, e.g. a UUID [36].

    3 Specifying a Graph of Lists, Maps, and Registers

    We now make the OpSets approach concrete by defining example semantics for commonlyused data structures: maps (which associate values with user-specified keys) and lists (linear sequences of values). The map datatype can also represent a set (by using keys as members of the set, and ignoring values). The list datatype can also represent text (by mapping each character to a list element). In both lists and maps the values may be primitives (such as numbers or strings), or references to other map or list objects. Using these references we can construct arbitrary object graphs, including cycles of object references, like in object-oriented programming languages. In § 5 we will show how to restrict this object graph so that it conforms to a tree structure.

        (id, MakeMap)
        (id, MakeList)
        (id, MakeVal(val)) 
            associates the ID id with the primitive value val (e.g. a number, string, or boolean). This operation is used to “wrap” any primitive value, allowing Assign operations (see below) to always use IDs as values, reg
        (id, Assign(obj, key, val, prev))
            assigns a new value to a key within a map (if obj is the ID of a prior MakeMap operation), or to a list element (if obj is the ID of a prior MakeList operation). In the case of map assignment, key is the user-specified key to be updated, which may be any primitive value such as a string or integer. In the case of a list, key is the ID of the list element to be updated (i.e. the ID of a prior InsertAfter operation). val is the ID of the value being assigned, which may identify a MakeMap, MakeList, or MakeVal operation. prev is the set of IDs of prior Assign operations to the same key in the same object, which are overwritten by the present operation.
        (id, Remove(obj, key, prev))

    3.2 Interpreting Operations

    To encode the current state of map and list data structures we use a pair of relations (E, L):

        The element relation E

        The list relation L
            * L never shrinks
            * it only ever grows through interpreting InsertAfter operations.
            * When a list element is removed by a Remove operation, the effect is that all values are removed from the list element in the element relation E, but the list element remains in L as a tombstone, so that any concurrent InsertAfter operations can still locate the referenced list position. Thus, from a user’s point of view a list element only exists if it has at least one associated value in the E relation; any list elements without an associated value should be ignored.

        * Assign and Remove updates only E and leaves L unchanged
        * InsertAfter and MakeList updates only L
        * Assign and Remove interpretations remove any tuples from causally prior assignments (those whose IDs appear in prev), but leave any tuples from concurrent assignments unchanged This is the behaviour of a multi-value register; if a last-writer-wins register is required, the condition id0 ∈/ prev can be changed to obj0 6= obj ∨ key0 6= key, which removes any existing tuples with the same object ID and key

    5 A Replicated Tree Datatype

    * every map or list object has a unique ID (namely, the ID of the MakeMap or MakeList)  objects can reference each other using these IDs.
    * We now build upon this model, showing how to restrict the object graph so that it is always a tree.
        *  A tree is a graph in which every vertex has exactly one parent (except for the root, which has no parent), and in which the parent relation has no cycles.

    5.2 Specifying a Tree with Atomic Moves

        * Our specification rules out violations of the tree structure
        * concurrent moves do not duplicate tree nodes
        * our CRDT does not require any locks or global synchronisation.

    A

        * setMapKey(O, map, key, val
        * removeMapKey(O, map, key)
        * valueID(O, val)
            * generates a new Make · · · operation for the value, and the new operation is added to O0)
        * setListIndex(O, list, index, val)
        * insListIndex(O, list, index, val)
        * removeListIndex(O, list, index)

    C.1 Abstract OpSet

        OpSet is a set of (ID, operation) pairs with an associated total order on IDs (represented here with the linorder typeclass), and satisfying the following properties:

            * The ID is unique (that is, if any two pairs in the set have the same ID, then their operation is also the same).
            * If the operation references the IDs of any other operations, those referenced IDs are less than that of the operation itself, according to the total order on IDs. To avoid assuming anything about the structure of operations here, we use a function deps that returns the set of dependent IDs for a given operation. This requirement is a weak expression of causality: an operation can only depend on causally prior operations, and by making the total order on IDs a linear extension of the causal order, we can easily ensure that any referenced IDs are less than that of the operation itself.

    C.4 The Replicated Growable Array (RGA)
        a replicated list (or collaborative text-editing)

https://arxiv.org/pdf/1608.03960.pdf

    4.2.2 Operation Structure
    An operation is a tuple of the form:
        id : N × ReplicaID,
        deps : P(N × ReplicaID),
        cur : cursor(hk1, . . . , kn−1i, kn),
        mut : insert(v) | delete | assign(v) v : VAL

Kappa Architecture
    https://milinda.pathirage.org/kappa-architecture.com/

The Log: What every software engineer should know about real-time data's unifying abstraction
https://engineering.linkedin.com/distributed-systems/log-what-every-software-engineer-should-know-about-real-time-datas-unifying

End-to-end Encrypted Scalable Abstract Data Types over ICN
https://conferences.sigcomm.org/acm-icn/2018/proceedings/icn18-final30.pdf

    The Network becomes the Database
        . In the new architecture there is no need for stand-alone database servers: Instead, all digital assets are handled as “linked encrypted data” accessed via a flat namespace from an Information Centric Network

        A chat message is referenced by its self-certifying name (the hash of its binary representation), while a collection of chat messages, i.e. a chat room, is a linked list of such hash names, and the room’s state being represented by the most recently added chat message. Chat messages are immutable, hence their name never changes. But chat rooms have dynamic content, hence a name derived from a binary representation of the latest chat message not sufficient. This is solved by creating a permanent “base name” from the empty room, to use this as durable reference for a chat room, and to provide a lookup service for getting hold of the latest version’s name based on the base name. In our prototype we call this lookup the HEAD service, while we call ICN’s storage for immutable content the WORM service (write-once-read-many). Similar to GitHub’s role as a version directory [10], or IOTA’s “set of tip nodes” which is also called “edge set” [21], *the HEAD service documents the dynamic frontier of the append-only WORM.*

    A canonical ICN network  provides a single service called read()
    A first step consists in complementing the current “READ” primitive (implemented with Interest/Data messages)
with (a) a “WRITE” primitive on equal footing (WORM service), (b)
Remote Procedure Call support (RPC) as integral part of ICN, (c)
a name translation service for updating and disseminating latest
version information (HEAD service), and finally (d) libraries that
turn these base services into high-level abstract data types which
hide the tedious details of implementing them with scaling and
encryption in mind (see Figure 1).

    2.3 Append-only (and Log-Structured) Storage
        ICN so far is very much focused on single name-data chunks
rather than how multiple chunks are actually persisted, both at
the retrieving end (in a repo) or on the writing side – which is
relevant for performance and for data consistency reasons. I

The recent push towards a decentralized Web includes nameddata access at application-level and goes hand-in-hand with appendonly data repositories. Using the content’s hash as the data name is
common practice and is used for the versioning large data sets, see
e.g. dat

 A core element of SSB is
the so called “subjective reader” where each client reconstructs the
chat dialogue by extracting the relevant cross-linked posts (which
are referenced by the hash of the corresponding log entry) from the
logs of the involved peers. As an application-level ICN, SSB occupies an interesting position in the caching space: The append-only
logs are eagerly replicated in totality based on a binary decision
whether another peer’s log is (subjectively) relevant or not: A set
difference protocol is used in SSB to check for and retrieve changes
from a peer, in a fully receiver-driven fashion. Due to the eager
replication of the relevant logs, a chat dialogue is reconstructed
locally, without requiring network access.

2.4 End-to-end Encryption

3 CONFLICT-FREE REPLICATED DATA
TYPES (CRDT)

3.1 Auto-Merge Log (AMlog)

We have implemented an append-only log file abstraction (or “list”
abstract data type) as a CRDT in form of a directed acyclic graph
(DAG). Starting from the empty list node, each update appends a
“log node” that refers to the predecessor node via its hash name.
If two updaters append at the same time, two such nodes exist in
the graph, representing two different “branches”. Later, one of the
updaters, or a third party, can add a “merge node” that reduces the
two or more nodes to one again. These merge operations can also
happen in parallel, wherefore several rounds of merging may be
necessary if many log or merge nodes are appended in parallel
 Ultimately,
when no updates are being made and all merging is done, the DAG
has a single frontier node (called “tip” in IOTA). If the AMlogreading clients need a serialized view of the DAG’s entry, it suffices
to traverse the DAG in the same agreed-on order e.g. after running
a topological sort and breaking ties based on some node property
(e.g. hash value of its binary representation).

The AMlog is our central data structure that allows multiple
writers to append to a shared log without need to coordinate. If two
or more writers append concurrently, these additions are recorded
in the DAG and the existence of such branches will be made visible
through the HEAD service. How these branches are merged is
explained in the “Merging Name-Sets” paragraph of Section 4.4.

3.2 Observed-Remove Maps (ORmap)

4.1 Linked Encrypted Data, Data Structure
Encryption and Storage-level Encryption
Similar to Name-based Access Control [38] we use content encryption to prevent unauthorized access to content, but distinguish
between encrypting data content (e.g. fields of a chat message) vs
encrypting structure information (fields required to manage linked
list or index tables)

4.3 WORM
The WORM API, as shown in the previous paragraph, may surprise
at first view: The write() operation receives an opaque byte array
and returns the object’s name, which is a self-certifying name

Third, only by forcing the round-trip to the WORM service with its
reliability guarantee, can the end device be sure that the item has
been persisted and will survive even catastrophic system failures.
Finally, the service providers can keep track of write() requests
in order to handle “storage leak” which can happen if an end device creates a data chunk but crashes before being able to write a
reference to it into a higher-level data structure.

4.4 HEAD
we need
an explicit naming service that returns the latest version
 inspired by Git, we designed a “HEAD”
service that maps a name to a set of names. This requires some
explanation and careful distinctions:
. For objects that can be superseded by a more recent version,
we introduce the notion of a base name. For a list, an empty object
is created (including some nonce to make it unique) and the hash
of this object becomes the (self-certifying) base name. In Git, our
base name corresponds to a repo name. As end devices create new
versions and want to commit their changes to the system, they do
so by referring to the base name for which they register a commit
pointing to the precedent version

We use the HEAD service to collect these commits but the service
will do nothing to merge or even resolve conflicts. The clients can call append() for new commits, which in Git corresponds to creating
new branches. The second, and preferred method of a client to
interact with the HEAD service, is to call replace() which lets the
HEAD service remove a given name set and replace it by a single
new commit.

            service S2head {
            rpc getHead(s2wire.S2pointer) // base name
                returns (HeadSet);
            rpc append(s2wire.S2pointer, // base name
                s2wire.S2pointer) // name of commit
                returns (google.protobuf.Empty);
            rpc replace(s2wire.S2pointer, // base name
                HeadSet, // set of names to be removed
                s2wire.S2pointer) // name of replacing commit
                returns (google.protobuf.Empty);
            }

Merging Name-Sets: The reason to have a “name-to-set-of-names”
map (instead of a “name-to-name” map) is concurrency: Unlike
GitHub, we avoid making HEAD a serializing service (which would
have to decide which branch becomes the latest master branch).
For example, append()ing the same branch name to an AMlog is
idempotent due to the use of a name set. Concurrent replacement
requests with different removal sets can be done gracefully (i.e.,
execute a replace() even if a name in the removal set already has
been removed). The standard configuration will be that a AMlog reading client will see multiple branch names via the getHead()
method and will have to merge them at application level, effectively
replacing a set of names with a single name. If two merges are done
in parallel, two branch names will emerge which the next client
will have to merge, etc. Note that there is no remove() method because a base name always has to map to a non-empty set of names
(initially the base name itself).

We have built our HEAD service with the semantics as explained
above. Our hypothesis is that manipulating a set of names can be
made in a distributed fashion without (global) serialization. Given
this is true, the HEAD service can be implemented in a peer-topeer fashion among brokers where the peers’ current name sets
are only exchanged periodically and added (via union) to the local
name set e.g., using a gossip style protocol. Name replacement
operations would not be exchanged i.e., applications at other peers
will have to re-merge commits or remove superfluous names, before
the next gossip round. Note that such a HEAD-maintained map
(that temporarily associates a name with a set of names) is not a
CRDT as it is not able nor supposed to merge the accumulated state
by itself. An interesting reference point is Secure Scuttlebutt [33]
where the eager and gossip-based replication automatically leads
to the propagating of all log changes, hence provides an implicit
implementation of the HEAD service and notification.

Turning the database inside out
    - https://www.youtube.com/watch?v=fU9hR3kiOK0
    - https://www.confluent.io/blog/turning-the-database-inside-out-with-apache-samza/

    We are now using the database as a kind of gigantic, global, shared, mutable state. It’s like a global variable that’s shared between all your application servers. It’s exactly the kind of horrendous thing that, in shared-memory concurrency, we’ve been trying to get rid of for ages. Actors, channels, goroutines, etc. are all attempts to get away from shared-memory concurrency, avoiding the problems of locking, deadlock, concurrent modifications, race conditions, and so on.

    This separation of reads and writes is really the key idea here. By putting writes only in the log, we can make them much simpler: we don’t need to update state in place, so we move away from the problems of concurrent mutation of global shared state. Instead, we just keep an append-only log of immutable events. This gives excellent performance (appending to a file is sequential I/O, which is much faster than random-access I/O), is easily scalable (independent events can be put in separate partitions), and is much easier to make reliable.

5.1 End device “boot record”

Concurrent Log-Structured Memory
for Many-Core Key-Value Stores
- https://github.com/gtkernel/nibble-lsm
- http://www.vldb.org/pvldb/vol11/p458-merritt.pdf

- a log-structured key-value store called Nibble that incorporates a multi-head log for supporting concurrent writes, a novel distributed epoch mechanism for scalable memory reclamation, and an optimistic concurrency index
    1. Optimistically-concurrent index. A partitioned, resizable hash table for mapping application keys to virtual memory locations — it reduces contention for highly concurrent operations, and allows memory compaction to run concurrently with application threads.
        - We wish to avoid use of a sorted index as concurrent methods can add complexities to the overall design... Thus, our index implements an open-addressing hash table, with no per-bucket chaining, illustrated in Figure 4
        - Optimistic concurrency in our hash table uses the following design. Each bucket has a version counter, initialized to zero. Mutators lock the bucket by atomically incrementing the version by one (to become odd), and again incrementing upon completion (to become even). Readers will enter only when the version is even, recording the version to their thread stack. Upon extracting an entry, the version is read again; if both are equal, it means no update modified the bucket contents, otherwise the read restarts. Writers attempt to increment the version by one when it is even; doing so ensures entry by one mutator, and forces in-progress reads to abort and retry. With an odd value, readers and writers will wait, reading the version until it becomes even. This method of optimistic concurrency has been demonstrated in other systems, such as in OLFIT [13], Masstree [50], MICA [46], and OPTIK [30], and is used in Nibble for its simplicity and performance.
        - The index is able to grow....
    2. Partitioned multi-head logs. Per-socket log-structured memory managers with per-core log heads and socket-isolated compaction support
    3. Distributed hardware-based epochs. Thread-private epochs accessible from a shared table enable full concurrency between operations and with background compaction to quickly identify quiescent periods for releasing memory.



Corfu: A distributed shared log
    https://blog.acolyer.org/2017/05/02/corfu-a-distributed-shared-log/

    If we can construct a (suitably performant) linearizable shared log, then this primitive can be used as a key building block to solve some hard distributed systems problems: an agreed upon global ordering becomes simply the order of events in the log, and through state machine replication, this can be parlayed into consistent views of system state. Shared logs have been used for failure atomicity and node recovery, for consistent remote mirroring, and for transactional systems…

    very simple API to applications, consisting of append, read, trim, and fill operations


https://www.slideshare.net/ConfluentInc/power-of-the-loglsm-append-only-data-structures

- Write
    - batch: writes collected in mem
    - sorted memtable: when buffer fills, sort
    - flush: write to disk, a small immutable file
- Read
    - search reverse chronologically, basd worst case with many files
    - LSM optimizes writes over reads (the harder part)
    - Optimisation:
        - bound num of files, create levels
        - other thread merges older files deduping them (Compaction)
        - prefetch (think L1/L2 cachelines)
        - bloom filter
        - want to do binary search
- immutablity: simpler locking semantics, only memtable is mutable
- Event Sourcing
    - Journal of state changes
    - no update in place



- Merge Records
    - counters, like atomic, avoids read-modify writes

- possible for block-compression
    - zstd :)

## c recommendations

    https://wiki.sei.cmu.edu/confluence/display/c/2+Rules

## Exception handling
## setjmp
    https://wiki.sei.cmu.edu/confluence/display/c/MSC22-C.+Use+the+setjmp%28%29%2C+longjmp%28%29+facility+securely
    https://en.wikipedia.org/wiki/Setjmp.h

## fn

http://cs-fundamentals.com/c-programming/function-pointers-in-c.php

/* array-fp.c */
/* How to Use Arrays of Function Pointers */

        #include <stdio.h>
        
        int getSum(int, int);
        int getDifference(int, int);
        int getProduct(int, int);
        int getDivision(int, int);

        int main(){  
            typedef int (*functionPtr)(int, int);  // 'functionPtr' now can be used as type

            functionPtr arrayFp[4] = {getSum, getDifference, getProduct, getDivision};

            int a = 50, b = 20;
            printf("Sum of %d and %d : %d\n", a, b, arrayFp[0](a, b));
            printf("Difference of %d and %d : %d\n", a, b, arrayFp[1](a, b));
            printf("Product of %d and %d : %d\n", a, b, arrayFp[2](a, b));
            printf("Division of %d and %d : %d\n", a, b, arrayFp[3](a, b));
            printf("\nProduct of sum and difference of %d and %d is : %d\n", a, b, arrayFp[2](arrayFp[0](a, b), arrayFp[1](a, b)));
        }
        int getSum(int x, int y) { return x + y; }
        int getDifference(int x, int y){ return x - y; }
        int getProduct(int x, int y){ return x * y; }
        int getDivision(int x, int y){ return x / y; }


/* fp-arguments.c */
/* Pass a Function Pointer as an Argument */

        #include <stdio.h>
        #include <string.h>
        
        char* compareStrings(char*, char*, int (*cmp)(const char*, const char*));
        
        int main()
        {
            char str1[80], str2[80];
            int (*functPtr)(const char *, const char *); /* function pointer */
            functPtr = strcmp; /* assign address of strcmp to functPtr */
            printf("Enter two strings.\n");
            scanf("%s%s", str1, str2);
            
            /* pass address of strcmp via functPtr */
            printf("Are \"%s\" and \"%s\" equal? : %s\n", str1, str2, compareStrings(str1, str2, functPtr));
            
            return 0;
        }

        char* compareStrings(char *a, char *b, int (*cmp) (const char*, const char*))
        {
            if(!(*cmp)(a, b)) // !cmp(a, b) is also correct
                return ("YES");
            else
                return("NO");
        }

/* return-max.c */
/* Calling a Function Using Function Pointer */*

        #include <stdio.h>

        int retMax (int n1, int n2){ return (n1 > n2) ? n1 : n2; }

        int main () {
            int (*ptrMaxFunctin)(int, int);
            ptrMaxFunctin = retMax;
            int qty1 = 20, qty2 = 50;
            
            printf ("Max of %d and %d is : %d \n", qty1, qty2, (*ptrMaxFunctin)(qty1, qty2));
            return 0;
        }

        OUTPUT
        ======
        [root@host ~/cprogs]$ gcc return-max.c -o return-max
        [root@host ~/cprogs]$ ./return-max
        Max of 20 and 50 is : 50

How to typedef Function Pointer?

        typedef int (*ptrToFunct)(int, int);
        //now on ptrToFunct can be used as a type

        int retSum(int n1, int n2){ return n1 + n2; }

        ptrToFunct pFunct = retSum;
        ...
        int x2 = (*pFunct)(10, 20); // call retSum() to calculate 10+20
        ...


How to check if the number is integer or float?
    https://stackoverflow.com/questions/20068234/c-how-to-check-if-the-number-is-integer-or-float


## ideas

casm

aec
cae
    caesar