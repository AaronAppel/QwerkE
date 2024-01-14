

## Additional Include Path Dependency Injection
Many libraries require modifying a project's [[Additional Includes]] list to point to either the "Libraries/" folder, or further down into the library specific folder. This is inconvenient but should be easily manageable using a build system. Ideally only the "Libraries" folder exists in the [[Additional Includes]] section so QwerkE files can reference library source files with nicer paths.

## Extending Libraries
When looking to extend a libraries source code, instead of manipulating the source files directly which is [[Bad Practice]]