#include "app.hpp"
#include "gui.h"

IMPLEMENT_APP(App)

bool App::OnInit()
{
  GUI *gui=new GUI();
  gui->Centre();
  gui->Show(true);

  return true;
}
