Name: Ryan Espiritu

USC ID: 9340109175

Email: raespiri@usc.edu

To compile type "make Scrabble"

To run type "./Scrabble config.txt"

To use an AI type CPUS/cpus or CPUL/cpul

*NOTE: I used my own codebase for this project not the one provided

*NOTE: when init file is present, I am unable to account for the differing 
values of same lettered tiles (i.e. if the init-file had tiles o-10 and o-2 
where 10 and 2 are the respective scores of letter 'o', my program cannot 
keep track of these two values only one) Thus all my "o" tile score values 
would be 10. This in turn will affect my AI's decisions

*NOTE: My scrabble codebase does have some bugs like accounting for forming
2 or more words with one "place" move, thus my AI's may make incorrect 
decisions due to this (like occasionally placing words in incorrect spots).
However I believe my AI and backtracing logic to be correct.

