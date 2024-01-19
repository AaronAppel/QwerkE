
## Switch Statements
Visual studio will indent the braces to match the case statement. I prefer another level of indentation here, to place the {} braces in line with the break. Otherwise the braces will be inline with the switch statement which I prefer to avoid, purely for visual preference.

```cpp
switch (enumType)
{
case 0:
    {
	    int var = 1;
    }
    break;
}
```