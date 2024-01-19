## Virtual Methods
Parent declarations should use "virtual" before the return type, while child implementations should use "override" after the declaration.

## Numbers
### Floating Point Numbers
When assigning values, omit any decimal places with the value of 0, so an assignment of 1.0f can simply be written as 1.f instead.

## References
#### Primitive Types
When returning a value, it's okay to return by value as primitive types are small. Could consider larger primitive types to require const references in the future.

// TODO: Research and decide on coding standards.
#TODO Watch and take notes :
## how Google writes gorgeous C++
- [ ] https://www.youtube.com/watch?v=6lU11IHfJgo&ab_channel=LowLevelLearning
- [ ] https://google.github.io/styleguide/cppguide.html#Nonstandard_Extensions
PDF : https://docs.google.com/file/d/0B5FQoJdw1sdRVHRid21IalRRb3M/edit?resourcekey=0-0OIuS0JpAEz34vYFETmWUQ
## how NASA writes space-proof code
- [ ] https://www.youtube.com/watch?v=GWYhtksrmhE&ab_channel=LowLevelLearning


\*Forced \#includes are currently necessary but should be deprecated in the future unless they can circumvent [[Additional Includes]].
#### Rules :
- Make sure the solution or project(s) works if [[Precompiled Headers]] are turned off.
- Only include necessary files in all \#includes