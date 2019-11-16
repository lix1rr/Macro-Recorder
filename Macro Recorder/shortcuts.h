#include <Windows.h>
#include <vector>
#include <string>
#include <iostream>

//(HWND, UINT, WPARAM, LPARAM)
struct Message {
	UINT msg;
	WPARAM wParam;
	LPARAM lParam;
};
class Action{
public:
	virtual void execute(bool = false) = 0;
	virtual std::string toString() = 0;
};
struct Shortcut {
	std::vector<Action*> actions;
	static Shortcut parse(std::string);
};
class KeyAction: public Action {
public:
	enum Type{Up, Down, Typed};
	Type type;
	char key;
	KeyAction(char c, Type t) { key = c; type = t; }
	virtual void execute(bool verbose = false) override;
	static void generate(Type t, std::string str, std::vector<Action*>* vec);
	virtual std::string toString() override { return "KeyAction  key = " + std::string(1, key); }
};
class WaitAction: public Action {
public:
	int time;
	virtual void execute(bool verbose = false) override { Sleep(time); }
	WaitAction(int miliseconds) { time = miliseconds; }
	virtual std::string toString() override { return "WaitAction waitTime = " + std::to_string(time);  }
}; 