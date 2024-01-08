Try to make a hard and fast rule for inheritance depth. Deep inheritance trees are often signs of lacking design.
By using other interfaces, namespace, or code restructuring, deep inheritance can be reduced or avoided.

One concern with deep inheritance is code extension and maintenance as the system becomes rigid and ugly (for lack of a better term) as new functionality can be more complex to implement and more classes get affected than needed for changes not directly relating to them in the first place.