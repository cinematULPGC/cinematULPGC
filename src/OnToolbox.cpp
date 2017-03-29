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
 * OnToolbox.cpp
 *
 *  Created on: 29/07/2014
 *      Author: Deriman Franco
 */




#include "wxVideoGUI.h"


void MainFrame::OnTools (wxCommandEvent& event)
{
	int id = event.GetId();
	int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
	int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);

	switch (id)
	{
	case wxID_PLAY:
	{
		if (!this->GetThread()) return;
		if (this->GetThread()->IsPaused())
		{
			this->GetThread()->Resume();
			m_toolBar1->SetToolNormalBitmap(wxID_PLAY,wxBitmap( wxT("res/player_pause.bmp"), wxBITMAP_TYPE_ANY ));
			m_toolBar1->SetToolShortHelp(wxID_PLAY,wxT("Pausa el video"));
			m_toolBar1->SetToolLongHelp(wxID_PLAY,wxT("Pausa el video"));

		}
		break;
	}
	case wxID_PREV:
	{
		if (!this->GetThread()) return;
		if (this->GetThread()->IsPaused())
		{
			this->GetThread()->Resume();
		}


		if(!m_toolBar1->GetToolState(wxID_PLAY)){
			m_toolBar1->ToggleTool(wxID_PLAY,true);
		}

		printf("attempt to take PREV lock\n");fflush(stdout);
		m_mtx_videocv.Lock();
		printf("PREV lock taken\n");fflush(stdout);
		 lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
		 framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
		if(framepos<=1)
		{

			videocv.set(CV_CAP_PROP_POS_FRAMES, 1);

			wxMessageBox(wxString("First Frame reached."),
					 _T("Previous Button clicked"),
					 wxOK | wxICON_INFORMATION,
					 this);
		}else{

			videocv.set(CV_CAP_PROP_POS_FRAMES, framepos-1 );

		}
		m_mtx_videocv.Unlock();
		printf("PREV lock released\n");fflush(stdout);

		break;
	}
	case wxID_NEXT:
	{
		if (!this->GetThread()) return;
		if (this->GetThread()->IsPaused())
		{
			this->GetThread()->Resume();
		}
		if(!m_toolBar1->GetToolState(wxID_PLAY)){
			m_toolBar1->ToggleTool(wxID_PLAY,true);
		}

		printf("attempt to take NEXT lock\n");fflush(stdout);
		m_mtx_videocv.Lock();
		printf("NEXT lock taken\n");fflush(stdout);
		 lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
		 framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
		if(framepos>=lastframe)
		{

			videocv.set(CV_CAP_PROP_POS_FRAMES, lastframe);

			wxMessageBox(wxString("Last Frame reached."),
					 _T("Next Button clicked"),
					 wxOK | wxICON_INFORMATION,
					 this);
		}else{

			videocv.set(CV_CAP_PROP_POS_FRAMES, framepos+1 );

		}
		m_mtx_videocv.Unlock();
		printf("NEXT lock released\n");fflush(stdout);

		break;
	}
	case wxID_ADDPTS:
		if (!this->GetThread())
		{
			m_toolBar2->ToggleTool(wxID_ADDPTS,false);
			return;
		}
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
			m_menuOpciones->Check(wxID_SEEPOINTS,true);
		}

		if(m_toolBar2->GetToolState(wxID_ADDPTS)){
			reference_mode=RF_ADD_PTS;
			m_toolBar1->ToggleTool(wxID_PLAY,true);
			if(m_toolBar2->GetToolState(wxID_RMVPTS)||m_toolBar1->GetToolState(wxID_DETECT)||m_toolBar5->GetToolState(wxID_VRDT)
					||m_toolBar5->GetToolState(wxID_3PTS)||m_toolBar4->GetToolState(wxID_2PTS))
			{
				m_toolBar2->ToggleTool(wxID_RMVPTS,false);
				m_toolBar5->ToggleTool(wxID_VRDT,false);
				m_toolBar5->ToggleTool(wxID_3PTS,false);
				m_toolBar4->ToggleTool(wxID_2PTS,false);
				m_toolBar1->ToggleTool(wxID_DETECT,false);}
		}else
			reference_mode=RF_NOTHING;
		break;
	case wxID_RMVPTS:
		if (!this->GetThread())
		{
			m_toolBar2->ToggleTool(wxID_RMVPTS,false);
			return;
		}
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
			m_menuOpciones->Check(wxID_SEEPOINTS,true);
		}

		if(m_toolBar2->GetToolState(wxID_RMVPTS)){
			reference_mode=RF_RMV_PTS ;
			m_toolBar1->ToggleTool(wxID_PLAY,true);
			if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar1->GetToolState(wxID_DETECT)||m_toolBar5->GetToolState(wxID_VRDT)
					||m_toolBar5->GetToolState(wxID_3PTS)||m_toolBar4->GetToolState(wxID_2PTS))
			{
				m_toolBar2->ToggleTool(wxID_ADDPTS,false);
				m_toolBar1->ToggleTool(wxID_DETECT,false);
				m_toolBar5->ToggleTool(wxID_VRDT,false);
				m_toolBar5->ToggleTool(wxID_3PTS,false);
				m_toolBar4->ToggleTool(wxID_2PTS,false);
			}
		}else
			reference_mode=RF_NOTHING;
		break;
	case wxID_DISTANCE:
		if (!this->GetThread()) return;
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
		}
		reference_mode=RF_DISTANCE;
		m_toolBar1->ToggleTool(wxID_PLAY,true);
		if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)||m_toolBar1->GetToolState(wxID_DETECT)||
				m_toolBar5->GetToolState(wxID_VRDT)||m_toolBar5->GetToolState(wxID_3PTS)||m_toolBar4->GetToolState(wxID_2PTS))
		{
			m_toolBar2->ToggleTool(wxID_ADDPTS,false);
			m_toolBar2->ToggleTool(wxID_RMVPTS,false);
			m_toolBar1->ToggleTool(wxID_DETECT,false);
			m_toolBar5->ToggleTool(wxID_VRDT,false);
			m_toolBar5->ToggleTool(wxID_3PTS,false);
			m_toolBar4->ToggleTool(wxID_2PTS,false);
		}
		break;
	case wxID_REALDIST:
		if (!this->GetThread()) return;
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
		}
		if(dist_ref!=0){
			reference_mode=RF_REALDIST;
			m_toolBar1->ToggleTool(wxID_PLAY,true);
		}else{
			wxMessageBox(wxString::Format(("Para calcular la distancia real necesitas primero conocer la distancia de referencia.")),
								 _T("Error: Se ha encontrado un problema"),
								 wxOK | wxICON_INFORMATION,
								 this);
		}
		break;
	case wxID_STOP:
	{
		if (!this->GetThread()) return;
		m_mtx_videocv.Lock();
		videocv.set(CV_CAP_PROP_POS_FRAMES, 0);
		m_mtx_videocv.Unlock();
		m_slider1->SetValue(0);
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
			m_toolBar1->SetToolNormalBitmap(wxID_PLAY,wxBitmap( wxT("res/player_play.bmp"), wxBITMAP_TYPE_ANY ));
			m_toolBar1->SetToolShortHelp(wxID_PLAY,wxT("Reproduce el video"));
			m_toolBar1->SetToolLongHelp(wxID_PLAY,wxT("Reproduce el video"));
			m_toolBar1->ToggleTool(wxID_PLAY,true);

		}
		break;
	}
	case wxID_DETECT:
	{

		if(m_toolBar1->GetToolState(wxID_DETECT)){
			reference_mode=RF_AUTODETECT;
			if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)||m_toolBar5->GetToolState(wxID_VRDT)
					||m_toolBar5->GetToolState(wxID_3PTS)||m_toolBar4->GetToolState(wxID_2PTS))
			{
				m_toolBar2->ToggleTool(wxID_ADDPTS,false);
				m_toolBar2->ToggleTool(wxID_RMVPTS,false);
				m_toolBar5->ToggleTool(wxID_VRDT,false);
				m_toolBar5->ToggleTool(wxID_3PTS,false);
				m_toolBar4->ToggleTool(wxID_2PTS,false);
			}
		}else
			reference_mode=RF_NOTHING;
		break;
	}
	case wxID_VRDT:
	{
		if (!this->GetThread())
		{
			m_toolBar1->ToggleTool(wxID_VRDT,false);
			return;
		}
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
			m_menuOpciones->Check(wxID_SEEPOINTS,true);
		}
		if(dist_ref!=0){
			reference_mode=RF_REALDIST;
		}else{
			wxMessageBox(wxString::Format(("Para calcular la distancia real necesitas primero conocer la distancia de referencia.")),
								 _T("Error: Se ha encontrado un problema"),
								 wxOK | wxICON_INFORMATION,
								 this);
		}
		if(m_toolBar5->GetToolState(wxID_VRDT)){
			reference_mode=RF_VRDT;
			m_toolBar1->ToggleTool(wxID_PLAY,true);
			if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)||m_toolBar1->GetToolState(wxID_DETECT)
					||m_toolBar5->GetToolState(wxID_3PTS)||m_toolBar4->GetToolState(wxID_2PTS))
			{
				m_toolBar2->ToggleTool(wxID_ADDPTS,false);
				m_toolBar2->ToggleTool(wxID_RMVPTS,false);
				m_toolBar1->ToggleTool(wxID_DETECT,false);
				m_toolBar5->ToggleTool(wxID_3PTS,false);
				m_toolBar4->ToggleTool(wxID_2PTS,false);
			}
		}else
			reference_mode=RF_NOTHING;
		break;
	}
	case wxID_2PTS:
	{
		if (!this->GetThread()) return;

		if(m_toolBar4->GetToolState(wxID_2PTS)){
			if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)||m_toolBar1->GetToolState(wxID_DETECT)
					||m_toolBar5->GetToolState(wxID_VRDT)||m_toolBar5->GetToolState(wxID_3PTS))
			{
				m_toolBar2->ToggleTool(wxID_ADDPTS,false);
				m_toolBar2->ToggleTool(wxID_RMVPTS,false);
				m_toolBar5->ToggleTool(wxID_VRDT,false);
				m_toolBar1->ToggleTool(wxID_DETECT,false);
				m_toolBar5->ToggleTool(wxID_3PTS,false);
			}

			if(dist_ref!=0){
				reference_mode=RF_2PTS;
				multiref_n=0;
				//ADD LAYERS HERE

				mpFXYVector* vectorLayer = new mpFXYVector(wxString::Format(wxT("Distancia_%i"),numln));
				numln++;
				vectorLayer->SetContinuity(true);

				vectorLayer->SetPen(wxPen(EligeColor->GetColour()));
				vectorLayer->SetDrawOutsideMargins(true);
				vectorLayer->ShowName(false);
				m_plot2->AddLayer(     vectorLayer );
			}else{
				wxMessageBox(wxString::Format(("Para calcular la distancia real necesitas primero conocer la distancia de referencia.")),
									 _T("Error: Se ha encontrado un problema"),
									 wxOK | wxICON_INFORMATION,
									 this);
			}
		}
		break;
	}
	case wxID_3PTS:
	{
		if (!this->GetThread()) return;
		if (!this->GetThread()->IsPaused())
		{
			this->GetThread()->Pause();
		}
		m_toolBar1->ToggleTool(wxID_PLAY,true);
		if(m_toolBar5->GetToolState(wxID_3PTS)){
			if(m_toolBar2->GetToolState(wxID_ADDPTS)||m_toolBar2->GetToolState(wxID_RMVPTS)||m_toolBar1->GetToolState(wxID_DETECT)
					||m_toolBar5->GetToolState(wxID_VRDT)||m_toolBar4->GetToolState(wxID_2PTS))
			{
				m_toolBar2->ToggleTool(wxID_ADDPTS,false);
				m_toolBar2->ToggleTool(wxID_RMVPTS,false);
				m_toolBar5->ToggleTool(wxID_VRDT,false);
				m_toolBar1->ToggleTool(wxID_DETECT,false);
				m_toolBar4->ToggleTool(wxID_2PTS,false);
			}

			reference_mode=RF_3PTS;
			multiref_n=0;
		}
		break;
	}
	case wxID_INI:
	{
		if (!this->GetThread()) return;
		if ( !videocv.isOpened() ){
			wxMessageBox(wxString::Format(("No hay video en reproducción.")),
			                 _T("Ha surgido un problema."),
			                 wxOK | wxICON_INFORMATION,
			                 this);
		}else{
			firsttimeframe=videocv.get(CV_CAP_PROP_POS_FRAMES);
			m_slider1->SetTick(firsttimeframe);
		}
		break;
	}
	case wxID_CALC:
	{
		if (!this->GetThread()) return;
		if ( !videocv.isOpened() ){
			wxMessageBox(wxString::Format(("No hay video en reproducción.")),
							 _T("Ha surgido un problema."),
			                 wxOK | wxICON_INFORMATION,
			                 this);
		}else{
			if(firsttimeframe<lasttimeframe){
				int new_fps = wxGetNumberFromUser(wxT("Elija un nuevo valor de FPS"),wxT("FPS"),wxT("FPS"),fps_calc,1,999);
				if (new_fps >0) fps_calc = new_fps;
				float tiempo = (float)(lasttimeframe-firsttimeframe)/(float)fps_calc;
				wxMessageBox(wxString::Format(("El tiempo entre el frame %i y el frame %i es de %f segundos."),firsttimeframe,lasttimeframe,tiempo),
											 _T("Tiempo calculado"),
											 wxOK | wxICON_INFORMATION,
											 this);
			}else{
				wxMessageBox(wxString::Format(("Posicion de los frames marcados incorrectos. \nEl final debe ser posterior al primero.")),
				                 _T("Ha surgido un problema."),
				                 wxOK | wxICON_INFORMATION,
				                 this);
			}
		}

		break;
	}
	case wxID_END:
	{
		if (!this->GetThread()) return;
		if ( !videocv.isOpened() ){
			wxMessageBox(wxString::Format(("No hay video en reproducción.")),
			                 _T("Ha surgido un problema."),
			                 wxOK | wxICON_INFORMATION,
			                 this);
		}else{
			lasttimeframe=videocv.get(CV_CAP_PROP_POS_FRAMES);
			m_slider1->SetTick(lasttimeframe);
		}
		break;
	}
	case wxID_HOR:
	{
		if (!this->GetThread()) return;
		if ( !videocv.isOpened() ){
			wxMessageBox(wxString::Format(("No hay video en reproducción.")),
			                 _T("Ha surgido un problema."),
			                 wxOK | wxICON_INFORMATION,
			                 this);
		}else{
			reference_mode=RF_HOR;
		}
		break;
	}
	default: break;
	}
	fillTable();
	this->Layout();
}

void MainFrame::onMouseRelease(wxMouseEvent& event)
{

	int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
	event.Skip();
	if(reference_mode){
		wxPoint pos = ScreenToClient(wxGetMousePosition());
		int win_width	= m_camera_panel->GetClientSize().GetX();
		int win_height	= m_camera_panel->GetClientSize().GetY();
		if (pos.x >= w_offpos && pos.x <= win_width - w_offpos &&	pos.y >= h_offpos && pos.y <= - h_offpos + win_height)
		{
			float distanciareal;
			switch(reference_mode)
			{
			case RF_DISTANCE:
				ref_0=pos;
				reference_mode++;
				break;
			case RF_DISTANCE_2 :
				dist_ref=sqrt(pow((double)(ref_0.x-pos.x),2)+pow((double)(ref_0.y-pos.y),2));

				dist_med = wxGetNumberFromUser(wxT("Elija el valor real de distancia"),wxT("Distancia"),wxT("Distancia"),30,1,300);
				razon=dist_med/dist_ref;
				points.razondistancia(razon);

				if(m_toolBar2->GetToolState(wxID_ADDPTS))
					reference_mode=RF_ADD_PTS ;
				else
					reference_mode=RF_NOTHING ;
				break;
			case RF_REALDIST :
				ref_0=pos;
				reference_mode++;
				break;
			case RF_REALDIST_2 :
				distanciareal=razon*sqrt(pow((double)(ref_0.x-pos.x),2)+pow((double)(ref_0.y-pos.y),2));
				wxMessageBox(wxString::Format(("Distancia real es %f."),distanciareal),
									 _T("Distancia calculada"),
									 wxOK | wxICON_INFORMATION,
									 this);

				if(m_toolBar2->GetToolState(wxID_ADDPTS))
					reference_mode=RF_ADD_PTS ;
				else
					reference_mode=RF_REALDIST;
				break;
			case RF_ADD_PTS :
				if(m_toolBar2->GetToolState(wxID_ADDPTS)){
					points.savePoint ( framepos, pos );
					//refresh listbox
				}
				break;
			case RF_RMV_PTS  :
				if(m_toolBar2->GetToolState(wxID_RMVPTS)){
					if( points.erasePoint ( framepos, pos ) < 0 )
						wxMessageBox(wxString::Format(("Nada que borrar.")),
															 _T("Error de borrado"),
															 wxOK | wxICON_INFORMATION,
															 this);
					//refresh listbox
				}
				break;
			case RF_VRDT :
				if(m_toolBar5->GetToolState(wxID_VRDT)){
					//Si en el punto marcado no se encuentra un punto o una linea, muestra el mensaje
					if(points.selectPoint( framepos, pos ) < 0)
						//if( points.selectLine ( videocv.get(CV_CAP_PROP_POS_FRAMES), pos ) < 0 )
							wxMessageBox(wxString::Format(("Nada que mostrar.")),
																 _T("Error de seleccion"),
																 wxOK | wxICON_INFORMATION,
																 this);
					//refresh listbox
				}
				break;
			case RF_2PTS:
				if(m_toolBar4->GetToolState(wxID_2PTS)){
					//Si el punto ya existe, tomar ese punto en su lugar, sino, añadirlo
					if(points.selectPoint( framepos, pos ) < 0){
						points.savePoint ( framepos, pos );
					}
					int aux;
					aux=points.markLine(framepos,pos);
					if(aux<0)
						wxMessageBox(wxString::Format(("Error.")));

					fillTable();
				}
				::wxBell();
				break;
				case RF_3PTS:
					if(m_toolBar5->GetToolState(wxID_3PTS)){
						switch(multiref_n)
						{
						case 0:
							delete[] multiref;
							multiref=new wxPoint[3];
							//Si el punto ya existe, tomar ese punto en su lugar, sino, añadirlo
							if(points.selectPoint( framepos, pos ) < 0){
								points.savePoint ( framepos, pos );
							}
							multiref[0]=pos;
							multiref_n++;

							m_statusBar1->SetStatusText(wxString::Format(_("o")),0);
							::wxBell();
							break;
						case 1:
							//Si el punto ya existe, tomar ese punto en su lugar, sino, añadirlo
							if(points.selectPoint( framepos, pos ) < 0){
								points.savePoint ( framepos, pos );
							}
							multiref[1]=pos;
							multiref_n++;

							m_statusBar1->SetStatusText(wxString::Format(_("oo")),0);
							::wxBell();
							break;
						case 2:
							//Si el punto ya existe, tomar ese punto en su lugar, sino, añadirlo
							if(points.selectPoint( framepos, pos ) < 0){
								points.savePoint ( framepos, pos );
							}
							multiref[2]=pos;

							m_statusBar1->SetStatusText(wxString::Format(_("ooo")),0);
							::wxBell();
							multiref_n=points.threePoints(framepos,multiref[1],multiref[0],multiref[2]);
							if(multiref_n<0)
								wxMessageBox(wxString::Format(("Error.")));
							if(multiref_n>0){

								//ADD LAYERS HERE

								mpFXYVector* vectorLayer = new mpFXYVector(wxString::Format(wxT("Angulo_%i"),numgraf));
								numgraf++;

								vectorLayer->SetContinuity(true);
								vectorLayer->SetPen(wxPen(EligeColor->GetColour()));
								vectorLayer->SetDrawOutsideMargins(true);
								vectorLayer->ShowName(false);
								m_plot->AddLayer(     vectorLayer );
								//END LAYERS
								fillTable();
							}
							if(multiref_n==0)
								wxMessageBox(wxString::Format(("Puntos no encontrados en la imagen.")));
							multiref_n=0;
							break;
					default:
						break;
					}
				}
				break;
				case RF_HOR:
					/*Establece umbral horizontal*/
					umbral_hor=pos.y;
					break;
				default:break;
			}

			wxQueueEvent(this, new wxThreadEvent(wxEVT_COMMAND_THREAD_UPDATE));
			this->GetThread()->Sleep(1000/video_fps);
		}
	}
}
void MainFrame::onMouseMove(wxMouseEvent& event)
{

	int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
	wxPoint pos = ScreenToClient(wxGetMousePosition());
	if(m_menuOpciones->IsChecked(wxID_MOUSELOC)){
		m_statusBar1->SetStatusText(wxString::Format(
				_("Mouse: %i - %i FPS: %f"),
				pos.x, pos.y,
				video_fps),0);
	}
	multimap< int, wxPoint>::iterator ultimo;

	wxPoint* vector;
	vector = new wxPoint[points.count(framepos)];
	int puntosenelframe = points.loadRange(framepos,vector);
	if (event.Dragging()){
    		/*Arrastrar el ratón mientras se añaden puntos*/
	    	if((reference_mode==RF_ADD_PTS)&&(puntosenelframe>=2))
	    	{

	            wxClientDC dic(m_camera_panel);
	    		this->Render(dic);
	    		if(m_menuOpciones->IsChecked(wxID_SEEPOINTS))
	    			this->Renderpoints(dic);
	            wxPen pen(*wxRED, 1); // red pen of width 1
	            dic.SetPen(pen);
				dic.DrawLine(pos,vector[puntosenelframe-1]);
	            wxPen pen2(*wxRED, 1,wxDOT); // red pen of width 1
	            dic.SetPen(pen2);
				dic.DrawLine(pos,vector[puntosenelframe-2]);
				dic.SetPen(wxNullPen);
	    	}
	    	/*Arrastrar el ratón mientras se calcula la medida de referencia (RF_DISTANCE)*/

	    	if(reference_mode==RF_DISTANCE)
	    	{
	    		ref_0=pos;
				reference_mode++;
	    	}
	    	if(reference_mode==RF_DISTANCE_2)
	    	{
	            wxClientDC dic(m_camera_panel);
	    		this->Render(dic);
	    		if(m_menuOpciones->IsChecked(wxID_SEEPOINTS))
	    			this->Renderpoints(dic);
	            wxPen pen(*wxRED, 1); // red pen of width 1
	            dic.SetPen(pen);
				dic.DrawLine(pos,ref_0);
				dic.SetPen(wxNullPen);
	    	}
	    	/*Arrastrar el ratón mientras se calculan distancias (RF_REALDIST)*/

	    	if(reference_mode==RF_REALDIST)
	    	{
	    		ref_0=pos;
				reference_mode++;
	    	}
	    	if(reference_mode==RF_REALDIST_2)
	    	{
	            wxClientDC dic(m_camera_panel);
	    		this->Render(dic);
	    		if(m_menuOpciones->IsChecked(wxID_SEEPOINTS))
	    			this->Renderpoints(dic);
	            wxPen pen(*wxRED, 1); // red pen of width 1
	            dic.SetPen(pen);
				dic.DrawLine(pos,ref_0);
				dic.SetPen(wxNullPen);
	    	}
	    	/*Arrastrar el ratón para ver la linea horizontal (RF_REALDIST)*/
	    	if(reference_mode==RF_HOR)
	    	{
	    		int img_width	= m_video_image.GetSize().GetX();
	            wxClientDC dic(m_camera_panel);
	    		this->Render(dic);
	    		if(m_menuOpciones->IsChecked(wxID_SEEPOINTS))
	    			this->Renderpoints(dic);
	            wxPen pen(*wxRED, 1); // red pen of width 1
	            dic.SetPen(pen);
				dic.DrawLine(wxPoint(0,pos.y),wxPoint(img_width,pos.y));
				dic.SetPen(wxNullPen);
	    	}
	}
	delete[] vector;
}

