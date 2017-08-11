#ifndef TOOLS_DLIBDAN_H
#define TOOLS_DLIBDAN_H

#include <cstring> //Para memset
#include <fstream>
#include <iostream>
#include <string>
#include <locale>
#include <cmath>
#include <cctype>
#include <vector>
#include <cstdio>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159
#endif

namespace ldt
{

//!Converts degrees to radians.
template<typename T>
float deg_to_rad(T val) {return (val * M_PI) / 180.0;}

//!Converts radians to degrees.
template<typename T>
float rad_to_deg(T val) {return ( val * 180.0) / M_PI;}

//!Determines if segments overlap.

//!Only valid for points in the same plane. The comparison is done by
//!negating "segments don't overlap".
template<typename T>
bool segments_overlap(T aini, T afin, T bini, T bfin, bool unit_is_collision=false)
{
        if(unit_is_collision)
        {
                return !( (bfin < aini) || (bini > afin) );
        }
        else
        {
                return !( (bfin <= aini) || (bini >= afin) );
        }
}

float degree_360(float f);
float angle_from_points(float, float, float, float);
float percent(float, float);

template <typename T, typename U> bool rects_overlap(T, T, U, U, T, T, U, U, bool=false);
template <typename T, typename U> bool rects_overlap(T, T, U, U, T, T, U, U, T&, T&, U&, U&, bool=false);
template <typename T, typename U> void position_dimension_segments(T, T, T, T, T&, U&);
template <typename T, typename U> void position_dimension_segments_partial_solution(T, T, T, T, T&, U&);
template <typename T, typename U> void position_dimension_segments_complete_solution(T, T, T, T, T&, U&);
template <typename T, typename U> bool box_in_box(T, T, U, U, T, T, U, U);
template <typename T, typename U> bool point_in_box(T, T, U, U, T, T);
}

#include "tools.templates.cpp"

#endif
