#include "gui.hpp"

#include "opttritri.hpp" //test

#include <wx/xrc/xmlres.h>
#include <wx/checkbox.h>
#include <iostream>

Gui::Gui()
{
	Bind(wxEVT_IDLE, &Gui::OnIdle, this);
	Timer.reset();
}

void Gui::InitAfterXRC()
{
	FindWindow("ButtonQuit")->Bind(wxEVT_BUTTON, &Gui::onQuitButton, this);
	viewerWindow=(wxWindow*)FindWindow("osgViewer");

	algorithmChoice=(wxChoice*)FindWindow("AlgorithmChoice");
	algorithmChoice->Append("Brak");
	algorithmChoice->Append(wxString::FromUTF8("Trójkąt-trójkąt"));
	algorithmChoice->SetSelection(1);
	algorithmChoice->Append("OpenCL");
	algorithmChoice->Append(wxString::FromUTF8("Trójkąt-trójkąt wszystkie pary"));
	algorithmChoice->Bind(wxEVT_CHOICE,
	                      [=](wxCommandEvent& event)
	                      {
		                      PhysicsEngine.ActiveAlgorithm = Physics::CollisionAlgorithmEnum(event.GetInt());
	                      });

	SceneChoice=(wxChoice*)FindWindow("SceneChoice");
	SceneChoice->Bind(wxEVT_CHOICE,
	                  [=](wxCommandEvent& event)
	                  {
		                  CurrentScene=&(Scenes[event.GetInt()]);
		                  osgViewer->setSceneData(CurrentScene->GroupNode);
	                  });

	WorkGroupCombo=(wxComboBox*)FindWindow("WorkGroupCombo");
	WorkGroupCombo->Bind(wxEVT_COMBOBOX,
	                  [=](wxCommandEvent& event)
	                  {
		                  PhysicsEngine.workGroupSize=event.GetInt();
	                  });
	WorkGroupCombo->Bind(wxEVT_TEXT_ENTER,
	                  [=](wxCommandEvent& event)
	                  {
		                  int index=wxAtoi(event.GetString());
		                  if(index>=WorkGroupCombo->GetCount())
			                  index=0;
		                  WorkGroupCombo->SetSelection(index);
		                  PhysicsEngine.workGroupSize=index;
	                  });

	MaxWorkGroupText=(wxStaticText*)FindWindow("MaxWorkGroupText");
}

void Gui::onQuitButton(wxCommandEvent& event)
{
	Close(true);
}

void Gui::SetViewer(osgViewer::Viewer *viewer)
{
	osgViewer = viewer;
}

wxWindow* Gui::GetViewerWindow()
{
	return viewerWindow;
}

wxChoice* Gui::GetAlgorithmChoice()
{
	return algorithmChoice;
}

void Gui::OnIdle(wxIdleEvent &event)
{
	if (!osgViewer->isRealized())
		return;
	((wxCheckBox*)FindWindow("AlgorithmCheckBox"))->SetValue(PhysicsEngine.CheckCollision(*CurrentScene));
	((wxCheckBox*)FindWindow("SceneCheckBox"))->SetValue(PhysicsEngine.CheckSceneCollision(*CurrentScene));
	PhysicsEngine.Tick(*CurrentScene, Timer.elapsedTime_m());
	Timer.reset();

	osgViewer->frame();
		
	event.RequestMore();
}
