#include <windows.h>
#include "shortcuts.h"

int main(int argc, char* argv[]) {
	if (argc < 3) { return -1; }
	if (std::strcmp(argv[1], "create") == 0) {
		bool verbose = false;
		if (argc > 3) {
			if (std::strcmp(argv[2], "\\v")) {
				verbose = true;

			}
		}
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
