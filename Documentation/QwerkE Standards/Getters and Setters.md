

Conventionally, a GetThing() method would return a modifiable reference to a Thing instance.
This would mean that anyone trying to only read values on an object would be able to set properties. Perhaps using the SetThing() method to modify would be more safe. That being said, it goes against the standard to get something back from a setter, so most users may find this concept unintuitive, which I can understand.

The main reason I am considering changing this API "philosphy", is for myself. I find it confusing unsafe to have a Getter() that is modifiable as most of the time I just want to read values. If I want to write, then I should be explicit and use another "Getter" to have a mutable reference given to me, or pass my value into a Setter() method.

I'll try this out in QwerkE and see how it goes. More than anything, I just want a standard and I am not loving the "SeeThing()" API I have going on right now for constant references...