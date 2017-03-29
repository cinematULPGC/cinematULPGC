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
 * opticalflow.h
 *
 *  Created on: 13/03/2013
 *      Author: Deriman Franco
 */

//Pyramid Lucas-Kanade optical flow code


// Pyramid L-K optical flow example
//
//
/*

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>*/

#include <cstdio>
 //  using namespace std;
#include "opticalflow.h"

int classFLOW::opticalFLOW(cv::Mat imgAin, cv::Mat imgBin, cv::Mat imgC)
{
	// Initialize, load two images from the file system, and
	// allocate the images and other structures we will need for
	// results.
	// The first thing we need to do is get the features
	// we want to track.
	//
	cv::Mat imgA = imgAin.clone();
	cv::Mat imgB = imgBin.clone();
	cv::cvtColor(imgA, imgA, CV_BGR2GRAY);
	cv::cvtColor(imgB, imgB, CV_BGR2GRAY);


	int corner_count = MAX_CORNERS;
	std::vector<cv::Point2f> cornersA;
	//if (cornersA.empty())
	{
		/*Run Shi and Tomasi to track good features in the image
		 * that is, elements with high eigenvalues
		 * goodFeaturesToTrack(
		 * InputArray image, OutputArray corners,
		 * int maxCorners,
		 * double qualityLevel,
		 * double minDistance*/
		cv::goodFeaturesToTrack(
			imgA,
			cornersA,
			corner_count,
			0.01,
			10.0
		);
	}
	/*
	Size winSize = Size( 5, 5 );
	Size zeroZone = Size( -1, -1 );

	TermCriteria criteria = TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
	cv::cornerSubPix(
		imgA,
		cornersA,
		winSize,
		zeroZone,
		criteria
	);*/
	// Call the Lucas Kanade algorithm
	//
	std::vector<cv::Point2f> cornersB;
	std::vector<uchar>  features_found;
	cv::Mat  feature_errors;
	/*prevImg – first 8-bit input image or pyramid constructed by buildOpticalFlowPyramid().
	nextImg – second input image or pyramid of the same size and the same type as prevImg.
	prevPts – vector of 2D points for which the flow needs to be found;
	nextPts – output vector of 2D points (with single-precision floating-point coordinates)
	containing the calculated new positions of input features in the second image;
	status – output status vector (of unsigned chars); 	each element of the vector is set to
	1 if the flow for the corresponding features has been found, otherwise, it is set to 0.
	err – output vector of errors; each element of the vector is set to an error
	for the corresponding feature, type of the error measure can be set in flags parameter;
	if the flow wasn’t found then the error is not defined (use the status parameter to find such cases).*/
	//Mat auxiliar = cornersA.getMat();
	//if(auxiliar.checkVector(2, CV_32F, true) >= 0)	throw "what";

	cv::calcOpticalFlowPyrLK(
		imgA,//prevImg
		imgB,//nextImg
		cornersA,//prevPts
		cornersB,//nextPts
		features_found,//status
		feature_errors,//err
		Size(11,11),//window size
		5,//5 levels
		TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.31)
	);

	// Now make some image of what we are looking at:
	//int mysize = features_found.size();
	printf("Corner counts: %i - ",corner_count);
	printf("features size: %i\n",features_found.size());
	for( unsigned i=0; i<features_found.size(); i++ ) {

		Point p0 = Point(
			cvRound( cornersA[i].x ),
			cvRound( cornersA[i].y )
		);
		Point p1 = cvPoint(
			cvRound( cornersB[i].x ),
			cvRound( cornersB[i].y )
		);

		if (p0 == p1){
			line( imgC, p0, p1, CV_RGB(0,255,0),2 );
		}else{
			line( imgC, p0, p1, CV_RGB(255,0,0),2 );
		}
	}

	return 0;
}



/*
 * funcion PRINCIPAL entrada IMAGEN_1, IMAGEN_2, MARCADORES devuelve NUEVO MARCADORES
 *
 * GOOD FEATURES TO TRACK IMAGEN_1 devuelve LISTA DE PUNTOS
 *
 * FLUJO OPTICO IMAGEN_1, IMAGEN_2 devuelve NUEVA LISTA DE PUNTOS
 *
 * COMPARA MARCADORES Y LISTA DE PUNTOS
 * 		cierto AÑADIR NUEVO MARCADORES DESDE NUEVA LISTA DE PUNTOS
 * 		falso SIGUIENTE PUNTO / SALIR
 *
 * DEVOLVER NUEVO MARCADORES (añadir marcadores una vez fuera de la funcion)
 * */

std::vector<cv::Point2f> classFLOW::PRINCIPAL(cv::Mat imgAin, cv::Mat imgBin, std::vector<cv::Point2f> puntos,
		double qualityLevel, double minDistance, int windowsize, int levels)
{
	// Initialize, load two images from the file system, and
	// allocate the images and other structures we will need for
	// results.
	// The first thing we need to do is get the features
	// we want to track.
	//
	int AREA=5;
	std::vector<cv::Point2f> nuevopuntos;// = new cv::Point2f[puntos.size()];

	//clausula de escape si no hay puntos a trazar
	if (puntos.size()==0) return nuevopuntos;

	cv::Mat imgA = imgAin.clone();
	cv::Mat imgB = imgBin.clone();
	cv::cvtColor(imgA, imgA, CV_BGR2GRAY);
	cv::cvtColor(imgB, imgB, CV_BGR2GRAY);

	int corner_count = MAX_CORNERS;
	std::vector<cv::Point2f> cornersA;
	if (cornersA.empty())
	{
		/*Run Shi and Tomasi to track good features in the image
		 * that is, elements with high eigenvalues
		 * goodFeaturesToTrack(
		 * InputArray image, OutputArray corners,
		 * int maxCorners,
		 * double qualityLevel, 0.01
		 * double minDistance 10.0*/
		cv::goodFeaturesToTrack(
			imgA,
			cornersA,
			corner_count,
			qualityLevel,//0.01,
			minDistance//10.0
		);

	}


	// Call the Lucas Kanade algorithm
	//
	std::vector<cv::Point2f> cornersB;
	std::vector<uchar>  features_found;
	cv::Mat  feature_errors;

	cv::calcOpticalFlowPyrLK(
		imgA,//prevImg
		imgB,//nextImg
		cornersA,//prevPts
		cornersB,//nextPts
		features_found,//status
		feature_errors,//err
		Size(windowsize,windowsize),//window size 11
		levels,//5 levels
		TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.31)
	);
	AREA+=12;
	float tmp,distancia;
	for( unsigned k=0; k<puntos.size(); k++ ) {
		distancia=100000000;
		for( unsigned i=0; i<features_found.size(); i++ ) {

			Point p0 = Point(
				cvRound( cornersA[i].x ),
				cvRound( cornersA[i].y )
			);
			Point p1 = cvPoint(
				cvRound( cornersB[i].x ),
				cvRound( cornersB[i].y )
			);
				//distancia manhattan
				tmp=abs(p0.x-puntos[k].x)+abs(p0.y-puntos[k].y);
				if (tmp<=distancia){
					if(distancia!=100000000) nuevopuntos.pop_back();
					nuevopuntos.push_back(p1);
					distancia=tmp;
				}
		}
	}

	printf("Optical Flow ends\n");
	fflush(stdout);
	return nuevopuntos;
}
