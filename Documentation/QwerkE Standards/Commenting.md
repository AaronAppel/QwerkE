Minimal commenting is preferred. Ideally, symbol naming is used to communicate the purpose of a field, or the behaviour of a function.

Comments can be used to offer additional information when simply reading the immediately relevant code still misses key information. An example may include a potential bug tat can happen in another location (like in data for instance) when a seemingly harmless operation is performed. A function may be observed to have no negative effect while debugged, but cause an issue later in execution making it hard to point point the issue. **\#NOTE** comments should be used for purposes like these, to clarify the purpose of the comment.

\#endif pre-processor directives sometimes generate a comment afterwards, to match the \#if case. The comment doesn't always automatically update when the \#if is changed, so remove any comments in line with \#endif directives.