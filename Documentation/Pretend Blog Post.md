
#TODO Overview the process of adding new QwerkE serialization system.

Commits :
- https://github.com/AaronAppel/Qwerk_Engine/commit/3d685712ae7740c830954dc6cec105c93b2e5a5d
- https://github.com/AaronAppel/Qwerk_Engine/commit/4b40b0ddb95a65d30dca4f7af11ee22fea05c035
- https://github.com/AaronAppel/Qwerk_Engine/commit/9e121635978e1b80d7216ca34336d2d379a81ce4

## The problem without a solution
Scoured internet, in the past and more recently. Couldn't find good answers, aside from using Boost library.

Found blogs, and eventually found a blog post that looked promising. Couldn't get the code to work.
For fun, tried "massaging" the code in a new, sand box app. Eventually got it compiling, although not functional. Figured out my errors and got it working with restrictions, kept going, and going, and bam! Got the example code working with simple types.

## Onto QwerkE: To QwerkE, and beyond!

Started by getting a Thing class to compile. then tried adding existing classes and types.
- Created a new Reflection namespace
- Created a new Serialization namespace
- Extended types to handle more than just ints
- Used templating to gain more indirection
- Went raw with memory reading and writing/assignment
- Renamed or refactored existing code
- Replaced existing code

## The troubles
- Ran into naming collisions with primitive types (int, bool, float, double)
- Can't have macros in Reflection namespace
- Memory assignment required new understanding of pointers
- Frustrating times when doing the correct operations, but on the wrong type (string, not const char*)
## For future me
- Try out layout structs to see how easy it is to change how serialized files are written to, or where objects are placed in the JSON hierarchy
- Expose private members using a Serialization friend class macro
- Make macros more convenient to use
#TODO Split serialization into a new library or re-usable code file(s).


## Results
Overall, I feel pretty proud of myself. The initial source code was a true inspiration, but it also took a lot of effort from me, gaining new information and mastery over serialization to get everything together. I am more confident in myself and I know that the next seemingly insurmountable mountain  of a problem, will be one stone smaller than before.