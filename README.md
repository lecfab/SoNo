# Sorting nodes in networks

Here is a C++ tool to read a graph and permute its nodes according to various orders. The idea is that some algorithms may have different time and space efficiency depending on the ordering of nodes.


## Installation & compilation
`$ git clone https://github.com/lecfab/sono.git`

`$ make ord DEBUG=1` to obtain a debugging executable

`$ make ord` to optimise the executable for production

## Running

`$ ./ord DATASET ORDER FILE`


#### Parameters
-   `DATASET`: a graph representation for nodes [0 to N-1]. It must be a text file where each line corresponds to a directed edge of the form `a b` (i.e. a SPACE b, with a and b long unsigned integers).

-   `ORDER`: currently available orders are optimise

    -   `original`:   the original order provided by the dataset (warning: it is usually not random)
    -   `rand`: random reordering of nodes
    -   `deg`:  sorted by decreasing total degree
    -   `deg+`: sorted by decreasing outgoing degree
    -   `deg-`: sorted by decreasing ingoing degree
    -   `core`: sorted by degeneracy ordering (k-core pealing algorithm)

#### Options
-   `-o FILE`: output file in which the order will be printed (the new ID of the node with the old ID u is on line u)
-   `-d`: flag to specify that the graph is directed (it CAN have both `a b` and `b a` edges)
-   `-u` (default): flag to specify that the graph is undirected (it CANNOT have both `a b` and `b a` edges). See below how to make a directed graph undirected.

Type `$ ./ord --help` for more information.

## Undirected graphs
If your INPUT graph is directed and have opposite edges (eg. both `a b` and `b a` are in the edge list), we provide a tool to make it undirected. The OUTPUT edge list will only contain `a b`, while `b a` not written to reduce file size.
Here is the way to do so:

`$ make undirect`

`$ ./undirect INPUT OUTPUT`

## Contributors

Fabrice Lécuyer (fabrice.lecuyer@lip6.fr)

Maximilien Danisch (<http://bit.ly/danisch>, maximilien.danisch@gmail.com)

2020
