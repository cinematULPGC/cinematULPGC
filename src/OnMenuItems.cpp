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
*//*
 * OnMenuItems.cpp
 *
 *  Created on: 29/07/2014
 *      Author: Deriman Franco
 */


#include "wxVideoGUI.h"





// event handlers

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
    // true is to force the frame to close
    Close(true);
}
void MainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
	//Verify and destroy any previous video
	if (this->GetThread())
	{
		int answer = wxMessageBox("Se ha detectado un video en ejecucion. Si continua perdera todo el progreso no guardado. ¿Seguro de que desea continuar?",
				"Alerta", wxYES_NO, this);
		if (answer == wxNO){
		   return;
		}
		videocv.release();
		points.clearEverything();
		/*Borra la vieja tabla de puntos*/
		for(int i=0;i<numgraf;i++){
			mpFXYVector* vectorLayer;
			vectorLayer=(mpFXYVector*)m_plot->GetLayerByName(wxString::Format(wxT("Angulo_%i"),i));

			m_plot->DelLayer(vectorLayer,true,true);
		}
		for(int i=0;i<numln;i++){
			mpFXYVector* vectorLayer;
			vectorLayer=(mpFXYVector*)m_plot2->GetLayerByName(wxString::Format(wxT("Distancia_%i"),i));

			m_plot2->DelLayer(vectorLayer,true,true);
		}
		angleGrid->ClearGrid();
		lineGrid->ClearGrid();
		this->GetThread()->Delete();


		if(m_toolBar1->GetToolState(wxID_PLAY)||m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)||
				m_toolBar1->GetToolState(wxID_DETECT)||m_toolBar5->GetToolState(wxID_VRDT)||m_toolBar5->GetToolState(wxID_3PTS)||
				m_toolBar4->GetToolState(wxID_2PTS))
		{
			m_toolBar1->ToggleTool(wxID_PLAY,false);
			m_toolBar2->ToggleTool(wxID_ADDPTS,false);
			m_toolBar2->ToggleTool(wxID_RMVPTS,false);
			m_toolBar1->ToggleTool(wxID_DETECT,false);
			m_toolBar5->ToggleTool(wxID_VRDT,false);
			m_toolBar5->ToggleTool(wxID_3PTS,false);
			m_toolBar4->ToggleTool(wxID_2PTS,false);
		}
	}
	//Create a new thread to start the new video
	if (CreateThread(wxTHREAD_DETACHED) != wxTHREAD_NO_ERROR)
	{
		wxLogMessage("Error: Could not create the worker thread!");
		return;
	}
	if (GetThread()->Run() != wxTHREAD_NO_ERROR)
	{
		wxLogMessage("Error: Could not run the worker thread!");
	}
}
void MainFrame::OnOpenFromFrames(wxCommandEvent& WXUNUSED(event))
{
	//Parse the sequence files
	 wxString filename = wxFileSelector("Elija la primera imagen de la secuencia", "", "","", "*.*",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if ( !filename.empty() )
		{
			wxString newpath,buffer,name,number,newnumber,ext,path=filename;
			int len,pos,len2;
		    cv::Mat frame;

			//findnewpath()
			{
				name=path.BeforeLast('.');
				ext=path.AfterLast('.');
			//	if(path==name) no extension
				number=name;
				while(!number.IsNumber()){
					number=number.Remove(0,1);//Remove characters until I have only numbers
				}
				name.Replace(number,""); //Remove the numbers from the name
				len=number.Len();		//cuenta el numero de caracteres en number
				pos=wxAtoi(number);		//convierte number en int
			}

			char buf[200];
			strcpy( buf, (const char*)filename.mb_str(wxConvUTF8) );
			frame=imread(buf);
			filename=wxString::Format(wxT("%svideo.avi"), name);
			wxString newvideo = wxFileSelector("Elija el nombra y la ruta para guardar el archivo", "", filename,"", "*.*",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);

			if(!newvideo.Contains(".avi")){
				newvideo=wxString::Format(wxT("%svideo.avi"), newvideo);
			}
			VideoWriter outputVideo(newvideo.ToStdString(),-1,25.,frame.size());     // Open the output

			/*Abre una ventana para mostrar el progreso*/
			progressDialog=new wxProgressDialog(wxT("Procesando archivos"),wxT(""), 100, this,
					wxPD_APP_MODAL|wxPD_AUTO_HIDE| wxPD_SMOOTH|wxPD_ELAPSED_TIME|wxPD_CAN_ABORT);
			while(!frame.empty())
			{
				outputVideo << frame; //Si existe meto el frame en el video

				//findnewpath()
				{
					pos++;
					newnumber=wxString::Format(wxT("%i"), pos);
					len2=newnumber.Len();		//cuenta el numero de caracteres en newnumber
					if (len>len2)				//Añade un buffer de 0 al principio de la cadena
					{							//Si es necesario
						buffer.Append('0',(len-len2));
						buffer.Append(newnumber);
					} else {buffer=newnumber;}

					newpath= wxString::Format(wxT("%s%s.%s"), name,buffer,ext);
					buffer="";
				}

				path=newpath;
				strcpy( buf, (const char*)newpath.mb_str(wxConvUTF8) );
				printf("%s\n",buf);fflush(stdout);
				frame=imread(buf);
				if(!progressDialog->Pulse()) break;
			}
			outputVideo.release();
			progressDialog->Destroy();
		}
}

void MainFrame::OnSavePoints(wxCommandEvent& WXUNUSED(event))
{
	 wxString filename = wxFileSelector("Elija el nombra y la ruta para guardar el archivo", "", "","TXT|JSON", "TXT files (*.txt)|*.txt|JSON files (*.json)|*.json|Mostrar todo (*.*)|*.*",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if ( !filename.empty() )
		{
			if(points.saveJSON(filename)>0)

				wxMessageBox(wxString::Format(("Archivo guardado con exito.")),
										 _T("Exito"),
										 wxOK | wxICON_INFORMATION,
										 this);
			else
			    wxMessageBox(wxString::Format(("El archivo %s no pudo guardarse.\nO quizas no habian puntos que guardar."),filename),
			                 _T("Ha surgido un problema."),
			                 wxOK | wxICON_INFORMATION,
			                 this);
		}else{
		    wxMessageBox(wxString::Format(("El archivo %s no pudo ser abierto."),filename),
		                 _T("Ha surgido un problema."),
		                 wxOK | wxICON_INFORMATION,
		                 this);
		}
}
void MainFrame::OnLoadPoints(wxCommandEvent& WXUNUSED(event))
{
	 wxString filename = wxFileSelector("Elija un archivo de puntos para abrir", "", "","TXT|JSON", "TXT files (*.txt)|*.txt|JSON files (*.json)|*.json|Mostrar todo (*.*)|*.*",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if ( !filename.empty() )
		{
			progressDialog=new wxProgressDialog(wxT("Procesando archivo"),wxT(""), 100, this,
					wxPD_APP_MODAL|wxPD_AUTO_HIDE| wxPD_SMOOTH|wxPD_ELAPSED_TIME|wxPD_CAN_ABORT);
			/*Borra la vieja tabla de puntos*/
			for(int i=0;i<numgraf;i++){
				mpFXYVector* vectorLayer;
				vectorLayer=(mpFXYVector*)m_plot->GetLayerByName(wxString::Format(wxT("Angulo_%i"),i));

				m_plot->DelLayer(vectorLayer,true,true);
			}
			for(int i=0;i<numln;i++){
				mpFXYVector* vectorLayer;
				vectorLayer=(mpFXYVector*)m_plot2->GetLayerByName(wxString::Format(wxT("Distancia_%i"),i));

				m_plot2->DelLayer(vectorLayer,true,true);
			}
			/*Eliminar notas de la lista*/
			int pos;
			int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
			for(int frame = 0;frame<=lastframe;frame++){
				pos=FrameIndex1->FindString(wxString::Format(wxT("%i"),frame));
				if(pos>=0){
					FrameIndex1->Delete(pos);
					FrameIndex2->Delete(pos);
					points.borrarNota(frame);
				}
			}
			points.clearEverything();
			angleGrid->ClearGrid();
			lineGrid->ClearGrid();


			if(points.loadJSON(filename)>0){

				//ADD LAYERS HERE
				numgraf=points.cuantosgruposdeangulos();

				for(int i=0;i<numgraf;i++){

					mpFXYVector* vectorLayer = new mpFXYVector(wxString::Format(wxT("Angulo_%i"),i));
					vectorLayer->SetContinuity(true);
					vectorLayer->SetPen(wxPen(EligeColor->GetColour()));
					vectorLayer->SetDrawOutsideMargins(true);
					vectorLayer->ShowName(false);
					m_plot->AddLayer(     vectorLayer );
				//END LAYERS
				}

				//ADD LAYERS HERE
				numln=points.cuantosgruposderectas();

				if(points.countlines()>0){

					mpFXYVector* vectorLayer = new mpFXYVector(wxString::Format(wxT("Distancia_%i"),0));
					vectorLayer->SetContinuity(true);
					vectorLayer->SetPen(wxPen(EligeColor->GetColour()));
					vectorLayer->SetDrawOutsideMargins(true);
					vectorLayer->ShowName(false);
					m_plot2->AddLayer(     vectorLayer );
				//END LAYERS
				}
				fillTable();

				/*Agrega notas a la lista*/
				wxString string;
				for(int frame = 0;frame<=lastframe;frame++){
					if (points.imprimeNota(frame,&string)>0){

						FrameIndex1->Append( wxString::Format(wxT("%i"),frame) );
						FrameIndex2->Append( wxString::Format(wxT("%i"),frame) );
					}
				}
				progressDialog->Destroy();
				wxMessageBox(wxString::Format(("Archivo abierto con exito.")),
										 _T("Exito"),
										 wxOK | wxICON_INFORMATION,
										 this);
			}
		}else{
		    wxMessageBox(wxString::Format(("El archivo %s no pudo ser abierto."),filename),
		                 _T("Ha surgido un problema."),
		                 wxOK | wxICON_INFORMATION,
		                 this);
		}
}
void MainFrame::OnLoadSingleGraph(wxCommandEvent& WXUNUSED(event))
{
	 wxString filename = wxFileSelector("Elija un archivo de puntos para abrir", "", "","TXT|JSON", "TXT files (*.txt)|*.txt|JSON files (*.json)|*.json|Mostrar todo (*.*)|*.*",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
		if ( !filename.empty() )
		{
			std::vector<double> frames;
			std::vector<double> angulos;
			int numele=points.angulosJSON2graph(filename ,&frames,&angulos);
			if(numele>0){

				mpFXYVector* vectorLayer = new mpFXYVector(wxT("Angulo_cargado"));
				vectorLayer->SetContinuity(true);
				vectorLayer->SetPen(wxPen(EligeColor->GetColour()));
				vectorLayer->SetDrawOutsideMargins(true);
				vectorLayer->ShowName(false);
				m_plot->AddLayer(     vectorLayer );
				if(vectorLayer!=NULL){
					vectorLayer->SetData(frames,angulos);
				}
				m_plot->UpdateAll();

				wxMessageBox(wxString::Format(("Archivo abierto con exito.")),
										 _T("Exito"),
										 wxOK | wxICON_INFORMATION,
										 this);
			}
		}else{
		    wxMessageBox(wxString::Format(("El archivo %s no pudo ser abierto."),filename),
		                 _T("Ha surgido un problema."),
		                 wxOK | wxICON_INFORMATION,
		                 this);
		}
}
void MainFrame::OnSaveScreenshot(wxCommandEvent& WXUNUSED(event))
{
	 wxString filename = wxFileSelector("Elija el nombra y la ruta para guardar el archivo", "", "","BMP", "Bitmap files (*.BMP)|*.BMP|Mostrar todo (*.*)|*.*",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
		if ( !filename.empty() )
		{
			if(m_plot->SaveScreenshot(filename,wxBITMAP_TYPE_BMP,wxDefaultSize,false))
				wxMessageBox(wxString::Format(("Se ha guardado el archivo.")),
										 _T("Exito"),
										 wxOK | wxICON_INFORMATION,
										 this);
			else
			    wxMessageBox(wxString::Format(("El archivo %s no pudo guardarse."),filename),
			                 _T("Ha surgido un problema."),
			                 wxOK | wxICON_INFORMATION,
			                 this);
		}else{
		    wxMessageBox(wxString::Format(("El archivo %s no pudo ser abierto."),filename),
		                 _T("Ha surgido un problema."),
		                 wxOK | wxICON_INFORMATION,
		                 this);
		}
}
void MainFrame::OnExportsExcel(wxCommandEvent& WXUNUSED(event))
{
    wxAutomationObject excel;
    wxString macroName("Test");
    //Start the excel application
    if (!excel.GetInstance("Excel.Application"))
		if (!excel.CreateInstance("Excel.Application")) {
			wxMessageBox("La aplicacion ha fallado al intentar abrir Microsoft Excel.");
			return;
		}

    //create a new excel book
    {
    	//put excel in front
    	excel.PutProperty("visible", true);
    	//create workbooks
    	excel.CallMethod("workbooks.add");

    	wxString mytext = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";


        wxVariant rng[1],rng1[1];
    	wxAutomationObject rangeObject;


    	int j=1;
		int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
    	int frameite;
    	wxPoint *vectorgrados;
    	float *vc_grad;

		rng[0] =wxVariant( "A1" );
		rng1[0] =wxVariant( "B1" );
		excel.GetObject(rangeObject, wxT("ActiveSheet.Range"), 1, rng);
		rangeObject.PutProperty("Value","FRAME");
		excel.GetObject(rangeObject, wxT("ActiveSheet.Range"), 1, rng1);
		rangeObject.PutProperty("Value","GRADO");
    	//FOR EACH FRAMES
    	for(frameite=0;frameite<lastframe;frameite++)
    	{
    		//WATCH IF GRADOS EXISTS
			vectorgrados = new wxPoint[points.countgrad(frameite)*3];
			vc_grad = new float[points.countgrad(frameite)];
			int puntosdegrado = points.showGrad(frameite,vc_grad,vectorgrados);

			if (puntosdegrado>0){
				for(int i=0;i<puntosdegrado;i++){
					//"Value = Frame"
					rng[0] =wxVariant( wxString::Format("%c%i",mytext[1],i+j+1) );
					excel.GetObject(rangeObject, wxT("ActiveSheet.Range"), 1, rng);
					rangeObject.PutProperty("Value",frameite);
					/*Value = Grados*/
					rng[0] =wxVariant( wxString::Format("%c%i",mytext[2],i+j+1) );
					excel.GetObject(rangeObject, wxT("ActiveSheet.Range"), 1, rng);
					rangeObject.PutProperty("Value",vc_grad[i]);

				}

				j+=puntosdegrado;//J Deberia contar cada entrada hecha
			}
			delete[] vectorgrados;
			delete[] vc_grad;
    	}


    }

}

void MainFrame::OnExportsCSV(wxCommandEvent& WXUNUSED(event))
{
	 wxString filename = wxFileSelector("Elija el nombra y la ruta para guardar el archivo", "", "","TXT", "Text files (*.TXT)|*.TXT|Mostrar todo (*.*)|*.*",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( !filename.empty() )
	{
		wxFFile binario(filename, wxT("w"));
		if(binario.IsOpened())
		{
			wxString frameangulo = "FRAME;ANGULO\n";
			binario.Write(frameangulo,frameangulo.Length()); //sizeof(str)

			int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);


			int frameite;
			wxPoint *vectorgrados;
			float *vc_grad;
			wxString aux,aux2;

			//FOR EACH FRAMES
			for(frameite=0;frameite<lastframe;frameite++)
			{
				//WATCH IF GRADOS EXISTS
				vectorgrados = new wxPoint[points.countgrad(frameite)*3];
				vc_grad = new float[points.countgrad(frameite)];
				int puntosdegrado = points.showGrad(frameite,vc_grad,vectorgrados);
				if (puntosdegrado>0){
					for(int i=0;i<puntosdegrado;i++){
						//"Value = Frame"
						aux=wxString::Format(wxT("%i"),frameite);
						binario.Write(aux,aux.Length());
						binario.Write(";",1);
						/*Value = Grados*/
						aux2=wxString::Format(wxT("%f"),vc_grad[i]);
						binario.Write(aux2,aux2.Length());
						binario.Write("\n",1);
					}
				}
				delete[] vectorgrados;
				delete[] vc_grad;
			}
			binario.Close();
		}
	}else{
		wxMessageBox(wxString::Format(("El archivo %s no pudo ser abierto."),filename),
					 _T("Ha surgido un problema."),
					 wxOK | wxICON_INFORMATION,
					 this);
	}
}


void MainFrame::OnCal1(wxCommandEvent& WXUNUSED(event))
{

	if ( !videocv.isOpened() ){
	    wxMessageBox(wxString::Format(("No hay video en reproduccion.")),
	                 _T("Ha surgido un problema."),
	                 wxOK | wxICON_INFORMATION,
	                 this);
	}else{
		if (!this->GetThread()) return;
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
		}
		reference_mode=RF_DISTANCE;
		m_toolBar1->ToggleTool(wxID_PLAY,true);
		if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)
				||m_toolBar1->GetToolState(wxID_DETECT)||m_toolBar5->GetToolState(wxID_VRDT)
				||m_toolBar5->GetToolState(wxID_3PTS)||m_toolBar4->GetToolState(wxID_2PTS))
		{
			m_toolBar2->ToggleTool(wxID_ADDPTS,false);
			m_toolBar2->ToggleTool(wxID_RMVPTS,false);
			m_toolBar1->ToggleTool(wxID_DETECT,false);
			m_toolBar5->ToggleTool(wxID_VRDT,false);
			m_toolBar5->ToggleTool(wxID_3PTS,false);
			m_toolBar4->ToggleTool(wxID_2PTS,false);
		}
	}
}
void MainFrame::OnCal2(wxCommandEvent& WXUNUSED(event))
{
	if ( !videocv.isOpened() ){
	    wxMessageBox(wxString::Format(("No hay video en reproducción.")),
	                 _T("Ha surgido un problema."),
	                 wxOK | wxICON_INFORMATION,
	                 this);
	}else{
		int new_fps = wxGetNumberFromUser(wxT("Elija el nuevo valor de FPS"),wxT("FPS"),wxT("FPS"),fps_calc,1,999);
		if (new_fps >0) fps_calc = new_fps;
	}
}

void MainFrame::OnFPS(wxCommandEvent& WXUNUSED(event))
{
	if ( !videocv.isOpened() ){
	    wxMessageBox(wxString::Format(("No hay video en reproducción.")),
	                 _T("Ha surgido un problema."),
	                 wxOK | wxICON_INFORMATION,
	                 this);
	}else{
		int new_fps = wxGetNumberFromUser(wxT("Elija el nuevo valor de FPS"),wxT("FPS"),wxT("FPS"),video_fps,1,999);
		if (new_fps >0) video_fps = new_fps;
	}
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo *info=new wxAboutDialogInfo;
    info->SetName(_("HAV - Herramienta de Apoyo en Veterinaria"));
    info->SetVersion(_("Build 1011"));
    info->SetDescription(_T("Aplicacion de apoyo para el estudio de animales en veterinaria."));
    info->AddDeveloper("Deriman Franco Garcia <deriman.franco101@alu.ulpgc.es>");

    wxAboutBox(*info);
}


void MainFrame::OnHiding (wxCommandEvent& event)
{
	int id = event.GetId();
	switch(id)
	{
	case wxID_HIDE1:	//Oculta Puntos
		if(m_toolBar2->IsShown()){
			m_toolBar2->Hide();
		}else{
			m_toolBar2->Show();
		}
		break;
	case wxID_HIDE2:	//Ocultar Distancia
		if(m_toolBar4->IsShown()){
			m_toolBar4->Hide();
		}else{
			m_toolBar4->Show();
		}
		break;
	case wxID_HIDE3:	//Ocultar video
		if(m_camera_panel->IsShown()){
			m_camera_panel->Hide();
			m_toolBar1->Hide();
			m_toolBar2->Hide();
			m_toolBar3->Hide();
			m_toolBar4->Hide();
			m_toolBar5->Hide();
			m_slider1->Hide();
			EligeColor->Hide();
			SliderInfo->Hide();
			SliderInfo2->Hide();
		}else{
			m_camera_panel->Show();
			m_toolBar1->Show();
			if(m_menuVer->IsChecked(wxID_HIDE2)){
				m_toolBar4->Show();
			}
			if(m_menuVer->IsChecked(wxID_HIDE1)){
				m_toolBar2->Show();
			}
			if(m_menuVer->IsChecked(wxID_HIDE5)){
				m_toolBar5->Show();
			}
			if(m_menuVer->IsChecked(wxID_HIDE7)){
				m_toolBar3->Show();
			}
			m_slider1->Show();
			EligeColor->Show();
			SliderInfo->Show();
			SliderInfo2->Show();
		}
		break;
	case wxID_HIDE4:	//Oculta la leyenda en la grafica
		if((nfo->IsVisible())&&(nfo2->IsVisible())){
			nfo->SetVisible(false);
			nfo2->SetVisible(false);
		}else{
			nfo->SetVisible(true);
			nfo2->SetVisible(true);
		}
		break;
	case wxID_HIDE5:	//Oculta angulos
		if(m_toolBar5->IsShown()){
			m_toolBar5->Hide();
		}else{
			m_toolBar5->Show();
		}
		break;
	case wxID_HIDE6:	//Oculta la posición del ratón en la gráfica
		secondBox->Show(secondBox,false,true);
		if((leg->IsVisible())&&(leg2->IsVisible())){
			leg->SetVisible(false);
			leg2->SetVisible(false);
		}else{
			leg->SetVisible(true);
			leg2->SetVisible(true);
		}
		break;
	case wxID_HIDE7:	//Ocultar Distancia
		if(m_toolBar3->IsShown()){
			m_toolBar3->Hide();
		}else{
			m_toolBar3->Show();
		}
		break;
	default: break;
	}
	this->Layout();
}

void MainFrame::m_button1OnButtonClick (wxCommandEvent& event)
{
	wxString buffer;

	int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
	int frametotal = videocv.get(CV_CAP_PROP_FRAME_COUNT);

	wxString strpos = wxString::Format(wxT("%i"),framepos);
	wxString strtotal = wxString::Format(wxT("%i"),frametotal);

	int nzero = strtotal.Len() - strpos.Len();

	if (nzero>0)
	{
		wxString zeros ="";
		for (int i=0;i<nzero;i++){
			zeros.Append('0');
		}
		strpos.Prepend(zeros);
	}

	for(int i=0;i<m_textCtrl1->GetNumberOfLines();i++){
		buffer.Append(m_textCtrl1->GetLineText(i));
		buffer.Append("\n");
	}
	m_textCtrl2->Clear();
	(*m_textCtrl2) << buffer;
	points.agregaNota(framepos,buffer);

	buffer=wxString::Format(wxT("%i"),framepos);
	if(FrameIndex1->FindString(buffer)==wxNOT_FOUND ){
		FrameIndex1->Append( strpos );
		FrameIndex2->Append( strpos );
	}
}
void MainFrame::m_button2OnButtonClick (wxCommandEvent& event)
{
	wxString buffer;

	int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
	int frametotal = videocv.get(CV_CAP_PROP_FRAME_COUNT);

	wxString strpos = wxString::Format(wxT("%i"),framepos);
	wxString strtotal = wxString::Format(wxT("%i"),frametotal);

	int nzero = strtotal.Len() - strpos.Len();

	if (nzero>0)
	{
		wxString zeros ="";
		for (int i=0;i<nzero;i++){
			zeros.Append('0');
		}
		strpos.Prepend(zeros);
	}

	for(int i=0;i<m_textCtrl2->GetNumberOfLines();i++){
		buffer.Append(m_textCtrl2->GetLineText(i));
		buffer.Append("\n");
	}
	m_textCtrl1->Clear();
	(*m_textCtrl1) << buffer;
	points.agregaNota(framepos,buffer);

	buffer=wxString::Format(wxT("%i"),framepos);
	if(FrameIndex1->FindString(buffer)==wxNOT_FOUND ){
		FrameIndex1->Append( strpos );
		FrameIndex2->Append( strpos );
	}
}
void MainFrame::m_listBox1OnListBox( wxCommandEvent& event )
{
	wxString buffer;
	int frame, id = event.GetId();

	m_mtx_videocv.Lock();
	switch(id)
	{
	case wxID_SEL1:
		frame=wxAtoi(FrameIndex1->GetString(FrameIndex1->GetSelection()));
		FrameIndex2->SetSelection(FrameIndex1->GetSelection());
		if(points.imprimeNota(frame,&buffer)>0){
			m_textCtrl1->Clear();
			m_textCtrl2->Clear();
			(*m_textCtrl1) << buffer;
			(*m_textCtrl2) << buffer;
		}
		break;

	case wxID_SEL2:
		frame=wxAtoi(FrameIndex2->GetString(FrameIndex2->GetSelection()));
		FrameIndex1->SetSelection(FrameIndex2->GetSelection());
		if(points.imprimeNota(frame,&buffer)>0){
			m_textCtrl1->Clear();
			m_textCtrl2->Clear();
			(*m_textCtrl1) << buffer;
			(*m_textCtrl2) << buffer;
		}
		break;
	default:
		break;
	}
	videocv.set(CV_CAP_PROP_POS_FRAMES, frame );
	m_mtx_videocv.Unlock();

}

void MainFrame::OnResize(wxSizeEvent& event)
{
	this->Layout();

	wxSystemSettings systemsetting;

  	wxClientDC dc(m_camera_panel);
  	dc.SetBackground(systemsetting.GetColour(wxSYS_COLOUR_BTNFACE   )  );
	dc.Clear();
}
