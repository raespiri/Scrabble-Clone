Name: Ryan Espiritu

USC ID: 9340109175

Email: raespiri@usc.edu

To compile Counting.cpp type "make counting"

To run type "./counting [input] [output] x d r"

Which size of input cases did you use?
~100, ~1.2k, ~11k, ~119k, and ~1mil words

For each of the three collision resolution approaches (linear probing, 
quadratic probing, double hashing), report how long each of your input cases 
took. How long did it take per operation?
~100 words
linear: 0.000404s
quadratic: 0.000411s
double hashing: 0.000806s

~1.2k words
linear: 0.004221s
quadratic: 0.004797s
double hashing: 0.009538s

~11k words
linear: 0.029737s
quadratic: 0.030406s
double hashing: 0.058133s

~119k words
linear: 0.226678s
quadratic: 0.233358s
double hashing: 0.402766s

~1mil words
linear: 2.76045s
quadratic: 2.77629s
double hashing: 5.34913s

Explain why you think the results turned out the way they did.
I believe that the linear and quadratic functions had relatively similar run
times since they were each composed of a single hash function which at times
was incremented by a factor or i or i^2. On the otherhand the double hashing
technique roughly took twice as long as the linear and quadratic techniques.
This makes sense as two hash functions were used in this method which 
effectively doubled the runntime in cases where a key needed to by hashed
twice.

How do you think the running time would compare to each version of probing 
if you were to implement your Map in the following fashions? Briefly justify 
each, although you do not need to quantify how much slower/faster with any 
specificity.

Unsorted List
-Insert(x) would be faster than my versions of collision avoidance as it 
would run in O(1) since you could just push a new key to the back of the list 
whereas insert with a hashtable has a worst case of O(n)and an average of O(1)
-Find(x) would run in O(n) in the worst case which is much slower than my
hash table implementation which has an average of O(1)

Sorted list
-Insert(x) would run in O(n) since it is an ordered list, making it much
slower than my hashtable.
-Find(x) would run in O(log n) if binary search was implement which is better
than the worst case of my hashtable but slower than the average 

Binary search tree, non-rotating variety
-Both insert(x) and find(x) have a worst case of O(n) and an average of
O(log n) which is slower than my hashtable

Binary search tree with AVL balancing
-Both insert(x) and find(x) have O(log n) for the worst case and average
case, making my hashtable faster in the average case with it's runtime of
O(1)


