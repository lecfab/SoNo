# Sorting nodes in networks

Here is a tool to read a directed graph and permute its nodes according to various orders. The idea is that some algorithms may have different time and space efficiency depending on the ordering of nodes.

## Compilation
`$ make ord DEBUG=1` to obtain a debugging executable

`$ make ord` to optimise the executable for production

## Running
`$ ./ord DATASET ORDER FILE`

* DATASET: a graph representation for nodes [0 to N-1]. It must be a text file where each line corresponds to a directed edge of the form `a b` (ie a SPACE b, with a and b long unsigned integers).

* ORDER: currently available orders are
  * `no`:   the original order provided by the dataset (warning: it is usually not random)
  * `rand`: random reordering of nodes
  * `deg`:  sorted by decreasing total degree
  * `deg+`: sorted by decreasing outgoing degree
  * `deg-`: sorted by decreasing ingoing degree
  * `core`: sorted by degeneracy ordering (k-core pealing algorithm)

* FILE: text file in which the order will be printed (the new ID of the node with the old ID u is on line u)

## Contributors 

Fabrice Lécuyer (fabrice.lecuyer@lip6.fr)

Maximilien Danisch (http://bit.ly/danisch, maximilien.danisch@gmail.com)

2020
