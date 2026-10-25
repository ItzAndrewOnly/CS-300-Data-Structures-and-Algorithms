# CS 300 Project One: Pseudocode and Runtime Analysis

## Pseudocode — Milestones One, Two, and Three

Course = {number, title, prerequisites[]}. Shared routines below run separately with a vector, a chained hash table, or an unbalanced binary search tree (BST). Keys are trimmed and uppercase. Include MATH201 because it belongs to the supplied program pathway.

```text
LOAD(filename, type): Open file; if opening fails, display error and return FAILURE.
  Create empty temporary structure S of type; FOR each line in file:
    Split line by commas; trim fields; uppercase course and prerequisite numbers.
    IF fewer than 2 fields, any empty field, invalid number (letters then digits),
       or FIND(S, number) exists: close file; report line error; return FAILURE.
    Create Course(fields[0], fields[1], fields[2 onward]); INSERT(S, Course).
  Close file. FOR each course in S: FOR each prerequisite number in course:
    IF prerequisite equals course.number OR FIND(S, prerequisite) is missing:
      report invalid prerequisite; return FAILURE. Return SUCCESS with S.

VECTOR INSERT: Append course. FIND: Scan courses; return matching course or NONE.
HASH INSERT: Hash number to bucket; prepend course to its linked chain; grow and
  rehash when load factor exceeds 0.75. FIND: Hash key; scan its chain; return match or NONE.
BST INSERT: From root, compare numbers; move left if smaller, right if larger;
  attach new node at empty position. FIND: Follow these comparisons to match or NONE.

LIST(S): Vector: merge-sort a copy by number, then print each number and title.
  Hash: collect all bucket entries; merge-sort by number; print each number and title.
  BST: INORDER(node): if node exists, visit left; print number/title; visit right.

DETAIL(S, key): course = FIND(S, uppercase(trim(key))); if NONE, print “Not found.”
  Otherwise print course number/title; if no prerequisites, print “None.”
  FOR each prerequisite: use FIND(S, prerequisite) and print its number and title.

MENU(type): loaded = FALSE; REPEAT display “1 Load, 2 Course list, 3 Course, 9 Exit.”
  Read choice. IF 1: request filename; result = LOAD(filename, type).
    On SUCCESS, replace active structure with result.S; loaded = TRUE.
    On FAILURE, keep existing data and loaded status unchanged.
  ELSE IF 2 or 3: if not loaded, display “Load data first.”
    Otherwise, if 2 call LIST; if 3 request course number and call DETAIL.
  ELSE IF 9: display “Goodbye.” ELSE display “Invalid option.” UNTIL choice = 9.
```

## Runtime chart — loading and creating objects only

Let n be courses and p be total prerequisite entries. For the n-only chart, assume bounded line length and prerequisites per course, so p = O(n). Count assignments, comparisons, and loop controls as 1. Function calls use their runtime. Counts describe a valid file; errors can stop earlier.

| LOAD step / code | Line cost: V / H / BST | Executions | Worst total |
| --- | --- | --- | --- |
| Open, initialize, close, return | 1 each | 1 each | O(1)* |
| Line-loop test; read/split/trim/check fields | 1 each | n+1; n each | O(n) |
| FIND duplicate; check result | O(n) / O(n) / O(n); 1 | n each | O(n²), all |
| Create Course object | 1 | n | O(n) |
| INSERT course | O(1) amortized / O(n) / O(n) | n | O(n) / O(n²) / O(n²) |
| Course-loop test; prerequisite-loop test | 1 | n+1; p+n | O(n+p) |
| Self-check; FIND prerequisite; missing-check | 1; O(n) all; 1 | p each | O(pn+p) |
| Overall worst-case LOAD | V: O(n²); H: O(n²); BST: O(n²) | — | With p = O(n) |

*Hash bucket initialization totals O(n) if n-sized capacity is preallocated; geometric growth also uses O(n) space. The vector’s occasional resize costs O(n), but all appends together cost O(n). Duplicate and prerequisite checks make this loader O(n²) for all three structures; this is not insertion cost alone. Without bounded fields, include character-processing cost and retain p in O(n² + pn).

## Advantages, disadvantages, and memory

Vector: Easy to build and understand, with little extra storage. Its downside is linear searching. Option 2 takes O(n log n) for merge sort plus O(n) printing. Course lookup takes O(n). Storage is O(n+p); the sorted copy and merge buffer add O(n) temporary space.

Hash table: Usually finds a course in expected O(1), which helps repeated searches. However, collisions can make lookup O(n), and buckets/chains add overhead. Option 2 must gather and sort entries, taking O(n log n). Storage is O(n+p), with O(n) temporary sorting space. Expected loader time is O(n+p), but its worst case remains quadratic.

BST: Inorder traversal prints courses already sorted in O(n). Searching takes O(h), where h is tree height. A roughly balanced tree has O(log n) height, but sorted insertion can create a chain and O(n) searches. Node links add overhead. Storage is O(n+p); traversal uses O(h) stack space, up to O(n).

## Recommendation

I would use a vector for this small advising program. All three loaders have the same O(n²) worst case with these validation checks, so no structure wins that comparison. A vector is simpler to code and stores the eight supplied courses without bucket or node overhead. Sorting is O(n log n), and linear lookup is reasonable here. A hash table would be more useful for many repeated searches; a BST offers sorted output but needs balancing to guarantee fast searches.
