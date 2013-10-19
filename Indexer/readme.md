Paul Jones
Andrew Moore

Indexer Notes and Big-O Analysis  
================================

Design
------

The program operates by creating a sorted list of word nodes,
populating it, and then writing the populated list to the 
inputted filename.

Populating the list involves recursing over the inputted
directory to find every file, parsing every token from the
file, and inserting them in a sorted list.

Inserting a token into a sorted list happens by attempting
to find the token first, and if it does not exist, inserting it in
its correct position, if it does exist, either incrementing
the count in the case of the token being within the same file
or inserting into another sorted list if it is a new file
with the same token.

Printing then operates by traversing the populated list.

Analysis
--------

Creating a sorted list is constant time.

Populating the structure is O(n^2), as the worst case is that every
word is different and lexicographically following the previous word.

Traversing and printing to the file is constant time. 