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
 * pointhandler.cpp
 *
 *  Created on: 10/12/2012
 *      Author: Deriman Franco
 */

#include "pointhandler.h"

#include <wx/msgdlg.h>


pointhandler::pointhandler()
{
	ngrupos=24;
}

int pointhandler::offsetvertical()
{return h_offpos;}
int pointhandler::offsethorizontal()
{return w_offpos;}

int pointhandler::count(int frame)
{
	return points.count(frame);
}
wxPoint pointhandler::insizePoint(wxPoint punto)
{
	wxPoint aux;
	aux.x = wIn*(punto.x-w_offpos)+1;
	aux.y = hIn*(punto.y-h_offpos)+1;
	return aux;
}

wxPoint pointhandler::resizePoint(wxPoint punto)
{
	wxPoint aux;
	aux.x = (punto.x/wIn)+w_offpos;
	aux.y = (punto.y/hIn)+h_offpos;
	return aux;
}

int pointhandler::savePoint(int frame,wxPoint pos,bool resize)
{

	iterador primero= points.lower_bound(frame);
	iterador ultimo = points.upper_bound(frame);
	wxPoint aux;
	if(resize){
		pos = insizePoint(pos);
	}
	for (pIter = primero; pIter!=ultimo; pIter++ ){
		aux = loadPoint(pIter);
		if(isClose(pos,aux))
		{
			return -1 ;
		}
	}
	points.insert ( pointframeconundrum ( frame , t_pfconundrum(pos,1 )) );
	return 0 ;
}
int pointhandler::checkDirtyBit(int frame,wxPoint pos)
{
	iterador primero= points.lower_bound(frame);
	iterador ultimo = points.upper_bound(frame);
	wxPoint aux;
	for (pIter = primero; pIter!=ultimo; pIter++ ){
		aux = loadPoint(pIter);
		if(isClose(pos,aux))
		{
			pIter->second.second=0;
			return 1 ;
		}
	}
	return 0 ;
}
wxPoint pointhandler::loadPoint(iterador nodo)
{
	return (*nodo).second.first ;
}
int pointhandler::erasePoint(int frame,wxPoint punto,bool resize)
{
	iterador primero= points.lower_bound(frame);
	iterador ultimo = points.upper_bound(frame);
	wxPoint aux;
	if(resize){
		punto = insizePoint(punto);
	}
	for (pIter = primero; pIter!=ultimo; pIter++ ){

		aux = loadPoint(pIter);
		if(isClose(punto,aux))
		{
			collateral(frame,aux);
			points.erase( pIter );
			return 0 ;
		}
	}
	return -1 ;
}
int pointhandler::collateral(int framepos,wxPoint punto)
{
	//Collateral busca y elimina las ocurrencias de punto en el mismo frame para los grados y distancias
	//y devuelve cuantas ocurrencias encontró
	int total=grados.count(framepos);
	int n=0;
	if(total>0){
		iteragrados primero = grados.lower_bound(framepos);
		iteragrados ultimo = grados.upper_bound(framepos);
		for (gradIter = primero; gradIter!=ultimo; gradIter++ ){
			if(isClose((*gradIter).second.pto,punto)){
				grados.erase( gradIter ); n++;
				continue;
			}
			if(isClose((*gradIter).second.ant,punto)){
				grados.erase( gradIter ); n++;
				continue;
			}
			if(isClose((*gradIter).second.post,punto)){
				grados.erase( gradIter ); n++;
				continue;
			}
		}
	}
	total=lines.count(framepos);
	if(total>0){
		iteralineas primero = lines.lower_bound(framepos);
		iteralineas ultimo = lines.upper_bound(framepos);
		for (lineIter = primero; lineIter!=ultimo; lineIter++ ){
			if(isClose((*lineIter).second.first,punto)){
				lines.erase( lineIter ); n++;
				continue;
			}
		}
	}
	return n;
}
int pointhandler::loadRange(int framepos,wxPoint* vector,bool resize)
{
	int total=points.count(framepos);
	if(total>0){
		iterador primero = points.lower_bound(framepos);
		iterador ultimo = points.upper_bound(framepos);
		int i=0;
		for (pIter = primero; pIter!=ultimo; pIter++ ){
			if(resize){
				vector[i] = resizePoint(loadPoint(pIter));
			}else{
				vector[i] = loadPoint(pIter);
			}
			i++;
		}
		return total;
	}else return -1;

}
int pointhandler::loadDirtyRange(int framepos,wxPoint* vector, int* dirt, bool resize)
{
	int total=points.count(framepos);
	if(total>0){
		iterador primero = points.lower_bound(framepos);
		iterador ultimo = points.upper_bound(framepos);
		int i=0;
		for (pIter = primero; pIter!=ultimo; pIter++ ){
			if(resize){
				vector[i] = resizePoint(loadPoint(pIter));
			}else{
				vector[i] = loadPoint(pIter);
			}
			dirt[i] = pIter->second.second;
			i++;
		}
		return total;
	}else return -1;

}


float pointhandler::anglePoints(wxPoint pA,wxPoint pB,wxPoint pC)
{
	float A,B,C;
	A=sqrt(pow((double)(pA.x-pB.x),2)+pow((double)(pA.y-pB.y),2));
	B=sqrt(pow((double)(pA.x-pC.x),2)+pow((double)(pA.y-pC.y),2));
	C=sqrt(pow((double)(pC.x-pB.x),2)+pow((double)(pC.y-pB.y),2));
	return 180.0/3.14159*acos((B*B+A*A-C*C)/(2*A*B));
}
void pointhandler::pointRef(wxPoint point)
{
	ref_0 = point;
}
void pointhandler::razondistancia(float N)
{
	razon = N;
}
float pointhandler::distanceRef(float dist_med, wxPoint pos )
{
	float dist_ref=sqrt(pow((double)(ref_0.x-pos.x),2)+pow((double)(ref_0.y-pos.y),2));
	razon=dist_med/dist_ref;
	return razon;
}
float pointhandler::distanceMeasure(wxPoint pos )
{
	return razon*sqrt(pow((double)(ref_0.x-pos.x),2)+pow((double)(ref_0.y-pos.y),2));
}
void pointhandler::setforwardIter(multimap <int, t_pfconundrum> :: iterator A, int n)
{
	for(int i=0;i<n;i++) ++A;
}
void pointhandler::setbackwardIter(multimap <int, t_pfconundrum> :: iterator A,int n)
{
	for(int i=0;i<n;i++) --A;
}
void pointhandler::getReferences(int originalSize_h ,int originalSize_w,int newsize_h,int newsize_w,int h_off,int w_off)
{
	hIn = (float)originalSize_h/(float)newsize_h;
	wIn = (float)originalSize_w/(float)newsize_w;
	h_offpos=h_off;
	w_offpos=w_off;
}

int pointhandler::saveJSON(wxString direccion)
{
	/*The mode in which to open the file using standard C strings.
	 * Note that you should use "b" flag if you use binary files under
	 * Windows or the results might be unexpected due to automatic newline
	 * conversion done for the text files.*/
	/*The basic structure of the saved points is
	 * INT FRAME, INT POSITION X, INT POSITION Y*/
	/*The rate between the real point in the screen and the point
	 * in the image is given by the "razon" variable*/
	int numele=points.size();
	int frame,x,y;
	if (numele>0){
		wxFFile binario(direccion, wxT("w"));
		if(binario.IsOpened())
		{
			/*Creacion del objeto JSON*/
			wxJSONValue JSONobj;

			/*Creacion del objeto puntos dentro de JSON*/

			JSONobj["puntos"]["cantidad"]=numele;
			pIter= points.begin();
			for(int i=0;i<numele;i++){
				frame=(*pIter).first;
				x=(*pIter).second.first.x;
				y=(*pIter).second.first.y;
				JSONobj["puntos"]["puntos"][i]["frame"]=frame;
				JSONobj["puntos"]["puntos"][i]["x"]=x;
				JSONobj["puntos"]["puntos"][i]["y"]=y;
				pIter++;
			}

			/*Creacion del objeto angulos dentro de JSON*/

			numele=grados.size();
			JSONobj["angulos"]["cantidad"]=numele;
			JSONobj["angulos"]["ngrupos"]=ngrupos;

			for(unsigned int i=0;i<gruposdeangulos.size();i++){
				JSONobj["angulos"]["gruposdeangulos"][i]=gruposdeangulos[i];
			}
			gradIter= grados.begin();
			for(int i=0;i<numele;i++){
				frame=(*gradIter).first;
				JSONobj["angulos"]["coordenadas"][i]["frame"]=frame;
				JSONobj["angulos"]["coordenadas"][i]["punto"]["x"]=(*gradIter).second.pto.x;
				JSONobj["angulos"]["coordenadas"][i]["punto"]["y"]=(*gradIter).second.pto.y;
				JSONobj["angulos"]["coordenadas"][i]["anterior"]["x"]=(*gradIter).second.ant.x;
				JSONobj["angulos"]["coordenadas"][i]["anterior"]["y"]=(*gradIter).second.ant.y;
				JSONobj["angulos"]["coordenadas"][i]["posterior"]["x"]=(*gradIter).second.post.x;
				JSONobj["angulos"]["coordenadas"][i]["posterior"]["y"]=(*gradIter).second.post.y;
			//	JSONobj["angulos"]["coordenadas"][i]["valor"]=(*gradIter).second.valor;
				JSONobj["angulos"]["coordenadas"][i]["grupo"]=(*gradIter).second.grupo;
				JSONobj["angulos"]["coordenadas"][i]["modo"]=(*gradIter).second.modo;

				gradIter++;
			}
			/*Creacion del objeto segmentos dentro de JSON*/

			numele=lines.size();
			JSONobj["segmentos"]["cantidad"]=numele;
			JSONobj["segmentos"]["ngrupos"]=gruposderectas.size();

			for(unsigned int i=0;i<gruposderectas.size();i++){
				JSONobj["segmentos"]["gruposderectas"][i]=gruposderectas[i];
			}
			lineIter= lines.begin();
			for(int i=0;i<numele;i++){

				JSONobj["segmentos"]["coordenadas"][i]["frame"]=(*lineIter).first;
				JSONobj["segmentos"]["coordenadas"][i]["punto"]["x"]=(*lineIter).second.first.x;
				JSONobj["segmentos"]["coordenadas"][i]["punto"]["y"]=(*lineIter).second.first.y;

				lineIter++;
			}

			/*Creacion del objeto notas dentro de JSON*/

			numele=notas.size();
			JSONobj["notas"]["cantidad"]=numele;

			notaIter= notas.begin();
			for(int i=0;i<numele;i++){
				frame=(*notaIter).first;
				JSONobj["notas"]["entrada"][i]["frame"]=frame;
				JSONobj["notas"]["entrada"][i]["texto"]=(*notaIter).second;

				notaIter++;
			}
			/*Cierre del objeto*/

			wxString str;
			wxJSONWriter writer;
			writer.Write ( JSONobj, str);
			binario.Write(str); //sizeof(str)
			binario.Close();
			return points.size();;
		}
	}
	return -1;
}
int pointhandler::loadJSON(wxString direccion)
{
	int numele;
	wxString file;
	int frame,x,y,ngrupo,aux/*,aux2*/;
	wxFFile binario(direccion, wxT("r"));
	if(binario.IsOpened())
	{
		/*Preparación para la lectura del objeto JSON*/
		points.clear();
		binario.ReadAll(&file);
		binario.Close();

	    wxJSONReader reader;
	    wxJSONValue  JSONobj;
	    /*Se comprueba con el Parser si la estructura no tiene errores*/
	    int numErrors = reader.Parse( file, &JSONobj );
	    if ( numErrors > 0 )  {
	      // if there are errors in the JSON document, print the
	      // errors and return a non-ZERO value
	      const wxArrayString& errors = reader.GetErrors();
	      for ( int i = 0; i < numErrors; i++ )  {
	        printf(errors[i]);
	      }
	      return -2;
	    }

	    /*Lectura del objeto JSON*/

	   // wxString strazon=JSONobj["razon"].AsString();
	   // strazon.ToDouble(&razon);

	    /*Lectura de puntos en el objeto*/
	    numele=JSONobj["puntos"]["cantidad"].AsInt();
	    if(numele>0){
			for(int i=0;i<numele;i++){
				frame=JSONobj["puntos"]["puntos"][i]["frame"].AsInt();
				x=JSONobj["puntos"]["puntos"][i]["x"].AsInt();
				y=JSONobj["puntos"]["puntos"][i]["y"].AsInt();
				points.insert ( pointframeconundrum ( frame , t_pfconundrum(wxPoint(x,y), 0 )) );
			}
	    }

	    /*Lectura de angulos en el objeto*/

		numele=JSONobj["angulos"]["cantidad"].AsInt();

		for(int i=0;i<numele;i++){
			ngrupo=JSONobj["angulos"]["coordenadas"][i]["grupo"].AsInt();
			grados.insert ( gradosconundrum (JSONobj["angulos"]["coordenadas"][i]["frame"].AsInt(),
					gradospto(wxPoint(JSONobj["angulos"]["coordenadas"][i]["punto"]["x"].AsInt(),
							JSONobj["angulos"]["coordenadas"][i]["punto"]["y"].AsInt()),

						anglePoints(wxPoint(JSONobj["angulos"]["coordenadas"][i]["punto"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["punto"]["y"].AsInt()),
						wxPoint(JSONobj["angulos"]["coordenadas"][i]["anterior"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["anterior"]["y"].AsInt()),
						wxPoint(JSONobj["angulos"]["coordenadas"][i]["posterior"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["posterior"]["y"].AsInt())),

						wxPoint(JSONobj["angulos"]["coordenadas"][i]["anterior"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["anterior"]["y"].AsInt()),
						wxPoint(JSONobj["angulos"]["coordenadas"][i]["posterior"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["posterior"]["y"].AsInt()),
						1,ngrupo)
					));

			aux=0;
			for(unsigned int i=0;i<gruposdeangulos.size();i++) {if (gruposdeangulos[i]==ngrupo) aux=1;}
			if(aux!=1){	nuevogrupodeangulo(ngrupo);}
		}

		/*Lectura de segmentos en el objeto*/

		numele=JSONobj["segmentos"]["cantidad"].AsInt();

		if (numele>0){nuevogrupoderectas(0);}



		for(int i=0;i<numele;i++){
			lines.insert ( pointframeconundrum (JSONobj["segmentos"]["coordenadas"][i]["frame"].AsInt(),
					t_pfconundrum(wxPoint(JSONobj["segmentos"]["coordenadas"][i]["punto"]["x"].AsInt(),
										  JSONobj["segmentos"]["coordenadas"][i]["punto"]["y"].AsInt()),1)));

		}


		/*Lectura del objeto notas dentro de JSON*/

		numele=JSONobj["notas"]["cantidad"].AsInt();

		for(int i=0;i<numele;i++){
			agregaNota(JSONobj["notas"]["entrada"][i]["frame"].AsInt(),
					JSONobj["notas"]["entrada"][i]["texto"].AsString());
		}

		return JSONobj["puntos"]["cantidad"].AsInt();;
	}
	return -1;
}
int pointhandler::angulosJSON2graph(wxString direccion,std::vector<double>*frames,std::vector<double>*angulos)
{
	int numele;
	wxString file;
	wxFFile binario(direccion, wxT("r"));
	if(binario.IsOpened())
	{
		/*Preparación para la lectura del objeto JSON*/
		points.clear();
		binario.ReadAll(&file);
		binario.Close();

	    wxJSONReader reader;
	    wxJSONValue  JSONobj;
	    /*Se comprueba con el Parser si la estructura no tiene errores*/
	    int numErrors = reader.Parse( file, &JSONobj );
	    if ( numErrors > 0 )  {
	      // if there are errors in the JSON document, print the
	      // errors and return a non-ZERO value
	      const wxArrayString& errors = reader.GetErrors();
	      for ( int i = 0; i < numErrors; i++ )  {
	        printf(errors[i]);
	      }
	      return -2;
	    }
		    /*Lectura de angulos en el objeto*/

		numele=JSONobj["angulos"]["cantidad"].AsInt();

		for(int i=0;i<numele;i++){
			frames->push_back(JSONobj["angulos"]["coordenadas"][i]["frame"].AsInt());

			angulos->push_back(anglePoints(wxPoint(JSONobj["angulos"]["coordenadas"][i]["punto"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["punto"]["y"].AsInt()),
						wxPoint(JSONobj["angulos"]["coordenadas"][i]["anterior"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["anterior"]["y"].AsInt()),
						wxPoint(JSONobj["angulos"]["coordenadas"][i]["posterior"]["x"].AsInt(),
								JSONobj["angulos"]["coordenadas"][i]["posterior"]["y"].AsInt())));

		}
		return numele;

	}else{return -1;}
};

int pointhandler::clearEverything()
{
	points.clear();
	lines.clear();
	grados.clear();
	ngrupos=0;
	return points.size();
}

bool pointhandler::isClose(wxPoint target, wxPoint point)
{
	if(( (point.x-4 < target.x) && (point.x+4 > target.x) )
			&&( (point.y-4 < target.y) && (point.y+4 > target.y) )){
		return true;
	}
	return false;
}

/*
 *
 * Funciones de grupos
 *
 *
 * */


/*Cuantosgruposdeangulos/Cuantosgruposderectas copia el vector actual en vector
 * y devuelve su tamaño*/
int pointhandler::cuantosgruposdeangulos(std::vector< int> **vector)
{
	if(gruposdeangulos.size()>0){
		*vector = new std::vector< int>(gruposdeangulos);
		return (*vector)->size();
	}
	return 0;
}
int pointhandler::cuantosgruposdeangulos()
{
	return gruposdeangulos.size();
}
int pointhandler::cuantosgruposderectas(vector< int> **vector)
{
	if(gruposderectas.size()>0){
		*vector = new std::vector< int>(gruposderectas);
		return (*vector)->size();
	}
	return 0;
}
int pointhandler::cuantosgruposderectas()
{
	return gruposderectas.size();
}


/*Nuevogrupodeangulo añade un nuevo grupo al vector grupodeangulos
 * Ditto para nuevogrupoderectas*/
int pointhandler::nuevogrupodeangulo( int num)
{
	gruposdeangulos.push_back(num);
	return gruposdeangulos.size();
}
int pointhandler::nuevogrupoderectas( int num)
{
	gruposderectas.push_back(num);
	return gruposderectas.size();
}

/*
 *
 *
 * 	Ecuaciones de lineas
 *
 *
 * Revisión: no he aplicado resize ni insize
 * */

/* pointhandler::markLine
 * El punto medio de la recta se almacena junto al resto de lineas que se desean mostrar
 * Devuelve 1 si se añade a la lista
 * Devuelve -1 si se elimina de la lista
 * Devuelve 0 si ya se encontraba en la lista
*/
int pointhandler::markLine(int frame, wxPoint lepoint)
{
	wxPoint aux,point=insizePoint(lepoint);
	//Primero se comprueba si el punto ya existe
	iterador first= points.lower_bound(frame);
	iterador last = points.upper_bound(frame);
	for(pIter=first;pIter!=last;pIter++){
		aux = loadPoint(pIter);
		if(isClose(point,aux)){
			point=aux;
		}
	}
	//A continuación comprueba si el punto se ha almacenado como linea
	if (countlines()<=0){
		nuevogrupoderectas(0);
	}

	//Finalmente se añade a la lista de puntos
	lines.insert ( pointframeconundrum ( frame , t_pfconundrum ( point , 0)));
    return 1;
}

//Recopilación de las lineas (sus puntos medios) y sus correspondientes medidas
int pointhandler::countlines(){

	return lines.size();
}

//showLines devuelve las distancias, así como los 3 puntos. (Punto inicial, final y punto medio)

int pointhandler::showLines(int* frames,float* lenghts, wxPoint* vector)
{
	int total=lines.size();
	//int ocultas=0;
	if(total>0){
		int i=0;
		float zancada;
		for (std::multimap <int, t_pfconundrum>::iterator it=lines.begin(); it!=lines.end(); ++it){
			vector[i]=resizePoint((*it).second.first);
			frames[i]=(*it).first;
			if(i%2 && i>0){
				//zancada=abs(vector[i-1].x - vector[i].x);
				//zancada=fabs((float)razon*(float)sqrt(pow((float)(vector[i-1].x-vector[i].x),2)+pow((float)(vector[i-1].y-vector[i].y),2)));
				zancada=fabs(razon*sqrt(pow((vector[i-1].x-vector[i].x),2)+pow((vector[i-1].y-vector[i].y),2)));
			}else{
				zancada=0;
			}
			lenghts[i]=zancada;
			i++;
		}
		return total;
	}else return -1;
}


/*
 *
 *
 * 	Ecuaciones de grados
 *
 *
 *
 * Revisión: Cuidado con donde he aplicado resize e insize
 * */
int pointhandler::threePoints(int frame,wxPoint punto,wxPoint prev,wxPoint post, int grupo, int mode)
{
	wxPoint aux;
	wxPoint lepunto=insizePoint(punto), leprev=insizePoint(prev), lepost=insizePoint(post);
	int total=0;
	int control=0;
	int naux;
	iterador primero= points.lower_bound(frame);
	iterador ultimo = points.upper_bound(frame);
	for(pIter=primero;pIter!=ultimo;pIter++){
		aux = loadPoint(pIter);
		if(isClose(lepunto,aux)){control++; lepunto=aux;}
		if(isClose(leprev,aux)){control++; leprev=aux;}
		if(isClose(lepost,aux)){control++; lepost=aux;}
		if(control>1) return -1; //Dos puntos coinciden, son el mismo punto
		else {
			total+=control;
			control=0;
			if(total>3) return -1;
		}
	}
	if(total==3){
		if (gradsearch(frame,punto)<0){ //Se comprueba que no se encuentre ya en la lista

			naux=0;
			for(unsigned int i=0;i<gruposdeangulos.size();i++) {if (gruposdeangulos[i]==grupo) naux=1;}
			if(naux!=1){	nuevogrupodeangulo(grupo);}
			grados.insert ( gradosconundrum ( frame , gradospto(lepunto, anglePoints(lepunto,leprev,lepost),leprev,lepost,mode,grupo)));
			return 1; //Puntos detectados, valor de grado añadido
		}
	}
	return 0; //No todos los puntos forman parte de la imagen
}


int pointhandler::threePoints(int frame,wxPoint punto,wxPoint prev,wxPoint post)
{
	wxPoint aux;
	wxPoint lepunto=insizePoint(punto), leprev=insizePoint(prev), lepost=insizePoint(post);
	int total=0;
	int control=0;
	iterador primero= points.lower_bound(frame);
	iterador ultimo = points.upper_bound(frame);
	for(pIter=primero;pIter!=ultimo;pIter++){
		aux = loadPoint(pIter);
		if(isClose(lepunto,aux)){control++; lepunto=aux;}
		if(isClose(leprev,aux)){control++; leprev=aux;}
		if(isClose(lepost,aux)){control++; lepost=aux;}
		if(control>1) return -1; //Dos puntos coinciden, son el mismo punto
		else {
			total+=control;
			control=0;
			if(total>3) return -1;
		}
	}
	if(total==3){
		if (gradsearch(frame,punto)<0){ //Se comprueba que no se encuentre ya en la lista
			 int grupo = ngrupos;//generateHash(hashme,strlen(hashme));
			ngrupos=ngrupos+1;
			nuevogrupodeangulo(grupo);
			grados.insert ( gradosconundrum ( frame , gradospto(lepunto, anglePoints(lepunto,leprev,lepost),leprev,lepost,1,grupo)));
			return 1; //Puntos detectados, valor de grado añadido
		}
	}
	return 0; //No todos los puntos forman parte de la imagen
}
/*
 * selecPoint: Toma un punto y cambia el modo (0 invisible, 1 normal, 2 angulo opuesto)
 * */
int pointhandler::selectPoint(int frame,wxPoint punto)
{
	iteragrados primero= grados.lower_bound(frame);
	iteragrados ultimo = grados.upper_bound(frame);
	wxPoint aux;
	int modo=0;
	punto = insizePoint(punto);
	if(grados.count(frame)>0){
		for (gradIter=primero; gradIter!=ultimo; gradIter++ ){
			aux = (*gradIter).second.pto;
			modo =(*gradIter).second.modo;
			if(isClose(punto,aux)){
				switch(modo){
					case 0:
						(*gradIter).second.modo=1;
						break;
					case 1:
						(*gradIter).second.modo=2;
						break;
					case 2:
						(*gradIter).second.modo=0;
						break;
				}
				return (*primero).second.modo;
			}
		}
	}
	return -1;
}
/*
 * Gradsearch busca y cambia el modo segun su ultima posición.
 * */
int pointhandler::gradsearch(int frame,wxPoint punto,bool resize){
	//Se halla la ecuación de la recta a partir de dos puntos y se observa si coincide con el punto marcado
	iteragrados primero= grados.lower_bound(frame);
	iteragrados ultimo = grados.upper_bound(frame);
	wxPoint aux;

	if(resize){
		punto = insizePoint(punto);
	}
	if(grados.count(frame)>0){
			for (gradIter=primero; gradIter!=ultimo; gradIter++ ){
				aux = (*gradIter).second.pto;
				if(isClose(punto,aux)){

					return (*primero).second.modo;
				}
			}
	}
	return -1;
}

//Recopilación de los puntos y sus correspondientes medidas y vecinos
int pointhandler::countgrad(int framepos){

	return grados.count(framepos);
}
//Hay que tener en cuenta que el vector de showGrad es 3 veces mayor ya que tb contiene anterior y posterior
/*
 * showGrad busca los puntos en el frame y almacena qué puntos hay en vector y el ángulo en grad
 *
 * */
int pointhandler::showGrad(int framepos,float* grads,wxPoint* vector)
{
	int total=grados.count(framepos);
	int ocultas=0;
	if(total>0){
		iteragrados primero = grados.lower_bound(framepos);
		iteragrados ultimo = grados.upper_bound(framepos);
		int i=0;
		for (gradIter = primero; gradIter!=ultimo; gradIter++ ){
			if((*gradIter).second.modo>0){
				vector[i*3] = resizePoint((*gradIter).second.pto);
				vector[i*3+1] = resizePoint((*gradIter).second.ant);
				vector[i*3+2] = resizePoint((*gradIter).second.post);
				if((*gradIter).second.modo>1)
					grads[i] = 360-(*gradIter).second.valor;
				else
					grads[i] = (*gradIter).second.valor;
				i++;
			}else{
				ocultas++;
			}
		}
		//No hay forma de reubicar el espacio reservado sin utilizar en vector/grads.
		//Excepto si se usa la clase <vector> y se hace pop_back por cada oculto.
		return total-ocultas;
	}else return 0;
}
int pointhandler::showGrad(int framepos,float* grads,wxPoint* vector, int* grupos, int* modes)
{
	int total=grados.count(framepos);
	int ocultas=0;
	if(total>0){
		iteragrados primero = grados.lower_bound(framepos);
		iteragrados ultimo = grados.upper_bound(framepos);
		int i=0;
		for (gradIter = primero; gradIter!=ultimo; gradIter++ ){
			if((*gradIter).second.modo>0){
				vector[i*3] = resizePoint((*gradIter).second.pto);
				vector[i*3+1] = resizePoint((*gradIter).second.ant);
				vector[i*3+2] = resizePoint((*gradIter).second.post);
				if (modes != NULL) modes[i]=(*gradIter).second.modo;
				if((*gradIter).second.modo>1)
					grads[i] = 360-(*gradIter).second.valor;
				else
					grads[i] = (*gradIter).second.valor;
				grupos[i] = (*gradIter).second.grupo;
				i++;
			}else{
				ocultas++;
			}
		}
		//No hay forma de reubicar el espacio reservado sin utilizar en vector/grads.
		//Excepto si se usa la clase <vector> y se hace pop_back por cada oculto.
		return total-ocultas;
	}else return 0;
}


unsigned int pointhandler::generateHash(const char * string, size_t len) {

    unsigned int hash = 0;
    for(size_t i = 0; i < len; ++i)
        hash = 65599 * hash + string[i];
    return hash ^ (hash >> 16);
}



void pointhandler::agregaNota(int frame, wxString nota)
{
	notas.erase(frame);
	notas.insert(solonotas(frame,nota));
}
void pointhandler::borrarNota(int frame)
{
	notas.erase(frame);
}
int pointhandler::countnotas(int framepos)
{
	return notas.count(framepos);}
int pointhandler::countnotas()
{
	return notas.size();}
int pointhandler::imprimeNota(int frame, wxString *nota)
{

	notaIter = notas.find(frame);
	if(notaIter!=notas.end()){
		*nota=((*notaIter).second).mb_str();
		return 1;
	}

	return 0;
}
