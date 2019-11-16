#include <windows.h>
#include "shortcuts.h"
#include "help.h"
#include <sstream>



int smain() {
	char* args = GetCommandLineA();
	
	std::stringstream ss(args);
	std::string s;
	std::vector<char*> argv;
	while (std::getline(ss, s, ' ')) {
		argv.push_back((char*)s.c_str());
		std::cout << (char*)s.c_str() << " ";
	}
	
	std::cout << std::endl;

	for (char* str : argv) {
		std::cout << str << " ";
	}
	std::cout << std::endl;
	
	//start(argv.size(), argv.data());
	return 68;

}


int main(int argc, char* argv[]) {
	/*
		Putting no arguments terminates program with message.
	*/
	if (argc < 2) {
		std::cout << "Invalid usage, type 'help' for information." << std::endl;
		return -1;
	}
	/*
		All the flags.
	*/
	bool verbose = false;
	bool saveToFile = false;
	bool runAfter = false;
	std::vector<Action*> actions;
	std::string word(argv[1]);

	/*
		Checking which mode the user specified.
	*/
	if (word.compare("macro") == 0) {
		for (size_t i = 2; i < argc; i++) {

			if (strlen(argv[i]) == 2) {
				if (argv[i][0] == '-') {
					switch ((char)std::tolower(argv[i][1])) {
						case 'v':
							verbose = true;
							continue;
						case 'f':
							saveToFile = true;
							continue;
						case 'r':
							runAfter = true;
							continue;
						default:
							std::cout << "Invalid argument: \"" << argv[i] << "\"";
							exit(-2);
					}
				}
			}
			{
				std::string str(argv[i]);
				size_t length = str.length();
				if (!(length >= 1))
					continue;
				switch (str.at(0)) {
					case 'k':

						if (!(length >= 4)) {
							std::cout << "Invalid command: \"" << str << "\"." << std::endl;
							exit(-2);
						}
						if (str.at(1) == '*' && str.at(length - 1) == '*') {
							KeyAction::generate(KeyAction::Type::Typed, str.substr(2, length - 3), &actions);
						}
						else {
							if (!(length >= 5)) {
								std::cout << "Invalid command: \"" << str << "\"." << std::endl;
								exit(-2);
							}
							if (str.at(1) == 'd') {
								if (str.at(1) == '*' && str.at(length - 1) == '*') {
									KeyAction::generate(KeyAction::Type::Down, str.substr(3, length - 2), &actions);
								}
							}
							else if (str.at(1) == 'u') {
								if (str.at(1) == '*' && str.at(length - 1) == '*') {
									KeyAction::generate(KeyAction::Type::Up, str.substr(3, length - 2), &actions);
								}
							}
							else {
								std::cout << "Invalid command: \"" << str << "\"." << std::endl;
								exit(-2);
							}
						}
						break;
					case 'm':
						if (!(length >= 3)) {
							std::cout << "Invalid command: \"" << str << "\"." << std::endl;
							exit(-2);
						}
						std::cout << "mouse not supported" << std::endl;
						exit(-1000);
						break;
					case 'w': {
						if (!(length >= 2)) {
							std::cout << "Invalid command: \"" << str << "\"." << std::endl;
							exit(-2);
						}
						int time = std::stoi(str.substr(1, length - 1));
						actions.push_back(&WaitAction(time));
						break;
					}
					default:
						std::cout << "Invalid command: \"" << str << "\"." << std::endl;
						exit(-2);

				}

			}
		}
	}
	else if (word.compare("list-windows")) {
		std::cout << "Not Implemented" << std::endl;
		return -3;
	}
	else if (word.compare("list-macros")) {
		//TODO:
		std::cout << "Not Implemented" << std::endl;
		return -3;
	}
	else if (word.compare("delete")) {
		//TODO:
		std::cout << "Not Implemented" << std::endl;
		return -3;
	}
	else if (word.compare("help")) {
		std::cout << HELP_MESSAGE << std::endl;
		return 0;
	}
	else {
		std::cout << "Invalid usage, type 'help' for information." << std::endl;
		return -1;
	}
	for (Action* pAction : actions) {
		if (pAction != nullptr) {
			std::cout << pAction->toString() << std::endl;
		}
	}
	std::cout << "doing";
	while (true) {
		for (Action* pAction : actions) {
			if (pAction != nullptr) {
				pAction->execute(verbose);
			}
		}
	}
}

void KeyAction::execute(bool verbose) {
	for (HWND hwnd = GetTopWindow(NULL); hwnd != NULL; hwnd = GetNextWindow(hwnd, GW_HWNDNEXT)) {
		if (IsWindowVisible(hwnd)) {
			int length = GetWindowTextLength(hwnd);
			if (length > 0) {
				char* title = new char[length + 1];
				GetWindowTextA(hwnd, title, length + 1);
				if ((title != "Program Manager") && (title != "Settings") && (title != "Microsoft Store")) {
					switch (type) {
						case Down:
							PostMessage(hwnd, WM_KEYDOWN, key, 0);
							if (verbose) {
								std::cout << "Sent (WM_KEYDOWN, " << key << ", 0) message HWND=" << hwnd << ", NAME=" << title << std::endl;
							}
							break;
						case Up:
							PostMessage(hwnd, WM_KEYUP, key, 0);
							if (verbose) {
								std::cout << "Sent (WM_KEYUP, " << key << ", 0) message HWND=" << hwnd << "NAME=" << title << std::endl;
							}
							break;
						case Typed:
							PostMessage(hwnd, WM_CHAR, key, 0);
							if (verbose) {
								std::cout << "Sent (WM_CHAR, " << key << ", 0) message HWND=" << hwnd << "NAME=" << title << std::endl;
							}
						
					}
				}
			}
		}

	}
}

void KeyAction::generate(Type t, std::string str, std::vector<Action*>* vec) {
	bool escape = false;
	for (char c : str) {
		if (escape) {
			escape = false;
			switch (c) {
				case '\\':
					vec->push_back(new KeyAction('\\', t));
					break;
				case 's':
					vec->push_back(new KeyAction(' ', t));
					break;
				case 'n':
					vec->push_back(new KeyAction('\n', t));
					break;
				case 'q':
					vec->push_back(new KeyAction('\"', t));
					break;
				case 'a':
					vec->push_back(new KeyAction('*', t));
					break;
				case 'b':
					vec->push_back(new KeyAction('\b', t));
					break;
			}
		}
		else {
			if (c == '\\') {
				escape = true;
			}
			else {
				vec->push_back(new KeyAction(c, t));
			}
		}
	}
}