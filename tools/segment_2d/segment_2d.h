#ifndef LIBDANSDL2_SEGMENT_2D_H
#define LIBDANSDL2_SEGMENT_2D_H

#include "../point_2d/point_2d.h"
#include "../vector_2d/vector_2d.h"

namespace ldt {

//!A segment is a couple of points joined by a bearing vector... 

//TODO: Pretty much sure we can use a point and a single vector to represent this.
template<typename T>
struct segment_2d
{
	

	//!Defines the point type.
	typedef	point_2d<T>		tpoint;
	tpoint					v1, 		//!< Starting vertex of the segment.
							v2;			//!< End vertex of the segment.
	vector_2d<T>			direction;	//!< Direction of the segment. Somewhat redundant given that we have vertexes... or is it the other way around?.

	//!Creates a segment from v1 to v2.
					segment_2d<T>(tpoint pv1, tpoint pv2)
		:v1(pv1), v2(pv2), direction(vector_from_points(v1, v2))
	{}

					segment_2d<T>()
		:v1(tpoint(0,0)), v2(tpoint(0,0)), direction(vector_2d<T>(0,0))
	{}

	//!Copy constructor.
					segment_2d<T>(const segment_2d<T>& o)
	:v1(o.v1), v2(o.v2), direction(o.direction)
	{}

	//!Moves the segment by p units.
	void				move(tpoint p)
	{
		v1+=p;
		v2+=p;
	}

	//!Checks for strict equality.
	bool				operator==(const segment_2d<T> s) const
	{
		return v1==s.v1 && v2==s.v2 && direction==s.direction;
	}
};

//Calculates the middle point of a segment...
template<typename T>
point_2d<T> segment_middle_point(const segment_2d<T>& s)
{
	//TODO: If segments are changed, must be changed a bit...

	return {	(s.v1.x+s.v2.x) / 2., 
			(s.v1.y+s.v2.y) / 2.};
}

//!Checks if two segments are intersecting.

//!Non intersecting segments must be non-paralell and prolong to the point where they cross.
//!Adapted from  de https://github.com/siebenschlaefer/line-segments-intersect/blob/included_line_segments/js/line-segments-intersect.js

template<typename T>
bool segments_intersect(const segment_2d<T>& a, const segment_2d<T>& b) {

	auto scalar_product=[](const point_2d<T>& pa, const point_2d<T>& pb) {

		return (pa.x * pb.y) - (pa.y * pb.x);
	};

	auto are_equal=[](bool pa, bool pb, bool pc, bool pd) {

		return pa==pb && pa==pc && pa==pd;
	};

	point_2d<T> r=a.v2 - a.v1;
	point_2d<T> s=b.v2 - b.v1;
	point_2d<T> bminusa=b.v1-a.v1;

	T uNumerator=scalar_product(bminusa, r);
	T denominator=scalar_product(r, s);

	//Part of the same line...
	if(uNumerator==0.0 && denominator==0.0) {

		//There is a coincidence in points...
		if(a.v1==b.v1 || a.v1==b.v2 || a.v2==b.v1 || a.v2==b.v2) {

			return true;
		}

		//Crossing check: are all points in the same bearing of the same sign?
		return !are_equal(
				(b.v1.x - a.v1.x < 0),
				(b.v1.x - a.v2.x < 0),
				(b.v2.x - a.v1.x < 0),
				(b.v2.x - a.v2.x < 0)) ||
			!are_equal(
				(b.v1.y - a.v1.y < 0),
				(b.v1.y - a.v2.y < 0),
				(b.v2.y - a.v1.y < 0),
				(b.v2.y - a.v2.y < 0));

	}

	//Paralells.
	if(denominator == 0) {

		return false;
	}

	point_2d<T> npt=b.v1-a.v1;
	T u=uNumerator / denominator;
	T t=scalar_product(npt, s) / denominator;

	return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}


}

#endif
