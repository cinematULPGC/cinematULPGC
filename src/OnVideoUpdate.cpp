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
 * OnVideoUpdate.cpp
 *
 *  Created on: 29/07/2014
 *      Author: Deriman Franco
 */



#include "wxVideoGUI.h"



void MainFrame::OnThreadUpdate(wxCommandEvent& event)
{
	wxClientDC dc(m_camera_panel);
	this->Render(dc);
	if(m_menuOpciones->IsChecked(wxID_SEEPOINTS))
		this->Renderpoints(dc);
}

void MainFrame::Render(wxDC &dc)
{

	if (!dc.IsOk() || !m_video_image.IsOk()) return;

	int win_width	= m_camera_panel->GetClientSize().GetX();
	int win_height	= m_camera_panel->GetClientSize().GetY();

	wxCriticalSectionLocker lock(m_cs_image);

	int img_width	= m_video_image.GetSize().GetX();
	int img_height	= m_video_image.GetSize().GetY();

	int new_img_width	= (img_width	*	win_height)		/	img_height;
	int new_img_height	= (img_height	*	win_width)		/	img_width;
	int width_diff		= (win_width	-	new_img_width)	/	2;
	int height_diff		= (win_height	-	new_img_height)	/	2;


	if (height_diff >= 0)
	{
		dc.DrawBitmap(wxBitmap(m_video_image.Scale(win_width, new_img_height)), 0, height_diff);
		h_offpos = height_diff;
		w_offpos = 0;
		points.getReferences(img_height,img_width,new_img_height,win_width,h_offpos,w_offpos);
	} else
	{
		dc.DrawBitmap(wxBitmap(m_video_image.Scale(new_img_width, win_height)), width_diff, 0);
		w_offpos = width_diff;
		h_offpos = 0;
		points.getReferences(img_height,img_width,win_height,new_img_width,h_offpos,w_offpos);
	}


}

void MainFrame::Renderpoints(wxDC &dc)
{

	if (!dc.IsOk() || !m_video_image.IsOk()) return;

	wxCriticalSectionLocker lock(m_cs_image);

	//Draw something else
	int j=0;
	int k=0;
	int framepos= videocv.get(CV_CAP_PROP_POS_FRAMES);
	wxPoint * vector,*vectorgrados;
	float *vc_grad;
	vector = new wxPoint[points.count(framepos)];
	vectorgrados = new wxPoint[points.countgrad(framepos)*3];
	vc_grad = new float[points.countgrad(framepos)];
	int puntosenelframe = points.loadRange(framepos,vector);
	int puntosdegrado = points.showGrad(framepos,vc_grad,vectorgrados);
	/*Dibuja los puntos en la imagen*/
	if (puntosenelframe > 0){
		for (int i=0;i<puntosenelframe;i++)
		{
			dc.DrawCircle(vector[i],3);
		}

		for (int i=0;i<puntosdegrado;i++)
		{
			{
				dc.SetBackgroundMode(wxSOLID);
				dc.SetTextBackground(*wxWHITE);
				{
				dc.DrawText( //Ley de los cosenos anglePoints(lepunto,leprev,lepost)
						wxString::Format(("%.2fº"),vc_grad[i])
					,vectorgrados[i*3].x+5,vectorgrados[i*3].y);
				}
				dc.SetPen(*wxWHITE);
				dc.DrawLine(vectorgrados[i*3+1],vectorgrados[i*3]);
				dc.DrawLine(vectorgrados[i*3],vectorgrados[i*3+2]);
				k++;j++;
			}
		}
	}
	if(reference_mode==2){
		dc.SetPen(*wxGREEN);
		dc.SetBrush(*wxGREEN);
		dc.DrawCircle(ref_0,3);
	}
	if(reference_mode==4){
		dc.SetPen(*wxRED);
		dc.SetBrush(*wxRED);
		dc.DrawCircle(ref_0,3);
	}
	delete[] vector;

}


