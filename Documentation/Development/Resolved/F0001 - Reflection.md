Status : Implemented
Resolved using [[Mirror Library]]  and [[Serializer]] libraries.

By accepting a ClassInfo*, instead of using a templated type, we are able to do type based serialization at runtime without knowing the type at compile time.

A potential \#Feature could be to collect and maintain a list of all types of ClassInfo at compile time, or at program startup that can dynamically fetch type information when needed anywhere at anytime. This could remove the need of type specification and templating, giving a truly type independent work flow.

This may be beneficial, or I may spend time implementing a system, only to find out it isn't as practical as I initially thought. Should still be a good exercise in systems design.
