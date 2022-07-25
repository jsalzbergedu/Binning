The purpose of this repository is to collect notes, papers, ideas, and code for improving the compilation
of control flow structures using binning.

Project goals:
Rewriting iterated conditionals to use binning networks knowing a bound/ratio on the number of times each branch is taken,
and counting/bounding the number of times each branch is taken automatically

# Rewriting Iterated Conditionals to Bin
We wish to rewrite iterated conditionals, knowing the number of true and false cases, to binning constructs.
## Tasks
- For this Zach envisions a new compiler -- one that allows us to both split witness generation and circuit checking, as well as one that allows for the program transformations we desire.
## Papers

# Counting/Bounding the number of times each branch is taken
We wish to, as an extension of the project, bound the number of times each branch is taken.
## Tasks
## Papers
- [ ] Florian Zuleger's work (ranking functions)
- [ ] Sumit Gulwani's work (instrument + count)
- [ ] Hoffman's work (relational, multivariate)
- [ ] Work on nonconvex invariants (incl. Monniaux & Gonnard, Zuleger)
