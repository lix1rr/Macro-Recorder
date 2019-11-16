const char* HELP_MESSAGE = 
R""""(Macro-Recorder usage:
macro.exe [mode] [options]
[mode]:
  macro [arguments/commmands]
    Each argument must be seperated by a space.

	Arguments:
		-v                         verbose
		-r                         run afterwards
		-f=\"path/to/file.macro"   save to file
		-s=\"name\"                save for name
		-w=\"window1;window2\"     windows to run on (default = all)
								   all windows must be seperated by a semicolon
	Commands:
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
  list windows [no options]
    Lists all windows with their associated number
  list macros [no options]
    Lists all the macros' names
  delete [name1] [name2] [name3] ...
    Delete macros with the name (space seperated)
  help [no options]
    Displays this help page

)"""";