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
*////////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WXVIDEOGUI_H__
#define __WXVIDEOGUI_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/aboutdlg.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/dcclient.h>
#include <wx/dcbuffer.h>
#include <wx/aui/aui.h>
#include <wx/numdlg.h>
#include <wx/msw/ole/automtn.h>
#include <wx/filedlg.h>
#include <wx/textdlg.h>
#include <wx/grid.h>
#include <wx/slider.h>
#include <wx/clrpicker.h>
#include <wx/settings.h>
#include <wx/progdlg.h>

//wxMathPlot
#include "wxMathPlot/mathplot.h"

//OpenCV
#include "opencv2/opencv.hpp"

using namespace cv;

//wxJSON incluye las mismas macros para MAX y MIN que OpenCV, así que hay que deshacer alguna.
#undef MAX
#undef MIN

//Common and custom libraries
#include <map>
#include "pointhandler.h"
#include "opticalflow.h"
   using namespace std;

///////////////////////////////////////////////////////////////////////////

#define wxID_PLAY 1000
#define wxID_PAUSE 1001
#define wxID_NEXT 1002
#define wxID_ADDPTS 1003
#define wxID_DISTANCE 1004
#define wxID_FPS 1005
#define wxID_PREV 1006
#define wxID_REALDIST 1007
#define wxID_RMVPTS 1008
#define wxID_DETECT 1009
#define wxID_VRDT 1010
#define wxID_2PTS 1011
#define wxID_3PTS 1012
#define wxID_CAL1 1013
#define wxID_CAL2 1014
#define wxID_INI 1015
#define wxID_END 1016
#define wxID_CALC 1017
#define wxID_HOR 1018

#define wxID_HIDE1 1021
#define wxID_HIDE2 1022
#define wxID_HIDE3 1023
#define wxID_HIDE4 1024
#define wxID_HIDE5 1025
#define wxID_HIDE6 1026
#define wxID_HIDE7 1027

#define wxID_BTN1  1030
#define wxID_BTN2  1031
#define wxID_SEL1  1032
#define wxID_SEL2  1033

//Macros para marcar puntos en pantalla ("reference modes")
#define RF_NOTHING    0
#define RF_DISTANCE   1
#define RF_DISTANCE_2 2
#define RF_REALDIST   3
#define RF_REALDIST_2 4
#define RF_ADD_PTS    5
#define RF_RMV_PTS    6
#define RF_AUTODETECT 7
#define RF_VRDT 	  8
#define RF_2PTS		  9
#define RF_3PTS 	  10
#define RF_HOR		  11


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////


DECLARE_EVENT_TYPE (wxEVT_COMMAND_THREAD_UPDATE, -1)
DECLARE_EVENT_TYPE (wxEVT_COMMAND_SLIDER_UPDATE, -1)
DECLARE_EVENT_TYPE (wxID_MOUSELOC, -1)
DECLARE_EVENT_TYPE (wxID_SEEPOINTS, -1)
DECLARE_EVENT_TYPE (wxID_REPEAT, -1)
DECLARE_EVENT_TYPE (wxID_LISTA, -1)
DECLARE_EVENT_TYPE (wxID_OPENFRAMES, -1)
DECLARE_EVENT_TYPE (wxID_LOAD, -1)
DECLARE_EVENT_TYPE (wxID_LOADPLOT, -1)
DECLARE_EVENT_TYPE (wxID_EXCEL, -1)
DECLARE_EVENT_TYPE (wxID_SCREENSHOT,-1)
DECLARE_EVENT_TYPE (wxID_CSV,-1)



class MainFrame : public wxFrame , public wxThreadHelper
{
	private:
	// Private members
		wxThread::ExitCode	Entry();
		wxCriticalSection	m_cs_image;
		wxMutex				m_mtx_videocv;
		wxMutex				m_mtx_slider;
		wxImage				m_video_image;
		cv::VideoCapture	videocv;
		float				video_fps, fps_ref, dist_ref, dist_med, razon;
		int 				h_offpos, w_offpos;
		wxPoint				ref_0;
		wxPoint				*multiref;
		int					reference_mode,multiref_n;
		pointhandler		points;
		cv::Mat 			old_image_rp;
		classFLOW 			flowclass;
	
	protected:
		wxBoxSizer* secondBox;

		wxToolBar* m_toolBar1;
		wxToolBar* m_toolBar2;
		wxToolBar* m_toolBar3;
		wxToolBar* m_toolBar4;
		wxToolBar* m_toolBar5;
		wxSlider* m_slider1;
		wxMenuBar* m_menubar1;
		wxMenu* m_menuArchivo;
		wxMenu* m_menuOpciones;
		wxMenu* m_menuVer;
		wxMenu* m_menuAyuda;
		wxStatusBar* m_statusBar1;
		wxPanel* m_camera_panel;
		wxPanel* m_angulos_panel;
		wxPanel* m_distan_panel;
		wxColourPickerCtrl* EligeColor;
		wxNotebook* m_notebook1;
		wxStaticText* SliderInfo;
		wxStaticText* SliderInfo2;
		wxStaticText* notaFrameT;
		wxStaticText* notaFrameT2;
		wxStaticText* notaBoxT;
		wxStaticText* notaBoxT2;

		/*Anotaciones*/
		int lastframenoted;
		wxListBox* FrameIndex1;
		wxListBox* FrameIndex2;
		wxTextCtrl* m_textCtrl1;
		wxTextCtrl* m_textCtrl2;
		wxButton* m_button1;
		wxButton* m_button2;

		wxPanel* plot_panel;
		mpWindow* m_plot; // wxMathPlot : mpWindow can be embedded as subwindow in a wxPanel, a wxFrame, or any other wxWindow.
		wxPanel* plot_panel2;
		mpWindow* m_plot2; // wxMathPlot : mpWindow can be embedded as subwindow in a wxPanel, a wxFrame, or any other wxWindow.

	    mpInfoCoords *nfo; // mpInfoLayer* nfo; Contiene las coordenadas del raton en la grafica
	    mpInfoLegend* leg; // mpInfoLayer* leg; Contiene la información de los colores de la grafica
	    mpInfoCoords *nfo2; // mpInfoLayer* nfo; Contiene las coordenadas del raton en la grafica
	    mpInfoLegend* leg2; // mpInfoLayer* leg; Contiene la información de los colores de la grafica
		wxGrid* angleGrid;
		wxGrid* lineGrid;

		/*Barra de progreso*/
		wxProgressDialog* progressDialog;


		//numero de graficas
		int numgraf,numln;
		float grmax,grmin;
		float lnmax,lnmin;

		//Posición de Frames
		int lasttimeframe;
		int firsttimeframe;
		int fps_calc;
		//int framepos;

		//Umbral de zancada
		int umbral_hor;


		// event handlers (these functions should _not_ be virtual)

	    void OnExit(wxCommandEvent& event);
	    void OnOpen(wxCommandEvent& event);
	    void OnOpenFromFrames(wxCommandEvent& event);
	    void OnLoadPoints(wxCommandEvent& event);
	    void OnLoadSingleGraph(wxCommandEvent& event);
	    void OnSavePoints(wxCommandEvent& event);
	    void OnSaveScreenshot(wxCommandEvent& event);
	    void OnExportsCSV(wxCommandEvent& (event));
	    void OnExportsExcel(wxCommandEvent& (event));
	    void OnFPS(wxCommandEvent& (event));
	    void OnCal1(wxCommandEvent& (event));
	    void OnCal2(wxCommandEvent& (event));
	    void OnAbout(wxCommandEvent& event);
		void Render(wxDC &dc);
		void Renderpoints(wxDC &dc);
		void OnThreadUpdate(wxCommandEvent& event);
		void onMouseRelease(wxMouseEvent& event);
		void onMouseMove(wxMouseEvent& event);
		void OnTools(wxCommandEvent& event);
		int fillTable();

		void OnSliderUpdate (wxScrollEvent& event);
		void OnSliderChanged (wxScrollEvent& event);
		void OnSliderAutoUpdate (wxCommandEvent& event);
		void OnSliderGrab (wxScrollEvent& event);
		void OnHiding (wxCommandEvent& event);
		void m_button1OnButtonClick (wxCommandEvent& event);
		void m_button2OnButtonClick (wxCommandEvent& event);
		void m_listBox1OnListBox( wxCommandEvent& event );


		void OnResize(wxSizeEvent& event);

	    DECLARE_EVENT_TABLE()
	
	public:
		
		MainFrame(const wxString& title);

		~MainFrame();

};


#endif //__WXVIDEOGUI_H__
