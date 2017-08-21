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
T deg_to_rad(T val) {return (val * (T)M_PI) / (T)180;}

//!Converts radians to degrees.
template<typename T>
T rad_to_deg(T val) {return ( val * (T)180) / (T)M_PI;}

//!Determines if segments overlap.

//!Only valid for points in the same plane. The comparison is done by
//!negating "segments don't overlap". It works with the assumption that the
//!"begin" values are always smaller than the "end" ones.

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

template<typename T>
T percent(T p_part, T p_total)
{
	T zero{};
	if(p_total==zero) return zero;
	else return (p_part * (T)100) / p_total;
}

template <typename T, typename U> bool rects_overlap(T, T, U, U, T, T, U, U, bool=false);
template <typename T, typename U> bool rects_overlap(T, T, U, U, T, T, U, U, T&, T&, U&, U&, bool=false);
template <typename T, typename U> bool box_in_box(T, T, U, U, T, T, U, U);
template <typename T, typename U> bool point_in_box(T, T, U, U, T, T);
}

#include "tools.templates.cpp"

#endif
