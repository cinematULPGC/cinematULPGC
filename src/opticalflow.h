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
*/
#ifndef OPTICALFLOW_H_
#define OPTICALFLOW_H_

#include "opencv2/opencv.hpp"

using namespace cv;

const int MAX_CORNERS = 500;

class classFLOW
{
	public:

	int opticalFLOW(cv::Mat imgA, cv::Mat imgB, cv::Mat imgC);
	std::vector<cv::Point2f> PRINCIPAL(cv::Mat imgAin, cv::Mat imgBin, std::vector<cv::Point2f> puntos,
			double qualityLevel=0.01, double minDistance=10.0, int windowsize=11, int levels=5);
};

#endif
