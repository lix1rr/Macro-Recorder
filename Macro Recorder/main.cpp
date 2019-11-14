#include <windows.h>
#include "shortcuts.h"


int main(int argc, char* argv[]) {
	if (argc < 3) { return -1; }
	if (std::strcmp(argv[1], "create") == 0) {
		bool verbose = false;
		bool saveToFile = false;
		bool runAfter = false;

		for (int i = 2; i < argc; i++) {
			if (strlen(argv[i]) == 2) {
				if (argv[i][0] == '-') {
					switch ((char)std::tolower(argv[i][1])) {
						case 'v':
							verbose = true;
							break;
						case 'f':
							saveToFile = true;
							break;
						case 'r':
							runAfter = true;
							break;
						default:
							std::cout << (char)std::tolower(argv[i][1])  << 'v' <<std::endl;
							std::cout << "Invalid argument: \"" << argv[i] << "\"";
							exit(-2);

					}
				}
			}
		}
		std::cout << (verbose ? "verbose" : "not verbose") << std::endl;
		std::cout << (saveToFile ? "saveToFile" : "not saveToFile") << std::endl;
		std::cout << (runAfter ? "runAfter" : "not runAfter") << std::endl;

		Shortcut s = Shortcut::parse(std::string(argv[2]));
		std::cout << "Macro sequence:" << std::endl;
		for (int i = 0; i < s.actions.size(); i++) {
			Action* action = s.actions.at(i);
			if (action != nullptr) {
				std::cout << i << ": " + action->toString() << std::endl;
			}
			else {
				std::cout << i << ": NULL" << std::endl;
			}
		}
		while (true) {
			for (int i = 0; i < s.actions.size(); i++) {
				Action* action = s.actions.at(i);
				if (action != nullptr) {
					action->execute(verbose);
				}
			}
		}
	}
}

Shortcut Shortcut::parse(std::string str) {
	std::vector<Action*> actions;
	bool timeToken = false;
	std::string time = "";
	for (char c : str) {
		if (timeToken) {
			if (c == 'T') {
				timeToken = false;
				actions.push_back(new WaitAction(std::stoi(time)));
				
			} else if ( (c >= '0') | (c <= '9') ) time.append(std::string(1, c));
		} 
		else if (c == 'T') {
			timeToken = true;
		}
		else if (c == 'S') {
			actions.push_back(new KeyAction(' '));
		}
		else {
			actions.push_back(new KeyAction(c));
		}
	}
	Shortcut s;
	s.actions = actions;
	return s;
}

void KeyAction::execute(bool verbose) {
	for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
		if (IsWindowVisible(hwnd)) {
			int length = GetWindowTextLength(hwnd);
			if (length > 0) {
				char* title = new char[length + 1];
				GetWindowTextA(hwnd, title, length + 1);
				if ((title != "Program Manager") && (title != "Settings") && (title != "Microsoft Store")) {
					PostMessage(hwnd, WM_CHAR, key, 0);
					if (verbose) {
						std::cout << "Sent " << key << "  typed message to " << title << std::endl;
					}
				}
			}
		}

	}
}
