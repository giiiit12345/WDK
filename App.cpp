#include "App.h"
#include "id.h"

IMPLEMENT_APP(App);

App::App()
{

}

bool App:: OnInit()
{
	if (!wxApp::OnInit())
		return false;

	Frame* main = new Frame(nullptr, frame::id::FRAME, _("Main Frame"));
	main->Show();
	return true;
}

App::~App() 
{

}