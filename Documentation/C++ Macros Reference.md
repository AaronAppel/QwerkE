
# Stack Overflow "# [How to remove the enclosing parentheses with macro?](https://stackoverflow.com/questions/24481810/how-to-remove-the-enclosing-parentheses-with-macro)"
\#define ESC(...) \_\_VA\_ARGS\_\_
MY_MACRO( ESC(A<int, double>), text );
and the Mirror library fix for BGFX enforcing compiler conformance
```cpp
#define _Args(...) __VA_ARGS__
#define STRIP_PARENS(X) X
#define PASS_PARAMETERS(X) STRIP_PARENS( _Args X )
```
Adding () parentheses to all user facing calls but in an indirection macro
INDIRECTION_MACRO(TYPE) ORIGINAL_MACRO((TYPE))
then wrap TYPE in PASS_PARAMETERS(TYPE) wherever it is used INSIDE an expansion (not in another \#define)
# Stack Overflow "# [Removing extra parentheses from C++ macro](https://stackoverflow.com/questions/43809796/removing-extra-parentheses-from-c-macro)"
\#define IGNORE\_(ignore,...) \_\_VA\_ARGS\_\_
\#define IGNORE(...) IGNORE\_ \_\_VA\_ARGS\_\_

# Chat GPT google search "c++ macro remove parentheses"
To handle commas within macro expansions, especially when dealing with sequences or lists, you often need to ensure the comma is treated as part of the macro's argument rather than as a delimiter. Techniques like using parentheses, token pasting (##), or specialized macro functions (like BOOST_PP_SEQ_ENUM in Boost) can help. 
Here's a breakdown of common approaches:
1. Parentheses for Grouping:
If the comma is within a macro argument that's expected to be a single entity (like a template argument list), enclosing it in parentheses can prevent the preprocessor from interpreting it as a separator. 
C++

#define MACRO(arg) do_something(arg)
#define TEMPLATE_MACRO(T) template <typename T> void some_function()

// Example usage:
MACRO((int, double));  // The comma is part of the argument (a tuple)
TEMPLATE_MACRO(std::pair<int, int>); // The comma is part of the template argument
2. Token Pasting (##):
The token-pasting operator ## can be used to combine tokens, potentially including commas, into a single token. This can be useful when you need to build up a larger token from smaller parts. 
C++

#define CONCAT(a, b) a##b
#define COMMA ,
// Example:
CONCAT(int, COMMA int)  // results in 'int,int'
3. Specialized Macro Functions (e.g., BOOST_PP_SEQ_ENUM):
Libraries like Boost provide preprocessor utilities that handle comma-separated sequences. For example, BOOST_PP_SEQ_ENUM can be used to generate a comma-separated list from a sequence. 
C++

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#define PROCESS_MEMBER(r, data, elem) data elem
#define MEMBERS (x)(y)(z)

#define MY_STRUCT(name) \
struct name { \
    BOOST_PP_SEQ_ENUM(MEMBERS) ; \
    name() : BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(PROCESS_MEMBER, . , MEMBERS)) {} \
}
4. Escaping Commas:
In some cases, you might need to escape the comma using a backslash (`\`) to prevent it from being interpreted as a separator. This is less common and often less desirable than the other methods. 
C++

#define MACRO(arg) some_function(arg)
// Example:
MACRO(a\,b) // The backslash escapes the comma.
5. Recursive Macro Expansion:
In more complex scenarios, you might need to use a series of macro expansions to achieve the desired result. This often involves creating intermediate macros to handle specific parts of the process. 
