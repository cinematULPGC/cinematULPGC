/*
 	CinematULPGC - An application for the study of biomechanics in veterinary medicine
    Copyright (C) 2016  Deriman Franco García

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "wxVideoGUI.h"

DEFINE_EVENT_TYPE(wxEVT_COMMAND_THREAD_UPDATE)
DEFINE_EVENT_TYPE(wxEVT_COMMAND_SLIDER_UPDATE)
DEFINE_EVENT_TYPE(wxID_MOUSELOC)
DEFINE_EVENT_TYPE(wxID_REPEAT)
DEFINE_EVENT_TYPE(wxID_SEEPOINTS)
DEFINE_EVENT_TYPE(wxID_OPENFRAMES)
DEFINE_EVENT_TYPE(wxID_LISTA)
DEFINE_EVENT_TYPE(wxID_LOAD)
DEFINE_EVENT_TYPE(wxID_SCREENSHOT)
DEFINE_EVENT_TYPE(wxID_EXCEL)
DEFINE_EVENT_TYPE(wxID_CSV)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_COMMAND  (wxID_ANY, wxEVT_COMMAND_THREAD_UPDATE, MainFrame::OnThreadUpdate)
	EVT_COMMAND  (wxID_ANY, wxEVT_COMMAND_SLIDER_UPDATE, MainFrame::OnSliderAutoUpdate)
	EVT_COMMAND (wxID_BTN1, wxEVT_COMMAND_BUTTON_CLICKED, MainFrame::m_button1OnButtonClick)
	EVT_COMMAND (wxID_BTN2, wxEVT_COMMAND_BUTTON_CLICKED, MainFrame::m_button2OnButtonClick)
	EVT_COMMAND (wxID_ANY, wxEVT_COMMAND_LISTBOX_SELECTED, MainFrame::m_listBox1OnListBox)
	EVT_SCROLL_THUMBRELEASE  (MainFrame::OnSliderUpdate)
	EVT_SCROLL_CHANGED  (MainFrame::OnSliderChanged)
	EVT_SCROLL_THUMBTRACK  (MainFrame::OnSliderGrab)
    EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
    EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
    EVT_MENU(wxID_OPENFRAMES, MainFrame::OnOpenFromFrames)
    EVT_MENU(wxID_SAVE, MainFrame::OnSavePoints)
    EVT_MENU(wxID_SCREENSHOT, MainFrame::OnSaveScreenshot)
    EVT_MENU(wxID_LOAD, MainFrame::OnLoadPoints)
    EVT_MENU(wxID_EXCEL, MainFrame::OnExportsExcel)
    EVT_MENU(wxID_CSV, MainFrame::OnExportsCSV)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
    EVT_MENU(wxID_SEEPOINTS, MainFrame::OnThreadUpdate)
    EVT_MENU(wxID_HIDE1, MainFrame::OnHiding)
    EVT_MENU(wxID_HIDE2, MainFrame::OnHiding)
    EVT_MENU(wxID_HIDE3, MainFrame::OnHiding)
    EVT_MENU(wxID_HIDE4, MainFrame::OnHiding)
    EVT_MENU(wxID_HIDE5, MainFrame::OnHiding)
    EVT_MENU(wxID_HIDE6, MainFrame::OnHiding)
    EVT_MENU(wxID_HIDE7, MainFrame::OnHiding)
    EVT_MENU(wxID_FPS, MainFrame::OnFPS )
    EVT_MENU(wxID_CAL1, MainFrame::OnCal1 )
    EVT_MENU(wxID_CAL2, MainFrame::OnCal2 )
    EVT_TOOL(wxID_DETECT, MainFrame::OnTools)
    EVT_TOOL(wxID_PLAY, MainFrame::OnTools )
   	EVT_TOOL(wxID_NEXT, MainFrame::OnTools )
   	EVT_TOOL(wxID_PREV, MainFrame::OnTools )
   	EVT_TOOL(wxID_STOP, MainFrame::OnTools )
   	EVT_TOOL(wxID_INI, MainFrame::OnTools )
   	EVT_TOOL(wxID_CALC, MainFrame::OnTools )
   	EVT_TOOL(wxID_END, MainFrame::OnTools )
  	EVT_TOOL(wxID_ADDPTS, MainFrame::OnTools )
  	EVT_TOOL(wxID_RMVPTS, MainFrame::OnTools )
  	EVT_TOOL(wxID_VRDT, MainFrame::OnTools )
  	EVT_TOOL(wxID_DISTANCE, MainFrame::OnTools )
  	EVT_TOOL(wxID_REALDIST, MainFrame::OnTools )
  	EVT_TOOL(wxID_HOR, MainFrame::OnTools )
  	EVT_TOOL(wxID_3PTS, MainFrame::OnTools )
  	EVT_TOOL(wxID_2PTS, MainFrame::OnTools )
    EVT_SIZE(MainFrame::OnResize)
END_EVENT_TABLE()
