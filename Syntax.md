# Using Macro-Recorder
## Marco writing syntax
To make a macro, you separate all commands and arguments by a space.
In this application you can save your macros by name (no specific directory), and you can access it from anywhere. If you want to save it to disk, there is an option to save to a file with a `.macro` extension.

**Usage:**
```

macro.exe [mode] [options]
[mode]:
  macro [arguments/commmands]
    Each argument must be seperated by a space.
  list-windows [no options]
    Lists all windows with their associated number
  list-macros [no options]
    Lists all the macros' names
  delete [name1] [name2] [name3] ...
    Delete macros with the name (space seperated)
  help [no options]
    Displays help page

```

**Arguments (only for the mode ``macro``):**
```
-v                         verbose
-r                         run afterwards
-f=\"path/to/file.macro"   save to file
-s=\"name\"                save for name
-w=\"window1;window2\"     windows to run on (default = all)
                           all windows must be seperated by a semicolon
```
**Commands (only for the mode ``macro``):**
```
keyboard down  "characters" = kd"characters"
keyboard up    "characters" = ku"characters"
keyboard typed "characters" = k"characters"

left mouse button down      = m1d
left mouse button up        = m1u

right mouse button down     = m2d
right mouse button up       = m2u

middle mouse button down    = m3d
middle mouse button up      = m3u

scroll up by 1 line         = msu1
scroll down by 1 line       = msd1

wait 50  miliseconds        = w50
```


**Listing windows**:

If you want to run the macro on specific windows, you can run
`macro.exe list windows`.
Which might output:
```
> macro.exe list windows
Listing Windows:

Window : Name
000001 : Google Chrome
000002 : Microsoft Word
000003 : Mozilla Firefox
000004 : Commmand Prompt

```
And you might run the macro like this:
```
> macro.exe macro -s="clicker" w="1;3"
```
