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
 * pointhandler.h
 *
 *  Created on: 10/12/2012
 *      Author: Deriman Franco
 */

#ifndef POINTHANDLER_H_
#define POINTHANDLER_H_

#include <wx/gdicmn.h>
#include <wx/ffile.h>
#include <map>
#include <vector>
   using namespace std;


//Libreria wxJSON
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

class pointhandler
{
private:
	int					h_offpos, w_offpos;
	double				razon, hIn, wIn,dist_med;
	//Tipos
	typedef pair <wxPoint, int> t_pfconundrum; // [Punto|dirt]
	typedef pair <const int, t_pfconundrum> pointframeconundrum; // [Frame|[Punto|dirt]]


	typedef struct valor3puntos{
		wxPoint pto;
		float valor;//anteriormente grados
		wxPoint ant;
		wxPoint post;
		int modo;
		 int grupo;

		valor3puntos(wxPoint thePoint, float theValue, wxPoint thePrev, wxPoint thePost, int theMode,  int theGroup)
		{
			pto=thePoint;
			valor=theValue;
			ant=thePrev;
			post=thePost;
			modo=theMode;
			grupo=theGroup;
		}

	}	gradospto;
		//Los modos son "0 - invisible" "1 - visible" "2 - ángulo conjugado"

	typedef pair <const int, gradospto> gradosconundrum; //[Frame|[Punto,Angulo,Punto anterior,Punto siguiente,Modo]]
	typedef pair <const int, wxString> solonotas; //[Frame|String]
	//Iteradores
	typedef multimap <int, t_pfconundrum> :: iterator iterador;
	typedef multimap <int, t_pfconundrum> :: iterator iteralineas;
	typedef multimap <int, gradospto> :: iterator iteragrados;
	typedef multimap <int, wxString> :: iterator iteranotas;

	//Base de datos con todos los puntos almacenados:
	multimap <int, t_pfconundrum> 		points;
	multimap <int, t_pfconundrum> 		lines;
	multimap <int, gradospto> 			grados;
	multimap <int, wxString> 			notas;

	std::vector< int> gruposdeangulos;
	std::vector< int> gruposderectas;

protected:
	iterador 			pIter;
	iteralineas 		lineIter;
	iteragrados 		gradIter;
	iteranotas			notaIter;
	wxPoint				ref_0;

	//a prueba
	int ngrupos;

public:
	pointhandler();
	//convert
	wxPoint insizePoint(wxPoint);
	wxPoint resizePoint(wxPoint);
	//return offset
	int offsetvertical();
	int offsethorizontal();

	//basic
	int count(int);
	int savePoint(int,wxPoint,bool resize=true);
	int checkDirtyBit(int ,wxPoint );
	wxPoint loadPoint(iterador);
	int erasePoint(int,wxPoint,bool resize=true);
	int loadRange(int,wxPoint*,bool resize=true);
	int loadDirtyRange(int,wxPoint*, int* ,bool resize=true);
	float anglePoints(wxPoint,wxPoint,wxPoint);
	float distanceRef(float , wxPoint );
	float distanceMeasure(wxPoint  );
	void pointRef(wxPoint);
	void razondistancia(float);
	void setforwardIter(iterador,int);
	void setbackwardIter(iterador,int);
	void getReferences(int,int,int,int,int,int);
	int saveJSON(wxString);
	int loadJSON(wxString);
	int angulosJSON2graph(wxString ,std::vector<double>*,std::vector<double>*);
	int clearEverything();
	bool isClose(wxPoint, wxPoint);
	int collateral(int frame,wxPoint punto);


	//grupos
	int cuantosgruposdeangulos();
	int cuantosgruposdeangulos(std::vector< int>**);
	int cuantosgruposderectas();
	int cuantosgruposderectas(std::vector< int>**);
	int nuevogrupodeangulo( int);
	int nuevogrupoderectas( int);


	//rectas

	int countlines();
	int markLine(int ,wxPoint );
	int showLines(int* ,float* ,wxPoint*);

	//grados

	int threePoints(int frame,wxPoint punto,wxPoint prev,wxPoint post);
	int threePoints(int frame,wxPoint punto,wxPoint prev,wxPoint post, int grupo, int mode=1);
	int countgrad(int);
	int selectPoint(int,wxPoint);
	int gradsearch(int,wxPoint,bool resize=true);
	int showGrad(int ,float* ,wxPoint*);
	int showGrad(int ,float* ,wxPoint*, int*, int* modes=NULL);
	unsigned int generateHash(const char * string, size_t len);


	//notas

	void agregaNota(int frame, wxString nota);
	void borrarNota(int frame);
	int countnotas(int);
	int countnotas();
	int imprimeNota(int frame, wxString *nota);



} ;


#endif /* POINTHANDLER_H_ */
