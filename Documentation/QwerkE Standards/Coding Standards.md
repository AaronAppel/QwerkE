
# Inheritance
### Virtual Methods
Parent declarations should use "virtual" before the return type, while child implementations should use "override" after the declaration.


# Numbers
### Floating Point Numbers
When assigning values, omit any decimal places with the value of 0, so an assignment of 1.0f can simply be written as 1.f instead.

# References
### Primitive Types
When returning a value, it's okay to return by value as primitive types are small. Could consider larger primitive types to require const references in the future.