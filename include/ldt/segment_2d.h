#pragma once

#include "point_2d.h"
#include "vector_2d.h"
#include <lm/log.h>
#include <ostream>

namespace ldt {

//!A segment is a couple of points joined by a bearing vector... Which is
//

template<typename T>
struct segment_2d {

	using tpoint=point_2d<T>; //!<Defines the point type.
	using tvector=vector_2d<T>;
	tpoint              point;  //!< Starting vertex of the segment.
	tvector             vector; //!< vector of the segment. Somewhat redundant given that we have vertexes... or is it the other way around?.

	//!Creates a segment from point to v2.
					segment_2d<T>(tpoint _point, tpoint _point_2)
		:point(_point), vector(vector_from_points(_point, _point_2)) {

	}

	//!Creates a segment from point and vector
					segment_2d<T>(
			tpoint _point, 
			tvector _vector
	) :point{_point}, vector{_vector} {

	}

					segment_2d<T>()
		:point(tpoint(0,0)), vector(tvector(0,0)) {

	}

	//!Copy constructor.
					segment_2d<T>(const segment_2d<T>& o)
	:point(o.point), vector(o.vector) {

	}

	//!Moves the segment by p units.
	segment_2d<T>&		move(tpoint p) {
		point+=p;
		return *this;
	}

	//!Extends the vector by the given factor and returns a new one.
	segment_2d<T>       operator*(T _factor) {

		return segment_2d<T> {point, vector*_factor};
	}

	//!Extends the vector by the given factor.
	segment_2d<T>&      operator*=(T _factor) {

		vector*=_factor;
		return *this;
	}
	

	//!Returns the point that marks the end of the segment
	tpoint				end() const {

		return {point.x+vector.x, point.y+vector.y};
	}

	//!Checks for strict equality.
	bool				operator==(const segment_2d<T> s) const {

		return point==s.point 
			&& vector==s.vector;
	}
};

//Calculates the middle point of a segment...
template<typename T>
point_2d<T> segment_middle_point(const segment_2d<T>& _segment) {

	auto v2=_segment.end();

	return {
		(_segment.point.x+v2.x) / 2.,
		(_segment.point.y+v2.y) / 2.
	};
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

	auto av2=a.end();
	auto bv2=b.end();

	point_2d<T> r=av2 - a.point;
	point_2d<T> s=bv2 - b.point;
	point_2d<T> bminusa=b.point-a.point;

	T uNumerator=scalar_product(bminusa, r);
	T denominator=scalar_product(r, s);

	//Part of the same line...
	if(uNumerator==0.0 && denominator==0.0) {

		//There is a coincidence in points...
		if(a.point==b.point || a.point==bv2 || av2==b.point || av2==bv2) {

			return true;
		}

		//Crossing check: are all points in the same bearing of the same sign?
		return !are_equal(
				(b.point.x - a.point.x < 0),
				(b.point.x - av2.x < 0),
				(bv2.x - a.point.x < 0),
				(bv2.x - av2.x < 0)) ||
			!are_equal(
				(b.point.y - a.point.y < 0),
				(b.point.y - av2.y < 0),
				(bv2.y - a.point.y < 0),
				(bv2.y - av2.y < 0));
	}

	//Paralells.
	if(denominator == 0) {

		return false;
	}

	point_2d<T> npt=b.point-a.point;
	T u=uNumerator / denominator;
	T t=scalar_product(npt, s) / denominator;

	return t >= 0 && t <= 1 && u >= 0 && u <= 1;
}

template<typename T>
std::ostream& operator<<(
	std::ostream& _stream,
	const segment_2d<T>& _segment
) {

	_stream<<_segment.point<<" => "<<_segment.vector;
	return _stream;
}

template<typename T>
lm::log& operator<<(
	lm::log& _log,
	const segment_2d<T>& _segment
) {

	_log<<_segment.point<<" => "<<_segment.vector;
	return _log;
}


}//End of namespace.
