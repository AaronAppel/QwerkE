Extend assert to include function name, file name, and file line info to asserts, and potentially log statements as well.

Copied from QF_Log.h
// #TODO Try to expand the formatted string argument to include more information (function, file, line)
// #TODO Create a method to increment/replace formatting string indices using variadics NUMARGS(__VA_ARGS__)
// constexpr char formattedMessage[] = "Assert! {X}, {0}, {X}, in {X}() in {X}({X})"; // Int indices for user args, and Xs for expansion (order dependent)
// \#define ASSERT_EXT(x, msg, ...)   if ((x)) { } else { LOG_CRITICAL(formattedMessage, FormatString(msg, __VA_ARGS__), formattedMessage, \#x, __FUNCTION__, __FILE__, __LINE__); BREAK }

Need a way to format the arguments in the QF_Log.h string, and then pass a pre-formatted string to LOG_CRITICAL() with more args to format.
Look at this post related to std::format and fmt : https://stackoverflow.com/questions/554063/how-do-i-print-a-double-value-with-full-precision-using-cout