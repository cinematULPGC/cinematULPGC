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
 * EntryThread.cpp
 *
 *  Created on: 29/07/2014
 *      Author: Deriman Franco
 */





#include "wxVideoGUI.h"


wxThread::ExitCode MainFrame::Entry()
{
	printf("filename\n");fflush(stdout);
    wxString filename = wxFileSelector("Choose a video file to open", "", "","AVI|MKV|MP4|MPEG", "AVI files (*.avi)|*.avi|MKV files (*.mkv)|*.mkv|MP4 files (*.mp4)|*.mp4|MPEG files (*.mpeg)|*.mpeg|Mostrar todo (*.*)|*.*",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if ( !filename.empty() )
	{
	    // work with the file
		char buf[200];
		strcpy( buf, (const char*)filename.mb_str(wxConvUTF8) );
		videocv.open(buf);

		printf("open\n");fflush(stdout);
	    if ( !videocv.isOpened() )
	    {
		    wxMessageBox(wxString::Format(("The file %s couldn't be opened."),buf),
		                 _T("Something wrong happened..."),
		                 wxOK | wxICON_INFORMATION,
		                 this);
			return (wxThread::ExitCode)0;
	    }
		//Activa todas las opciones disponibles al reconocer una reproducción
	    {
			m_button1->Enable(true);
			m_button2->Enable(true);
	    }

	    /*Getting the FourCC code*/
	    int ex = static_cast<int>(videocv.get(CV_CAP_PROP_FOURCC));
	    union { int v; char c[5];} uEx ;
	      uEx.v = ex;                              // From Int to char via union
	      uEx.c[4]='\0';


	    video_fps = videocv.get(CV_CAP_PROP_FPS);

		printf("%f\n",video_fps);fflush(stdout);

	    cv::Mat captured_image;
		while (!this->GetThread()->TestDestroy())
		{
			m_mtx_videocv.Lock();
			printf("while lock taken\n");fflush(stdout);
			/* Compruebo si la posición del Frame está fuera de los limites del video.
			 * De ser así. Se comprueba si la opción marcada es la de repetir la reproducción, o parar.
			 * En caso de repetir, Se devuelve la posición del Frame al principio y desactiva la detección
			 * automatica de puntos por flujo óptico.
			 **/
			int lastframe = videocv.get(CV_CAP_PROP_FRAME_COUNT);
			int framepos = videocv.get(CV_CAP_PROP_POS_FRAMES);
			if (lastframe<=framepos){
				if(m_menuOpciones->IsChecked(wxID_REPEAT)){

					videocv.set(CV_CAP_PROP_POS_FRAMES, 0 );

					if(m_toolBar1->GetToolState(wxID_DETECT)){
						m_toolBar1->ToggleTool(wxID_DETECT,false);
						reference_mode=RF_NOTHING;
					}
					old_image_rp*=0;

				}else{
					this->GetThread()->Pause();
				}
			}
			/*------------*/
			if(m_toolBar1->GetToolState(wxID_PLAY)){

				videocv.set(CV_CAP_PROP_POS_FRAMES, framepos-1 );

			}
			/*----------------*/
			printf("%c%c%c%c\n",uEx.c[0],uEx.c[1],uEx.c[2],uEx.c[3]);fflush(stdout);
			try
			{
			    wxCriticalSectionLocker lock(m_cs_image);
				if(!videocv.read(captured_image))
					throw 1;

			}catch(int a)
			{
				printf("Error durante la reproducción\n");fflush(stdout);

				wxMessageBox(wxString("Se ha producido un error durante la reproducción.\n"
						 "Para evitar problemas mayores se ha liberado el video del buffer."),
						 _T("Error durante la captura"),
						 wxOK | wxICON_INFORMATION,
						 this);
				return (wxThread::ExitCode)0;
			}
			framepos= videocv.get(CV_CAP_PROP_POS_FRAMES);
			printf("Frame %i captured\n",framepos);fflush(stdout);

			if (!captured_image.empty()) //Normalmente debido al final del video.
			{
				if (reference_mode == RF_AUTODETECT)
				{
					/* Necesito crear un old_image_rp a su justa medida sin necesidad de usar
					 *  tantos clone y copyTo. Pero del otro modo la imagen que ve el opticalFlow
					 *  corresponde al mismo Frame.
					 *  Meaning: Los punteros old_image y captured_frame correspondian a la misma matriz.*/
					cv::Mat old_image=captured_image.clone();
					if(!old_image_rp.empty()){
						old_image_rp.copyTo(old_image);
					}
					cv::Mat aux_image=captured_image.clone();
					printf("{\nclone image\n");
					fflush(stdout);
					//APLICAR FUNCION FLUJO OPTICO
					//**DEVUELVE LA IMAGEN COMBINADA DE AMBOS
					std::vector<cv::Point2f> vectordepuntos,vectorMarcados;
					std::vector<cv::Point2f>::iterator it;
					std::vector<cv::Point2f> vector_salida;
					if (framepos>1){
						wxPoint* Temp ;
						int* dirtybits;
						Temp = new wxPoint[points.count(framepos-1)];
						dirtybits = new int[points.count(framepos-1)];
						int puntosenelframe = points.loadDirtyRange(framepos-1,Temp,dirtybits,false);
						//para cambiar entre el un modo u otro
						int inputflow = 0;
						printf("{\nstart switch\n");
						fflush(stdout);
						switch(inputflow){ //************************  SWITCH HERE
							case 0: //Deteccion por puntos
							if(puntosenelframe>0){
								//Es necesario convertir los puntos wxPoint en Point2f
								//it = vectordepuntos.begin();
								printf("dirty bits: ");
								fflush(stdout);
								for(int i=0;i<puntosenelframe;i++){
									printf("%i ",dirtybits[i]);
									fflush(stdout);
									if(dirtybits[i]>0){
										vectordepuntos.push_back(Point2f(Temp[i].x,Temp[i].y));
										points.checkDirtyBit(framepos-1,Temp[i]);
									}

								}
								if (vectordepuntos.size()>0){

									printf("{\nOptical Flow\n");
									fflush(stdout);
									vector_salida=flowclass.PRINCIPAL(old_image, captured_image, vectordepuntos);
									//Una vez ejecutada la función Principal  los puntos se añaden a la lista
									//Se trata con la función especial StaticPoint, la cual no aplica la escala a los
									//puntos encontrados dado que la función PRINCIPAL trata con el tamaño original

									printf("}\nSuccess! ");
									fflush(stdout);
									for(unsigned i=0;i<vector_salida.size();i++){
										points.savePoint ( framepos, wxPoint(vector_salida[i].x,vector_salida[i].y),false);
										printf("o");
										fflush(stdout);

										/*  Código para añadir los puntos en los que se delimita la zancada */
										if(points.countlines()>0){
											int lineaanterior=vectordepuntos[i].y-umbral_hor;
											int lineaposterior=vector_salida[i].y-umbral_hor;
											printf(" Zancada:  %i %i\n",lineaanterior,lineaposterior);
											fflush(stdout);
											if(lineaanterior*lineaposterior<=0){
												int k=points.markLine ( framepos,points.resizePoint(wxPoint(vector_salida[i].x,vector_salida[i].y)));
												if(k==0){
													printf("   Punto ya encontrado en el frame %i\n",framepos);
												}if(k<0){
													printf("   Error en frame %i\n",framepos);
												}if(k>0){
													printf("   Punto añadido en el frame %i\n",framepos);
												}
												fflush(stdout);
											}
										}
										/*  ------------------------------------------------------------------  */

									}
									printf("\n");
									fflush(stdout);

									delete[] Temp;
									vector_salida.clear();
								}
								/*
								 *
								 *
								 * ESPACIO RESERVADO PARA LA DETECCIÓN DE LOS ANGULOS Y RECTAS A LO LARGO DEL VIDEO
								 *
								 *
								 *
								 * */
								if(points.countgrad(framepos-1)>0){
									Temp = new wxPoint[3*points.countgrad(framepos-1)];
									wxPoint aux;
									float* grads;
									grads = new float[points.countgrad(framepos-1)];
									int* grupos;
									grupos = new  int[points.countgrad(framepos-1)];
									int* modes;
									modes = new  int[points.countgrad(framepos-1)];
									int k = points.showGrad(framepos-1,grads,Temp,grupos,modes);
									//Es necesario convertir los puntos wxPoint en Point2f

									printf("Temp %i: ",k);
									for(int i=0;i<k*3;i++){
										printf("%i ",i);
										aux=points.insizePoint(Temp[i]);

										vectorMarcados.push_back(Point2f(aux.x, aux.y));

									}
									printf("\n");
									fflush(stdout);
									vector_salida=flowclass.PRINCIPAL(old_image, captured_image, vectorMarcados);

									//Una vez ejecutada la función Principal  los puntos se añaden a la lista
									//Dado que la función PRINCIPAL trata con el tamaño original aplicamos insizePoint y resizePoint
									//para tratar los puntos como si se tratara de la imagen original.
									printf("vector_puntos: %i\n",vector_salida.size());
									fflush(stdout);
									for(unsigned i=0;i<vector_salida.size();i+=3){
										k=points.threePoints ( framepos,
													points.resizePoint(wxPoint(vector_salida[i].x,vector_salida[i].y)),
													points.resizePoint(wxPoint(vector_salida[i+1].x,vector_salida[i+1].y)),
													points.resizePoint(wxPoint(vector_salida[i+2].x,vector_salida[i+2].y)),
													grupos[i/3],modes[i]);
										if(k==0){
											printf("   Not enough points in frame %i\n",framepos);
										}if(k<0){
											printf("   error en frame %i\n",framepos);
										}if(k>0){
											printf("   Success en frame %i-grupo %i\n",framepos,grupos[i]);

										}
										fflush(stdout);
									}
									vector_salida.clear();
									vectorMarcados.clear();
								}

								/*
								 *
								 *
								 */
							}
							break;
							case 1: //Deteccion de flujo optico
							if (flowclass.opticalFLOW(old_image, captured_image, aux_image) != 0 ){
								wxMessageBox(wxString::Format(("Impossible to apply Optical FLOW algorithm.")),
											 _T("Something wrong happened..."),
											 wxOK | wxICON_INFORMATION,
											 this);
							}
							break;
							default: break;
						}
					}
					//Se guarda el frame actual para la siguiente iteración
					captured_image.copyTo(old_image_rp);
					aux_image.copyTo(captured_image);

				}
				cv::cvtColor(captured_image, captured_image, CV_BGR2RGB);
				{
					wxCriticalSectionLocker lock(m_cs_image);
					m_video_image = wxImage(captured_image.cols, captured_image.rows, captured_image.data, true);
				}

			}
			m_mtx_videocv.Unlock();
			printf("while lock released\n");fflush(stdout);
			//Llamada a SliderAutoUpdate
			wxQueueEvent(this, new wxThreadEvent(wxEVT_COMMAND_SLIDER_UPDATE));
			this->GetThread()->Sleep(1);
			//Thread to the queue
			wxQueueEvent(this, new wxThreadEvent(wxEVT_COMMAND_THREAD_UPDATE));
			this->GetThread()->Sleep(1000/video_fps);
		}

		videocv.release();
	}
	return (wxThread::ExitCode)0;
}
