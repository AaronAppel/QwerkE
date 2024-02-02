
Namespaces can be distinguished by leaving the opening '{' brace on the same line as the declaration like so :
```
namespace QwerkE {
```

#TODO Update domains after moving all files to Root/Source/(Common, Framework, Framework)

Currently the entire Qwerk framework and engine are using namespace QwerkE. The main purpose is to avoid naming collisions in the future, for user code. Another work around could be to use prefixes like QF_* and QE_* before symbols like functions, classes, types, etc, for Qwerk specific logic. The extra indentation isn't a huge deal, but could be avoided for preference. The prefixes could also relate to the area/module/section/domain of code it lives in, giving some extra information.

## Domains
Common :
Framework :
Engine :
Platform :