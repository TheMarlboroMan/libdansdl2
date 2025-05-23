#pragma once

#include <ldt/segment_2d.h>
#include <ldt/box.h>
#include <ldt/point_2d.h>

namespace ldt {

/**
*Returns true if the given segment (ray) intersects the given box, both using
*cartesian coordinates! Seems this is called the Cyrus-Beck clip but math
*is far from my area of expertise.
*/
template<typename T, typename U>
bool ray_intersects_box_simple(
	const segment_2d<T>& _ray,
	const box<T, U>& _box
) {

	//Convert the vector to a point, to facilitate operations...
	point_2d direction={_ray.vector.x, _ray.vector.y};

	//this point represents the "end" of the box.
	auto endpoint=_box.origin;
	endpoint.x+=_box.w;
	endpoint.y+=_box.h;

	//Find out the two points in which the ray intersects the box's planes.
	//This are sort of percentages: they will be between 0 and 1 if they
	//lie within the ray's length and direction and anywhere else in any
	//other case.  These will be called "near" and "far" by convention,
	//we will sort the components later so that the names are true.
	//The math makes more sense if we decompose it into separate axis:
	//Considering 0 the beginning of the ray and 1 the end, a point t that
	//crosses one axis of the box can be calculated by box.edge - ray.origin
	//and then divided by the ray length so that we get a value between 0
	//and 1.
	//NOTE: using floats here we can get inf and -inf when direction is zero,
	//thus avoiding a crash.
	point_2d<T> near=( _box.origin - _ray.point) / direction,
	            far=(endpoint - _ray.point ) / direction;

	//These will produce stuff like:
	//ldt::point[2.09091,2.92857] ldt::point[-0.939394,1.5]
	//which means that at 2.09 times the ray we will intersect the x axis,
	//same for 2.92 and 1.5 at the y axis. The -0.93 means that we would
	//intersect the x axis "behind" the ray too, which is useless for us
	//now... We can safely discard any value lesser than 0 as being "behind"
	//and any value larger than 1 as being "beyond" the ray length.
	//We can use the minimum acceptable values on each axis to find when
	//we "first" intersect on each axis... say...
	//ldt::point[0.301587,0.470588] ldt::point[1.09524,0.797386]
	//We intersect x at 0.3 and the nearest y is 0.47. Running this and
	//checking values mean that we intersect with the Y axis (the largest
	//of the smallest valid values in each axis).
	//Another example: ldt::point[0.21978,0.167702] ldt::point[0.769231,0.78882]
	//X intersects at 0.2 and 0.7 and y at 0.1 and 0.7, thus the final pair
	//of intersections is x=0.2 and y=0.1
	//Now, by testing and observation of values we have asserted that the 
	//minimum valid value in one axis must be smaller than the largest one
	//in the other, so there goes another early exit... Let us sort the
	//components of the points to check that (this is the 
	if(near.x > far.x) {

		std::swap(near.x, far.x);
	}

	if(near.y > far.y) {

		std::swap(near.y, far.y);
	}

	if(near.x > far.y || near.y > far.x) {

		return false;
	}

	//Ok, we grab the furthest intersection.A negative value means we are
	//pointing away, so another early exit.
	T intersection_far=std::min(far.x, far.y);
	if(intersection_far < 0.) {

		return false;
	}

	//This is the nearest intersection... It must be between 0 and 1 to be
	//an intersection and the math just works out.
	T intersection_near=std::max(near.x, near.y);
	if(intersection_near < 0. || intersection_near > 1.) {

		return false;
	}

	return true;
}

template<typename T> struct
ray_box_intersection {
	bool        has_intersection{false};
	T           intersection_t; //intersection "moment"
	point_2d<T> point;
	vector_2d<T> normal;
};

/**
 *Returns a complex structure with all relevant intersection information.
 */
template<typename T, typename U>
ray_box_intersection<T> ray_intersects_box(
	const segment_2d<T>& _ray,
	const box<T, U>& _box
) {

	point_2d direction={_ray.vector.x, _ray.vector.y};

	auto endpoint=_box.origin;
	endpoint.x+=_box.w;
	endpoint.y+=_box.h;

	point_2d<T> near=( _box.origin - _ray.point) / direction,
	            far=(endpoint - _ray.point ) / direction;

	if(near.x > far.x) {

		std::swap(near.x, far.x);
	}

	if(near.y > far.y) {

		std::swap(near.y, far.y);
	}

	if(near.x > far.y || near.y > far.x) {

		return {false, 0., {0., 0.}, {0., 0.}};
	}

	//Ok, we grab the furthest intersection.A negative value means we are
	//pointing away, so another early exit.
	T intersection_far=std::min(far.x, far.y);
	if(intersection_far < 0.) {

		return {false, 0., {0., 0.}, {0., 0.}};
	}

	T intersection_near=std::max(near.x, near.y);
	if(intersection_near < 0. || intersection_near > 1.) {

		return {false, 0., {0., 0.}, {0., 0.}};
	}

	//We can plug the line equation here to get the point at where the
	//intersection takes place...
	point_2d<T> intersection_point=_ray.point + (direction * intersection_near);

	//The normals... the normals... We can combine vector direction and
	//some of the properties to obtain a normal.
	vector_2d<T> normal;
	if(near.x > near.y) {

		normal=direction.x < 0 ? vector_2d<T>{1., 0.} : vector_2d<T>{-1, 0.};
	}
	else {

		normal=direction.y < 0 ? vector_2d<T>{0., 1.} : vector_2d<T>{0., -1};
	}

	return {true, intersection_near, intersection_point, normal};
}

/**
 * Same as the one above but writes results in the last argument.
 */
template<typename T, typename U>
bool ray_intersects_box(
	const segment_2d<T>& _ray,
	const box<T, U>& _box,
	ray_box_intersection<T>& _out
) {
	_out={false, 0., {0., 0.}, {0., 0.}};

	point_2d direction={_ray.vector.x, _ray.vector.y};

	auto endpoint=_box.origin;
	endpoint.x+=_box.w;
	endpoint.y+=_box.h;

	point_2d<T> near=( _box.origin - _ray.point) / direction,
	            far=(endpoint - _ray.point ) / direction;

	if(near.x > far.x) {

		std::swap(near.x, far.x);
	}

	if(near.y > far.y) {

		std::swap(near.y, far.y);
	}

	if(near.x > far.y || near.y > far.x) {

		return false;
	}

	T intersection_far=std::min(far.x, far.y);
	if(intersection_far < 0.) {

		return false;
	}

	T intersection_near=std::max(near.x, near.y);
	if(intersection_near < 0. || intersection_near > 1.) {

		return false;
	}

	point_2d<T> intersection_point=_ray.point + (direction * intersection_near);

	vector_2d<T> normal;
	if(near.x > near.y) {

		normal=direction.x < 0 ? vector_2d<T>{1., 0.} : vector_2d<T>{-1, 0.};
	}
	else {

		normal=direction.y < 0 ? vector_2d<T>{0., 1.} : vector_2d<T>{0., -1};
	}

	_out={true, intersection_near, intersection_point, normal};
	return true;
}

}//end of namespace!
