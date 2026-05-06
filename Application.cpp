#include"Application.h"

Application::Application() :_currentinterface(nullptr), _nextinterface(nullptr) {

}

//不知何意味
Application::~Application() {
	for (auto it : _interface) {
		delete it.second;
	}
}

//插入界面
//以指针作为返回值实现调用的串联
Application* Application::Registerinterface(Interface* intf) {
	_interface.emplace(intf->Name(), intf);
	if (!_currentinterface) {
		_currentinterface = intf;
	}
	return this;
}

//在一轮更新后改变目标界面
void Application::Changeinterface(const std::wstring& name) {
	auto it = _interface.find(name);
	if (it == _interface.end()) {
		return;
	}
	_currentinterface = it->second;
	_currentinterface->Onenter();

}

//直接改变目标界面
void Application::Setinterface(const std::wstring& name) {
	auto it = _interface.find(name);
	if (it == _interface.end()) {
		return;
	}
	_currentinterface = it->second;
	_currentinterface->Onenter();
	_nextinterface = nullptr;
}

void Application::Update() {
	if (_currentinterface) {
		_currentinterface->Update();
	}
	if (_nextinterface && _nextinterface != _currentinterface) {
		_currentinterface->Onexit();
		_nextinterface = nullptr;
	}

}

void Application::Draw() {
	if (_currentinterface) {
		_currentinterface->Draw();
	}
}