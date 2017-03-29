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
 * OnSlider.cpp
 *
 *  Created on: 29/07/2014
 *      Author: Deriman Franco
 */


#include "wxVideoGUI.h"


void MainFrame::OnSliderUpdate (wxScrollEvent& event)
{
	int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);

	if (!this->GetThread()) return;
	int time=event.GetPosition();
	int newpos=(time * lastframe)/100;
	m_mtx_videocv.Lock();
	videocv.set(CV_CAP_PROP_POS_FRAMES, newpos );
	m_mtx_videocv.Unlock();
	if (this->GetThread()->IsPaused())
	{
		this->GetThread()->Resume();
	}

}
void MainFrame::OnSliderChanged (wxScrollEvent& event)
{
	int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);

	int time=event.GetPosition();
	int newpos=(time * lastframe)/100;
	m_statusBar1->SetStatusText(wxString::Format(_("%i"),newpos));

}
void MainFrame::OnSliderGrab (wxScrollEvent& event)
{
	if (!this->GetThread()->IsPaused())
	{
		this->GetThread()->Pause();

	}

}

void MainFrame::OnSliderAutoUpdate (wxCommandEvent& event)
{
	m_mtx_slider.Lock();
	int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
	int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
	printf("Slider draw Frames %i - %i.\n",framepos,lastframe);fflush(stdout);

	m_slider1->SetValue(100*framepos/lastframe);

	//Señala la posición y el total de frames a izquierda y derecha del Slider respectivamente
	wxString str = wxString::Format(wxT("Frame %i"),framepos);
	SliderInfo->SetLabel(str);
	str = wxString::Format(wxT("%i"),lastframe);
	SliderInfo2->SetLabel(str);


	//Codigo para pintar la posición en la grafica
	if(m_plot->IsShown()){
		mpFXYVector* vectorLayer ;
		vectorLayer=(mpFXYVector*)m_plot->GetLayerByName(wxString::Format(wxT("framepos")));
		if(vectorLayer) m_plot->DelLayer(vectorLayer,true,true);
		vectorLayer  = new mpFXYVector(_("framepos"));;
		vector<double> posX,posY;
		posX.push_back(framepos);
		posX.push_back(framepos);
		posY.push_back(grmin);
		posY.push_back(grmax);
		vectorLayer->SetData(posX,posY);
		vectorLayer->SetContinuity(true);
		wxPen vectorpen(*wxRED, 1, wxSOLID);
		vectorLayer->SetPen(vectorpen);
		vectorLayer->ShowName(false);
		m_plot->AddLayer(vectorLayer);
		m_plot->UpdateAll();
	}

	//Codigo para pintar la posición en la grafica
	if(m_plot2->IsShown()){
		mpFXYVector* vectorLayer ;
		vectorLayer=(mpFXYVector*)m_plot2->GetLayerByName(wxString::Format(wxT("framepos")));
		if(vectorLayer) m_plot2->DelLayer(vectorLayer,true,true);
		vectorLayer  = new mpFXYVector(_("framepos"));;
		vector<double> posX,posY;
		posX.push_back(framepos);
		posX.push_back(framepos);
		posY.push_back(lnmin);
		posY.push_back(lnmax);
		vectorLayer->SetData(posX,posY);
		vectorLayer->SetContinuity(true);
		wxPen vectorpen(*wxRED, 1, wxSOLID);
		vectorLayer->SetPen(vectorpen);
		vectorLayer->ShowName(false);
		m_plot2->AddLayer(vectorLayer);
		m_plot2->UpdateAll();
	}

	//Control de notas por frames

	wxString buffer;
	if(framepos!=lastframenoted){
		if(points.imprimeNota(framepos,&buffer)>0){
			lastframenoted=framepos;
			FrameIndex2->SetSelection(FrameIndex2->FindString(wxString::Format(wxT("%i"),framepos)));
			FrameIndex1->SetSelection(FrameIndex1->FindString(wxString::Format(wxT("%i"),framepos)));
			m_textCtrl1->Clear();
			m_textCtrl2->Clear();
			(*m_textCtrl1) << buffer;
			(*m_textCtrl2) << buffer;
		}
	}

	//Libera el Bloqueo
	m_mtx_slider.Unlock();
}
