# Test task for c system programmer position

## Overview
The task was to write tool to combine two dumps of data. Each data packet contains id filed. We needed to maintain unique ids in resulted array, removing duplicates in JoinDump().
I have achieved this by utilizing hash table, that I wrote for this porpuse. 
Hashtable is based on array of items(each item stores linked list of collisions) and uses FNV-1a hashing function to distribute items over tables. 
The novelty I use is merging on collision via function
> ht_merge_key(table, key, value, mergeCallback)  

When I detect collision in JoinDump() - I traverse list of collisions and combine similar 
I DO NOT resize hashtable. O(hashtable size) in JoinDump is length of first and second arrays summarized.

Alternative algorithms would be 
1) Brute force ->  O(n^2)
2) Sort both arrays and use two pointers algorithm -> O(n*log(n))*2 + O(2n). 
This is great in case we sort before storing, it also allows us to utilize parallel algorithms. But hashtable approach is steel better in my opinion
3) Hash table (thats what I use) - O(n) + O(n)

Note: Before writing dump in StoreDump() I write its size, for additional safety and managment

## Build instructions

### Your build system will create 3 executables
- prepareDumpForProcessor -> Will store two dumps at data1.bin and data2.bin
- statDataProcessor -> this is tool that takes as arguments in order 1) first dump path 2) second path 3) return path
- fastTest -> for hardcoded unit tests
- randomTestUtil -> for randomized testing
>mkdir build
>cd build
>cmake ..
>make
## Execute utils and tests
### Prepare test dumps
>./prepareDumpForProcessor 
 ### Tests on those prepared dumps
>./statDataProcessor data1.bin data2.bin dataRes.bin (*this will load and join those dumps, sort and print them*)
>Data 0: id=7, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 1: id=a, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 2: id=4, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 3: id=b, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 4: id=5, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 5: id=8, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 6: id=9, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 7: id=6, count=1, cost=1.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 8: id=3, count=2, cost=2.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 9: id=1, count=2, cost=2.000000e+00, primary=n, mode=00000000000000000000000000000011
>Data 10: id=2, count=2, cost=2.000000e+00, primary=n, mode=00000000000000000000000000000011
### Runt hardcoded unit tests
>./fastTest (*this will run hardcoded tests and print their time of exec*)
>testCase1() passed
Time taken: 433.000000 microseconds
testCase2() passed
Time taken: 294.000000 microseconds
testCase3() passed
Time taken: 257.000000 microseconds
testCase4() passed
Time taken: 316.000000 microseconds
End of fast test
### Run randomized tests
>valgrind --leak-check=full ./randomTestUtil
>Iteration 1000/1000:
Running TestHashTableMergeKey()....
Running TestHashTableInsertSearch()....
Running TestJoinOnRandomData()...
Running TestJoinInCaseInitialArraysContainDuplicateIds()....
This iteration Tests: 100005000, Passed: 100005000, Failed: 0
Total Tests: 100005000, Passed: 100005000, Failed: 0
