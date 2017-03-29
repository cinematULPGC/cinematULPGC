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

#include "wxVideoGUI.h"

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title)
{
	this->SetSizeHints( wxSize( 640,480 ), wxDefaultSize );

	wxBoxSizer* firstSizer;
	firstSizer = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* VideoFrame;
	VideoFrame = new wxBoxSizer( wxVERTICAL );

	VideoFrame->SetMinSize( wxSize( 400,300 ) );
	bSizer1->Add( VideoFrame, 6, wxEXPAND, 10 );
	
	m_camera_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxNO_BORDER );
	VideoFrame->Add( m_camera_panel, 1, wxEXPAND | wxALL, 0 );
	m_camera_panel->Bind(wxEVT_MOTION,&MainFrame::onMouseMove,this);
	m_camera_panel->Bind(wxEVT_LEFT_UP,&MainFrame::onMouseRelease,this);

	points.razondistancia(1);

	wxBoxSizer* SliderBox;
	SliderBox = new wxBoxSizer( wxHORIZONTAL );


	SliderInfo = new wxStaticText( this, wxID_ANY, wxT("No Frames"), wxDefaultPosition, wxDefaultSize, wxRIGHT);
	SliderInfo->Wrap( -1 );
	SliderBox->Add( SliderInfo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );

	m_slider1 = new wxSlider( this, wxID_ANY, 0, 0, 100, wxDefaultPosition, wxDefaultSize/*wxSize( 400,-1 )*/, wxSL_HORIZONTAL );
	m_slider1->SetMinSize( wxSize( 400,-1 ) );
	SliderBox->Add( m_slider1, 0, wxALL, 10 );

	SliderInfo2 = new wxStaticText( this, wxID_ANY, wxT("No Frames"), wxDefaultPosition, wxDefaultSize, wxLEFT);
	SliderInfo2->Wrap( -1 );
	SliderBox->Add( SliderInfo2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 1 );


	bSizer1->Add( SliderBox, 0, wxEXPAND, 5 );

	wxBoxSizer* ToolFrame;
	ToolFrame = new wxBoxSizer( wxHORIZONTAL );

	m_toolBar1 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL ); 
	m_toolBar2 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolBar3 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolBar4 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );
	m_toolBar5 = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL );

	m_toolBar1->AddCheckTool( wxID_PLAY, wxT("Play"), wxBitmap( wxT("res/player_play.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxT("Reproduce el video"), wxT("Reproduce el video"), NULL );

	m_toolBar1->AddTool( wxID_PREV, wxT("Previous Frame"), wxBitmap( wxT("res/player_prev.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Pausa el video y muestra el fotograma anterior"), wxT("Pausa el video y muestra el fotograma anterior"), NULL );
	
	m_toolBar1->AddTool( wxID_STOP, wxT("Stop"), wxBitmap( wxT("res/player_stop.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Detiene la reproduccion"), wxT("Detiene la reproduccion"), NULL );
	
	m_toolBar1->AddTool( wxID_NEXT, wxT("Next Frame"), wxBitmap( wxT("res/player_end.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Pausa el video y muestra el fotograma siguiente"), wxT("Pausa el video y muestra el fotograma siguiente"), NULL );

	/*Detectar automaticamente los puntos marcados (sin implementar)*/
	m_toolBar1->AddSeparator();

	m_toolBar1->AddCheckTool( wxID_DETECT, wxT("Detectar puntos"), wxBitmap( wxT("res/player_gear.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxT("Detector de puntos en el video"), wxT("Optical Flow puntos en la pantalla."), NULL );

	m_toolBar2->AddSeparator();

	m_toolBar2->AddCheckTool( wxID_ADDPTS, wxT("Crear puntos"), wxBitmap( wxT("res/player_detect.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxT("Marcador de puntos en el video"), wxT("Marca puntos en la pantalla, cada tercer punto indica su angulo con los dos puntos anteriores."), NULL );

	m_toolBar2->AddCheckTool( wxID_RMVPTS, wxT("Borrar puntos"), wxBitmap( wxT("res/player_erase.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxT("Borrador de puntos en el video"), wxT("Borra un punto de la pantalla."), NULL );

	m_toolBar3->AddSeparator(); //Time controllers

	m_toolBar3->AddTool( wxID_INI, wxT("Inicio"), wxBitmap( wxT("res/t_ini.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Marca el Frame de inicio para calcular el tiempo"), wxT("Marca el Frame de inicio para calcular el tiempo"), NULL );

	m_toolBar3->AddTool( wxID_CALC, wxT("Stop"), wxBitmap( wxT("res/timing.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Calcula la diferencia"), wxT("Calcula la diferencia"), NULL );

	m_toolBar3->AddTool( wxID_END, wxT("Final"), wxBitmap( wxT("res/t_end.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Marca el Frame final para calcular el tiempo"), wxT("Marca el Frame final para calcular el tiempo"), NULL );

	m_toolBar4->AddSeparator();

	m_toolBar4->AddCheckTool( wxID_2PTS, wxT("Calcula Zancada"), wxBitmap( wxT("res/player_line.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap,  wxT("Calcula Zancada"), wxT("Marca puntos durante la reproduccion para calcular la distancia a la que se encuentran"), NULL );

	m_toolBar4->AddTool( wxID_HOR, wxT("Umbral de Zancada"), wxBitmap( wxT("res/player_umbral.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Umbral de Zancada"), wxT("Marca el limite horizontal en el que se considera una zancada"), NULL );

	m_toolBar4->AddTool( wxID_REALDIST, wxT("Regla"), wxBitmap( wxT("res/player_regla.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxT("Regla"), wxT("Calcula la distancia real entre dos puntos"), NULL );

	m_toolBar5->AddSeparator();

	m_toolBar5->AddCheckTool( wxID_3PTS, wxT("Calcula Angulos"), wxBitmap( wxT("res/player_angle.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap,  wxT("Agrupa 3 puntos y calcula el angulo"), wxT("Agrupa 3 puntos y calcula el angulo"), NULL );
	
	m_toolBar5->AddCheckTool( wxID_VRDT, wxT("Angulo reflejo"), wxBitmap( wxT("res/player_view.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxT("Visor de detalles en el video"), wxT("Visualiza distancia y angulos."), NULL );

	m_toolBar1->AddSeparator(); 
	
	m_toolBar1->Realize();
	m_toolBar2->Realize();
	m_toolBar3->Realize();
	m_toolBar4->Realize();
	m_toolBar5->Realize();

	ToolFrame->Add( m_toolBar1, 0, wxALIGN_BOTTOM|wxEXPAND, 5 );
	ToolFrame->Add( m_toolBar2, 0, wxALIGN_BOTTOM|wxEXPAND, 0 );
	ToolFrame->Add( m_toolBar3, 0, wxALIGN_BOTTOM|wxEXPAND, 0 );
	ToolFrame->Add( m_toolBar4, 0, wxALIGN_BOTTOM|wxEXPAND, 0 );
	ToolFrame->Add( m_toolBar5, 0, wxALIGN_BOTTOM|wxEXPAND, 0 );

	m_toolBar2->Hide();
	
	bSizer1->Add( ToolFrame, 0, wxALIGN_BOTTOM|wxEXPAND, 5 );


	EligeColor = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_USE_TEXTCTRL ); //wxCLRP_USE_TEXTCTRL wxCLRP_DEFAULT_STYLE wxCLRP_SHOW_LABEL
	bSizer1->Add( EligeColor, 0, wxALIGN_BOTTOM|wxEXPAND, 1 );


	/*  Menu Archivo  */
	m_menubar1 = new wxMenuBar( 0 );
	m_menuArchivo = new wxMenu();
	wxMenuItem* m_menuAbrir;
	m_menuAbrir = new wxMenuItem( m_menuArchivo, wxID_OPEN, wxString( wxT("Abrir video") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuAbrir );
	wxMenuItem* m_menuAbrirPorFrames;
	m_menuAbrirPorFrames = new wxMenuItem( m_menuArchivo, wxID_OPENFRAMES, wxString( wxT("Generar video a partir de frames") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuAbrirPorFrames );

	m_menuArchivo->AppendSeparator();

	wxMenuItem* m_menuGuardar;
	m_menuGuardar = new wxMenuItem( m_menuArchivo, wxID_SAVE, wxString( wxT("Guardar puntos") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuGuardar );
	wxMenuItem* m_menuCargar;
	m_menuCargar = new wxMenuItem( m_menuArchivo, wxID_LOAD, wxString( wxT("Cargar puntos") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuCargar );

	m_menuArchivo->AppendSeparator();

	wxMenuItem* m_menuGuardarImagen;
	m_menuGuardarImagen = new wxMenuItem( m_menuArchivo, wxID_SCREENSHOT, wxString( wxT("Guardar imagen") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuGuardarImagen );

	m_menuArchivo->AppendSeparator();

	wxMenuItem* m_menuExportar;
	m_menuExportar = new wxMenuItem( m_menuArchivo, wxID_EXCEL, wxString( wxT("Exportar a Excel") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuExportar );

	wxMenuItem* m_menuExportarCSV;
	m_menuExportarCSV = new wxMenuItem( m_menuArchivo, wxID_CSV, wxString( wxT("Exportar a CSV") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuExportarCSV );

	m_menuArchivo->AppendSeparator();

	wxMenuItem* m_menuSalir;
	m_menuSalir = new wxMenuItem( m_menuArchivo, wxID_EXIT, wxString( wxT("Salir") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuArchivo->Append( m_menuSalir );
	
	m_menubar1->Append( m_menuArchivo, wxT("Archivo") ); 

	/*  Menu Opciones  */
	m_menuOpciones = new wxMenu();

	wxMenuItem* m_menuFPS;
	m_menuFPS = new wxMenuItem( m_menuArchivo, wxID_FPS, wxString( wxT("Establecer los FPS de la reproduccion") ) , wxString( wxT("Modifica la velocidad de reproduccion en funcion de los frames por segundo.") ), wxITEM_NORMAL );
	m_menuOpciones->Append( m_menuFPS );

	wxMenuItem* m_menuCalibrado;
	m_menuCalibrado = new wxMenuItem( m_menuArchivo, wxID_CAL1, wxString( wxT("Calibrado de distancia") ) , wxString( wxT("Calibra la medida de la distancia marcando y arrastrando en el video la medida de referencia.") ), wxITEM_NORMAL );
	m_menuOpciones->Append( m_menuCalibrado );

	wxMenuItem* m_menuCalibrado2;
	m_menuCalibrado2 = new wxMenuItem( m_menuArchivo, wxID_CAL2, wxString( wxT("Calibrado de tiempo") ) , wxString( wxT("Establece la velocidad a la que se grabo el video.") ), wxITEM_NORMAL );
	m_menuOpciones->Append( m_menuCalibrado2 );

	m_menuOpciones->AppendCheckItem( wxID_SEEPOINTS, wxString( wxT("Ver puntos") ), wxEmptyString );
	m_menuOpciones->AppendCheckItem( wxID_MOUSELOC, wxString( wxT("Ver posicion del raton") ), wxEmptyString );
	m_menuOpciones->AppendCheckItem( wxID_REPEAT, wxString( wxT("Repetir video al terminar") ), wxEmptyString );
	m_menuOpciones->Check( wxID_SEEPOINTS, true );
	m_menuOpciones->Check( wxID_MOUSELOC, false );
	m_menuOpciones->Check( wxID_REPEAT, true );

	m_menubar1->Append( m_menuOpciones, wxT("Opciones") );

	/*  Menu Vista  */
	m_menuVer = new wxMenu();
	m_menuVer->AppendCheckItem( wxID_HIDE1, wxString( wxT("Ver Botones de Puntos") ), wxEmptyString );
	m_menuVer->AppendCheckItem( wxID_HIDE2, wxString( wxT("Ver Botones de Distancia") ), wxEmptyString );
	m_menuVer->AppendCheckItem( wxID_HIDE5, wxString( wxT("Ver Botones de Angulos") ), wxEmptyString );
	m_menuVer->AppendCheckItem( wxID_HIDE7, wxString( wxT("Ver Botones de Tiempo") ), wxEmptyString );
	m_menuVer->AppendCheckItem( wxID_HIDE3, wxString( wxT("Ver Reproductor") ), wxEmptyString );
	m_menuVer->AppendSeparator();
	m_menuVer->AppendCheckItem( wxID_HIDE6, wxString( wxT("Ver Leyenda de la grafica") ), wxEmptyString );
	m_menuVer->AppendCheckItem( wxID_HIDE4, wxString( wxT("Ver posicion en la grafica") ), wxEmptyString );
	m_menuVer->Check( wxID_HIDE1, false );
	m_menuVer->Check( wxID_HIDE2, true );
	m_menuVer->Check( wxID_HIDE3, true );
	m_menuVer->Check( wxID_HIDE4, true );
	m_menuVer->Check( wxID_HIDE5, true );
	m_menuVer->Check( wxID_HIDE6, true );
	m_menuVer->Check( wxID_HIDE7, true );

	//m_menuVer->Enable(wxID_HIDE1,false);
	m_menubar1->Append( m_menuVer, wxT("Vista") );

	/*  Menu Ayuda  */
	m_menuAyuda = new wxMenu();
	wxMenuItem* m_menuSobre;
	m_menuSobre = new wxMenuItem( m_menuAyuda, wxID_ABOUT, wxString( wxT("Sobre...") ) , wxEmptyString, wxITEM_NORMAL );
	m_menuAyuda->Append( m_menuSobre );
	
	m_menubar1->Append( m_menuAyuda, wxT("Ayuda") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );

	/*Notebook // Pestañas*/

	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_angulos_panel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* Sizer_angulos;
	Sizer_angulos = new wxBoxSizer( wxVERTICAL );

	m_distan_panel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* Sizer_distan;
	Sizer_distan = new wxBoxSizer( wxVERTICAL );

	/*Plot Panel*/

	firstSizer->Add( bSizer1, 1, wxEXPAND, 5 );

	secondBox = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* plotBox;
	plotBox = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* plotBox2;
	plotBox2 = new wxBoxSizer( wxVERTICAL );

	/*Start of wxMathPlot*/
	grmin=0;
	grmax=0;
	lnmin=0;
	lnmax=0;

	wxFont graphFont(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    m_plot = new mpWindow( m_angulos_panel, -1, wxPoint(0,0), wxSize(300,300), wxSUNKEN_BORDER );

    mpScaleX* xaxis = new mpScaleX(wxT("Frames"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis = new mpScaleY(wxT("Grados"), mpALIGN_LEFT, true);
    xaxis->SetFont(graphFont);
    yaxis->SetFont(graphFont);
    xaxis->SetDrawOutsideMargins(false);
    yaxis->SetDrawOutsideMargins(false);
    m_plot->SetMargins(30, 30, 50, 100);

    m_plot->AddLayer(     xaxis );
    m_plot->AddLayer(     yaxis );

    m_plot->AddLayer( nfo = new mpInfoCoords(wxRect(400,5,90,40), wxWHITE_BRUSH)); //&hatch));
    nfo->SetVisible(true);
    wxBrush hatch2(wxColour(163,208,212), wxSOLID);
    m_plot->AddLayer( leg = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg->SetVisible(true);

    plotBox->Add( m_plot, 1, wxEXPAND );
    Sizer_angulos->Add( plotBox, 1, wxEXPAND, 5 );

    /*End of wxMathPlot*/

	/*Start of wxMathPlot 2*/

    m_plot2 = new mpWindow( m_distan_panel, -1, wxPoint(0,0), wxSize(300,300), wxSUNKEN_BORDER );

    mpScaleX* xaxis2 = new mpScaleX(wxT("Frames"), mpALIGN_BOTTOM, true, mpX_NORMAL);
    mpScaleY* yaxis2 = new mpScaleY(wxT("Distancia"), mpALIGN_LEFT, true);
    xaxis2->SetFont(graphFont);
    yaxis2->SetFont(graphFont);
    xaxis2->SetDrawOutsideMargins(false);
    yaxis2->SetDrawOutsideMargins(false);
    m_plot2->SetMargins(30, 30, 50, 100);

    m_plot2->AddLayer(     xaxis2 );
    m_plot2->AddLayer(     yaxis2 );

    m_plot2->AddLayer( nfo2 = new mpInfoCoords(wxRect(400,5,90,40), wxWHITE_BRUSH)); //&hatch));
    nfo2->SetVisible(true);
    m_plot2->AddLayer( leg2 = new mpInfoLegend(wxRect(200,20,40,40), wxTRANSPARENT_BRUSH)); //&hatch2));
    leg2->SetVisible(true);

    plotBox2->Add( m_plot2, 1, wxEXPAND );
    Sizer_distan->Add( plotBox2, 1, wxEXPAND, 5 );

    /*End of wxMathPlot*/

	/*Tablas*/

	wxBoxSizer* gridBox;
	gridBox = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* gridBox2;
	gridBox2 = new wxBoxSizer( wxHORIZONTAL );

	angleGrid = new wxGrid( m_angulos_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 1 );
	wxSystemSettings systemsetting;
	angleGrid->SetDefaultCellBackgroundColour(systemsetting.GetColour(wxSYS_COLOUR_BTNFACE   ) );

	// Grid
	angleGrid->CreateGrid( 10, 3 );
	angleGrid->EnableEditing( true );
	angleGrid->EnableGridLines( true );
	angleGrid->EnableDragGridSize( false );
	angleGrid->SetMargins( 0, 0 );

	// Columns
	angleGrid->SetColSize( 0, 60 );
	angleGrid->SetColSize( 1, 60 );
	angleGrid->SetColSize( 2, 65 );
	angleGrid->EnableDragColMove( false );
	angleGrid->EnableDragColSize( true );
	angleGrid->SetColLabelSize( 30 );
	angleGrid->SetColLabelValue( 0, wxT("Frame") );
	angleGrid->SetColLabelValue( 1, wxT("Angulo") );
	angleGrid->SetColLabelValue( 2, wxT("wxPoint") );
	angleGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	angleGrid->AutoSizeRows();
	angleGrid->EnableDragRowSize( true );
	angleGrid->SetRowLabelSize( 30 );
	angleGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	angleGrid->DisableCellEditControl();
	angleGrid->EnableEditing(false);

	// Label Appearance

	// Cell Defaults
	angleGrid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	gridBox->Add( angleGrid, 0, wxEXPAND | wxALIGN_CENTER | wxALL, 5 );

	Sizer_angulos->Add( gridBox, 1, wxEXPAND, 5 );

	lineGrid = new wxGrid( m_distan_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 1 );
	lineGrid->SetDefaultCellBackgroundColour(systemsetting.GetColour(wxSYS_COLOUR_BTNFACE   ) );

	// Grid
	lineGrid->CreateGrid( 10, 3 );
	lineGrid->EnableEditing( true );
	lineGrid->EnableGridLines( true );
	lineGrid->EnableDragGridSize( false );
	lineGrid->SetMargins( 0, 0 );

	// Columns
	lineGrid->SetColSize( 0, 60 );
	lineGrid->SetColSize( 1, 60 );
	lineGrid->SetColSize( 2, 65 );
	lineGrid->EnableDragColMove( false );
	lineGrid->EnableDragColSize( true );
	lineGrid->SetColLabelSize( 30 );
	lineGrid->SetColLabelValue( 0, wxT("Frame") );
	lineGrid->SetColLabelValue( 1, wxT("Distancia") );
	lineGrid->SetColLabelValue( 2, wxT("wxPoint") );
	lineGrid->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );

	// Rows
	lineGrid->AutoSizeRows();
	lineGrid->EnableDragRowSize( true );
	lineGrid->SetRowLabelSize( 30 );
	lineGrid->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	lineGrid->DisableCellEditControl();
	lineGrid->EnableEditing(false);

	// Cell Defaults
	lineGrid->SetDefaultCellAlignment( wxALIGN_RIGHT, wxALIGN_TOP );
	gridBox2->Add( lineGrid, 1, wxEXPAND | wxALIGN_CENTER | wxALL, 5 );


	Sizer_distan->Add( gridBox2, 1, wxEXPAND, 5 );

/*Cuadro de anotaciones*/

	//Cuadro de angulos
	wxBoxSizer* notaBox;
	notaBox = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* notaFrame;
	notaFrame = new wxBoxSizer( wxVERTICAL );

	//Selector de notas por Frame
	notaFrameT = new wxStaticText( m_angulos_panel, wxID_ANY, wxT("Frame:"), wxDefaultPosition, wxDefaultSize, wxLEFT);
	notaFrameT->Wrap( -1 );
	notaFrame->Add( notaFrameT, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	FrameIndex1 = new wxListBox( m_angulos_panel, wxID_SEL1, wxDefaultPosition, wxSize(50,200), 0, NULL, wxLB_SINGLE|wxLB_SORT ); //wxLB_SORT ordena alfabeticamente
	notaFrame->Add( FrameIndex1, 0, wxEXPAND|wxALL, 0 );

	gridBox->Add( notaFrame, 0, wxEXPAND|wxALL, 5 );

	//Caja de anotaciones
	notaBoxT = new wxStaticText( m_angulos_panel, wxID_ANY, wxT("Anotaciones:"), wxDefaultPosition, wxDefaultSize, wxLEFT);
	notaBoxT->Wrap( -1 );
	notaBox->Add( notaBoxT, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

	m_textCtrl1 = new wxTextCtrl( m_angulos_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250,200),wxTE_MULTILINE );
	notaBox->Add( m_textCtrl1, 1, wxEXPAND | wxALL, 0 );

	m_button1 = new wxButton( m_angulos_panel, wxID_BTN1, wxT("Guardar Nota"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button1->Enable(false);
	notaBox->Add( m_button1, 0, wxALIGN_RIGHT | wxALL, 0 );


	gridBox->Add( notaBox, 0, wxEXPAND|wxALL, 5 );

/*------------------------------------------------*/
	//Cuadro de distancias
	wxBoxSizer* notaBox2;
	notaBox2 = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* notaFrame2;
	notaFrame2 = new wxBoxSizer( wxVERTICAL );

	//Selector de notas por Frame
	lastframenoted=0;
	notaFrameT2 = new wxStaticText( m_distan_panel, wxID_ANY, wxT("Frame:"), wxDefaultPosition, wxDefaultSize, wxLEFT);
	notaFrameT2->Wrap( -1 );
	notaFrame2->Add( notaFrameT2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );
	FrameIndex2 = new wxListBox( m_distan_panel, wxID_SEL2, wxDefaultPosition, wxSize(50,200), 0, NULL, wxLB_SINGLE|wxLB_SORT ); //wxLB_SORT ordena alfabeticamente
	notaFrame2->Add( FrameIndex2, 0, wxEXPAND|wxALL|wxFIXED_MINSIZE, 0 );

	gridBox2->Add( notaFrame2, 0, wxALL, 5 );

	//Caja de anotaciones
	notaBoxT2 = new wxStaticText( m_distan_panel, wxID_ANY, wxT("Anotaciones:"), wxDefaultPosition, wxDefaultSize, wxLEFT);
	notaBoxT2->Wrap( -1 );
	notaBox2->Add( notaBoxT2, 0, wxALIGN_CENTER_VERTICAL|wxALL, 0 );

	m_textCtrl2 = new wxTextCtrl( m_distan_panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250,200), wxTE_MULTILINE );
	notaBox2->Add( m_textCtrl2, 1, wxEXPAND | wxALL, 0 );

	m_button2 = new wxButton( m_distan_panel, wxID_BTN2, wxT("Guardar Nota"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button2->Enable(false);
	notaBox2->Add( m_button2, 0, wxALIGN_RIGHT | wxALL, 0 );


	gridBox2->Add( notaBox2, 0, wxEXPAND|wxALL, 5 );
/*------------------------------------------------*/



	/*Notebooks*/
	m_angulos_panel->SetSizer( Sizer_angulos );
	m_angulos_panel->Layout();
	Sizer_angulos->Fit( m_angulos_panel );
	m_notebook1->AddPage( m_angulos_panel, wxT("Angulos"), false );

	m_distan_panel->SetSizer( Sizer_distan );
	m_distan_panel->Layout();
	Sizer_distan->Fit( m_distan_panel );
	m_notebook1->AddPage( m_distan_panel, wxT("Distancias"), false );


	secondBox->Add( m_notebook1, 1, wxEXPAND, 5 );


	firstSizer->Add( secondBox, 1, wxEXPAND, 5 );

	/* hasta aqui*/
	reference_mode = 0;
	dist_med = 0;
	numgraf=0;
	numln=0;
	fps_calc=100;
	firsttimeframe=0;
	lasttimeframe=0;
	umbral_hor = 0;

	this->SetSizerAndFit( firstSizer );
	this->Layout();
	
	this->Centre( wxBOTH );


}

MainFrame::~MainFrame()
{
}

