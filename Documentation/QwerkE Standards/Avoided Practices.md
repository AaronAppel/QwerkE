**Just a section of "no-nos" I *try* to avoid**

## External Library Source Files
DO NOT MODIFY EXTERNAL SOURCE FILES!!

Sometimes libraries will have dated code that more modern compilers don't like. These issues will likely come in the form of compiler warning. In this case, try to wrap \#includes with \#pragma warning( disable : warning_number ) and see if that appeases the compiler. If not, then the next step would be looking into adding specific ignore entries in the project file.

For enums.h and imgui.h extension, I elected to use a QC\_\* header file and add new logic in there. examples of extended logic include macros for convenience, and helpful constants that may be missing from the original library source files.