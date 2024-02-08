A serialization library, built on top of the [[Mirror Library]] reflection system.

The true power of Serializer comes from type indirection, supplied by the [[Mirror Library]]. This allows Serializer to treat data and code type agnostically, meaning it just concerns itself with the bytes being read and written.

Serializer can support primitives, collections, and complex types (structs/classes) without the user needing to explicitly specify how each should be handled. This means that new code values are super easy to add and remove, plus, changes like adding or removing values doesn't require manual/tedious effort and avoids several bugs and human error pitfalls.

From code :

// #TODO Make a note of key/value naming. Variable names should be used when possible.
// This is safer when using string matching. For primitives and complex types, variable
// names should be available. For array elements, the type name will do fine.