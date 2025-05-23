#include "../../include/ldv/camera.h"
#include "../../include/ldv/draw.h"
#include "../../include/ldt/box.h"
#include "../../include/ldt/aabb_ray_collision.h"
#include "../../include/ldt/segment_2d.h"


#include <vector>

namespace ldtest {

class draw {

	private:

	ldv::camera cam;
	std::vector<ldt::box<double, int>> boxes;
	ldt::point_2d<double> origin;

	public:

	draw():
		cam{{0,0,500,500},{0,0}, ldv::camera::tsystem::cartesian},
		origin{0,0}
	{

		boxes.push_back({{100, 100}, 60, 30});
		boxes.push_back({{120, 200}, 40, 100});
		boxes.push_back({{250, 30}, 50, 150});
		boxes.push_back({{400, 200}, 150, 40});
	}

	void run(
		ldv::screen& _screen, 
		ldi::sdl_input& _input
	) {

		while(true) {

			_input.loop();

			if(_input.is_exit_signal() || _input.is_key_down(SDL_SCANCODE_ESCAPE)) {

				break;
			}

			const auto& mouse_screen=_input.get_mouse().get_position();
			point_2d<double> mouse_pos(mouse_screen.x, _screen.get_h()-mouse_screen.y);

			//if click, set point...
			if(_input.is_mouse_button_down(1)) {

				origin=mouse_pos;
			}

			ldv::draw dr{_screen, &cam};
			dr.clear(dr.black);

			//draw a few boxes and see if there are any intersections...

			ldt::segment_2d<double> ray{origin, mouse_pos};
			ldt::ray_box_intersection<double> intersection;
			for(const auto& box : boxes) {

				if(!ldt::ray_intersects_box(ray, box, intersection)) {

					dr.box(box, dr.gray);
				}
				else {

					dr.box(box, dr.orange);

					//Collision point...
					dr.box(
						intersection.point.x-1,
						intersection.point.y-1,
						3, 3, dr.white
					);

					//And normal!
					dr.ray(
						{intersection.point, intersection.normal*10},
						dr.white
					);
				}
			}

			//dr.line(origin, mouse_pos, dr.red);
			dr.ray(ray, dr.red);

			dr.update();
		}
	}

};
}
