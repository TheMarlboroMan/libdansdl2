#include <iostream>
#include "tools/polygon_2d/polygon_2d.h"

int main(int argc, char ** argv) {

	typedef ldt::polygon_2d<double> tpoly;

//	tpoly a{ { {8., 4.}, {8., 9.}, {14., 9.}, {14., 4.} } };
//	tpoly b{ { {4., 2.}, {4., 7.}, {12., 7.}, {12., 2.} } };


	//TODO: This is already fucked up...
	tpoly a{ { {0., 0.}, {0., 10.}, {10., 10.}, {10., 0.} } };
	tpoly b{ { {8., 0.}, {8., 10.}, {18., 10.}, {18., 0.} } };

	auto col_res=ldt::SAT_collision_check_mtv(b, a);

	std::cout<<col_res.collision<<" ["<<col_res.mtv.x<<","<<col_res.mtv.y<<"]"<<std::endl;

}
