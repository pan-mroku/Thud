#ifndef APP_HPP
#define APP_HPP

#include "scene.hpp"
#include "physics.hpp"

#include <wx/wx.h>
#include <vector>

class App : public wxApp
{
public:
  virtual bool OnInit();
};

#endif
