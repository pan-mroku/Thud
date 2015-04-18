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
	algorithmChoice->Append("None");
	algorithmChoice->Append("Triangle-triangle");
	algorithmChoice->SetSelection(1);
	algorithmChoice->Append("OpenCL");
	algorithmChoice->Bind(wxEVT_CHOICE,
	                      [=](wxCommandEvent& event)
	                      {
		                      physicsEngine.ActiveAlgorithm = Physics::CollisionAlgorithmEnum(event.GetInt());
	                      });

	SceneChoice=(wxChoice*)FindWindow("SceneChoice");
	SceneChoice->Bind(wxEVT_CHOICE,
	                  [=](wxCommandEvent& event)
	                  {
		                  CurrentScene=&(Scenes[event.GetInt()]);
		                  osgViewer->setSceneData(CurrentScene->GroupNode);
	                  });
	
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
	((wxCheckBox*)FindWindow("AlgorithmCheckBox"))->SetValue(physicsEngine.CheckCollision(*CurrentScene));
	((wxCheckBox*)FindWindow("SceneCheckBox"))->SetValue(physicsEngine.CheckSceneCollision(*CurrentScene));
	physicsEngine.Tick(*CurrentScene, Timer.elapsedTime_m());
	Timer.reset();

	osgViewer->frame();
		
	event.RequestMore();
}
