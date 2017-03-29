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
 * filltable.cpp
 *
 *  Created on: 29/07/2014
 *      Author: Deriman Franco
 */





#include "wxVideoGUI.h"



int MainFrame::fillTable()
{
	printf("fillTable ");fflush(stdout);

	//Draw something else
	int k=0,j=0,g=0,q=0;
	int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);


	int frameite;
	wxPoint *vectorgrados,*vectordistancia;
	float *vc_grad,*vc_length;
	int* gruposenelframe,*grupos_aux, /* *grupos_aux2, */ *lineasenframes;
	std::vector< int> *grupos/*,*grlines*/;
	wxSystemSettings systemsetting;

	k=points.cuantosgruposdeangulos(&grupos);
	q=points.countlines();

	if((k==0)&&(q==0)) return 0;

	if(k>0){
		grupos_aux = grupos->data();
	}

	//Ajusta el tamaño de la grafica según el tamaño de la ventana (it shouldnt use m_camera_panel)
	wxSize maxsize(m_camera_panel->GetClientSize().GetX()/2,m_camera_panel->GetClientSize().GetY()/2);
	m_plot->SetMinClientSize(maxsize);
	m_plot2->SetMinClientSize(maxsize);

	//wxMathPlot code

	mpFXYVector* vectorLayer;
	std::vector<double> vectorx[5000], vectory[5000];
	std::vector<double> vector2x[5000], vector2y[5000];

	//Distancia de zancada

	int zancada_vector_pos = 0;
	int lineas=points.countlines();
	if(lineas>0){
		vectordistancia = new wxPoint[lineas];
		vc_length = new float[lineas];
		lineasenframes = new  int[lineas];
		lineas = points.showLines(lineasenframes,vc_length,vectordistancia); //Frame en el que está la linea, distancia calculada, extremo del segmento
	}

	//Preparación para los grados
	j=0; g=0;
	grmin=360;grmax=0;

	//FOR EACH FRAMES
	for(frameite=0;frameite<=lastframe;frameite++)
	{
		//WATCH IF GRADOS EXISTS

		vectorgrados = new wxPoint[points.countgrad(frameite)*3];
		vc_grad = new float[points.countgrad(frameite)];
		gruposenelframe = new  int[points.countgrad(frameite)];
		int puntosdegrado = points.showGrad(frameite,vc_grad,vectorgrados,gruposenelframe);

		if (puntosdegrado>0){
			for(int i=0;i<puntosdegrado;i++){
				//Escribe en wxGrid
				if(i+j>=angleGrid->GetNumberRows()) angleGrid->AppendRows();
				//"Value = Frame"
				angleGrid->SetCellValue(i+j,0,wxString::Format("%i",frameite));	//Escribe en wxGrid
				//Value = Grados
				angleGrid->SetCellValue(i+j,1,wxString::Format("%.2f",vc_grad[i]));	 //Escribe en wxGrid
				angleGrid->SetCellValue(i+j,2,wxString::Format("(%i,%i)",vectorgrados[i*3].x,vectorgrados[i*3].y));
				angleGrid->SetCellBackgroundColour(i+j,0,systemsetting.GetColour(wxSYS_COLOUR_WINDOW   ) );
				angleGrid->SetCellBackgroundColour(i+j,1,systemsetting.GetColour(wxSYS_COLOUR_WINDOW   ) );
				angleGrid->SetCellBackgroundColour(i+j,2,systemsetting.GetColour(wxSYS_COLOUR_WINDOW   ) );
				if(vc_grad[i]>grmax) {grmax=vc_grad[i];}
				if(vc_grad[i]<grmin) {grmin=vc_grad[i];}
				//almacena en vectorx/y para visualizar en el Plot
				for(int u=0;u<k;u++){
					if(gruposenelframe[i]==grupos_aux[u]){
						vectory[u].push_back(vc_grad[i]);
						vectorx[u].push_back(frameite);
					}
				}
			}

			j+=puntosdegrado;//J Deberia contar cada entrada hecha
		}
		delete[] vectorgrados;
		delete[] vc_grad;
		delete[] gruposenelframe;

		//Distancias de zancada
		if(lineas>0 && lineas>zancada_vector_pos){
			if (lineasenframes[zancada_vector_pos]==frameite)
			{
				g=1;
					//Escribe en wxGrid
					if(lineas>lineGrid->GetNumberRows()) lineGrid->AppendRows();
					//"Value = Frame"
					lineGrid->SetCellValue(zancada_vector_pos,0,wxString::Format("%i",frameite));	//Escribe en wxGrid
					//Value = Distancia
					lineGrid->SetCellValue(zancada_vector_pos,1,wxString::Format("%.2f",vc_length[zancada_vector_pos]));	 //Escribe en wxGrid
					lineGrid->SetCellValue(zancada_vector_pos,2,wxString::Format("(%i,%i)",vectordistancia[zancada_vector_pos].x,vectordistancia[zancada_vector_pos].y));
					lineGrid->SetCellBackgroundColour(zancada_vector_pos,0,systemsetting.GetColour(wxSYS_COLOUR_WINDOW   ) );
					lineGrid->SetCellBackgroundColour(zancada_vector_pos,1,systemsetting.GetColour(wxSYS_COLOUR_WINDOW   ) );
					lineGrid->SetCellBackgroundColour(zancada_vector_pos,2,systemsetting.GetColour(wxSYS_COLOUR_WINDOW   ) );

					//almacena en mglData para visualizar en el Plot

							vector2y[0].push_back(vc_length[zancada_vector_pos]);
							vector2x[0].push_back(frameite);
							zancada_vector_pos++;
			}
		}
	}
	if(lineas>0){
		delete[] vectordistancia;
		delete[] vc_length;
		delete[] lineasenframes;
	}
	/* Pintar grafica de angulos*/
	for(int u=0;u<numgraf;u++){
		//FIND LAYER / SETDATA
		vectorLayer=(mpFXYVector*)m_plot->GetLayerByName(wxString::Format(wxT("Angulo_%i"),u));
		printf("[Setgr]");fflush(stdout);
		if(vectorLayer!=NULL){
			vectorLayer->SetData(vectorx[u], vectory[u]);
		}else{
			printf("[Filltable not updated %i-%i]",u+1,numgraf);
		}
		fflush(stdout);
	}
	m_plot->UpdateAll();


	/* Pintar grafica de lineas*/

	for(int u=0;u<numln;u++){
		//FIND LAYER / SETDATA
		vectorLayer=(mpFXYVector*)m_plot2->GetLayerByName(wxString::Format(wxT("Distancia_%i"),u));
		printf("[Setln]");fflush(stdout);
		if(vectorLayer!=NULL){
			vectorLayer->SetData(vector2x[u], vector2y[u]);
		}else{
			printf("[Filltable not updated %i-%i]",u+1,numln);
		}
		fflush(stdout);
	}
	m_plot2->UpdateAll();

	printf("[plot]\n");fflush(stdout);


	printf("End fillTable\n");fflush(stdout);
	if ((j>0)||(g>0)) return 1;
	else return -1;
}
