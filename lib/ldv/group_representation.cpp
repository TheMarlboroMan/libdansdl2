#include <ldv/group_representation.h>

using namespace ldv;

group_representation::group_representation(point p)
	:representation(),
	position{p}
{
	update_base_view_position();
}

//!Empties the group.

//!All representations will be destroyed.

void group_representation::clear() {

	data.clear();
}

//!Draws the group to the screen using a camera.

//!Parameters are the same as in a regular representation object.

bool group_representation::draw(
	screen& p_screen, 
	const camera& pcamera, 
	bool skip_take
) {

	//Using the draw info allows us to work with cartesian coordinates.
	const auto& cf=pcamera.get_draw_info();
	const auto& vp=get_view_position();

	if(
		is_visible() && 
		(
			skip_take 
			|| ldt::rects_overlap<int, int>(cf.rel_x, cf.rel_y, cf.view_w, cf.view_h, vp.origin.x, vp.origin.y, vp.w, vp.h, true)
		)
	) {

		return draw_internal(p_screen, &pcamera);
	}

	return false;
}

//!Draws the group to the screen with no camera.

//!Parameters are the same as in a regular representation object.

bool group_representation::draw(
	screen& p_screen, 
	bool skip_take
) {

	auto in_screen=[this](screen &screen) {

		const auto& v_position=get_view_position();

		int ex=v_position.origin.x+v_position.w,
			ey=v_position.origin.y+v_position.h;

		return 	ex >= 0
			&& v_position.origin.x <= (int) screen.get_w()
			&& ey >= 0
			&& v_position.origin.y <= (int) screen.get_h();
	};

	if(is_visible() && (skip_take || in_screen(p_screen))) {

		return draw_internal(p_screen, nullptr);
	}

	return false;
}

//!Internal draw function.

//!The position of each representation will be added to the group position, so
//!top-left of the group is 0.0. Group alpha is applied to each item whenever
//!it is not opaque. In other words, it is not accumulative nor it does blend.

bool group_representation::draw_internal(
	screen& p_screen, 
	camera const * pcamera
) {

	//When assigning, matrixes are reset. If assignation occurs during
	//this iteration we can have terrible results because translations not
	//accounted for.
	if(pcamera!=nullptr) p_screen.set_camera(*pcamera);

	bool result=false;

	for(auto &r : data) {

		if(!r->is_visible()) {

			continue;
		}

		glMatrixMode(GL_MODELVIEW);

		//Traslación propia del grupo, rotaciones...
		auto tr=get_transformation_rotation();
		auto iv=pcamera != nullptr ? pcamera->get_draw_info() : p_screen.get_draw_info();

		glTranslatef(position.x*iv.zoom, position.y*iv.zoom, 0.f);

		if(pcamera!=nullptr) {

			//This is pure empiric knowledge. Try and error.
			if(tr.angle != 0.f) {

				float 	tx=iv.pos_x+((tr.center.x-iv.rel_x)*iv.zoom),
					ty=iv.pos_y+((tr.center.y-iv.rel_y)*iv.zoom);

				glTranslatef(tx, ty, 0.f);
				glRotatef(tr.angle, 0.f, 0.f, 1.f);
				glTranslatef(-tx, -ty, 0.f);
			}
		}
		else {

			if(tr.angle != 0.f) {

				glTranslatef(tr.center.x, tr.center.y, 0.f);
				glRotatef(tr.angle, 0.f, 0.f, 1.f);
				glTranslatef(-tr.center.x, -tr.center.y, 0.f);
			}
		}

		int alpha_original=r->get_alpha();
		auto blend_original=r->get_blend();

		int calculado=get_alpha() ? (alpha_original * get_alpha()) / alpha_max : alpha_min;
		r->set_blend(representation::blends::alpha);
		r->set_alpha(calculado);

		//Camera and screen checks are skipped.
		if(pcamera!=nullptr) {

			result|=r->draw(p_screen, *pcamera, true);
		}
		else {

			result|=r->draw(p_screen, true);
		}

		r->set_blend(blend_original);
		r->set_alpha(alpha_original);
	}

	return result;
}

//!This function is unreachable.

void group_representation::do_draw() {

}

//!Inserts a representation in the group.

//!The order of insertion determines the subsequent drawing order, making
//!groups a very rigid structure.

void group_representation::insert(representation * p_rep) {

	data.push_back(std::unique_ptr<representation>(p_rep));
	update_base_view_position();
}

//!Updates the group position.

void  group_representation::go_to(point p) {

	position={p.x, p.y};
	update_base_view_position();
}

//!Returns the box of all representations put together.

void group_representation::update_base_view_position() {

	if(!data.size()) {

		base_view_position.origin=position;
		base_view_position.w=0;
		base_view_position.h=0;
		return;
	}

	base_view_position=data[0]->base_view_position;

	int	fx=base_view_position.origin.x+base_view_position.w,
	    fy=base_view_position.origin.y+base_view_position.h;

	for(const auto& r : data) {

		if(!r->is_visible()) {

			continue;
		}

		const auto& pr=r->base_view_position;

		base_view_position.origin.x=std::min(pr.origin.x, base_view_position.origin.x);
		base_view_position.origin.y=std::min(pr.origin.y, base_view_position.origin.y);

		int	prfx=pr.origin.x+pr.w,
		    prfy=pr.origin.y+pr.h;

		if(prfx > fx) fx=prfx;
		if(prfy > fy) fy=prfy;
	}

	base_view_position.w=fx-base_view_position.origin.x;
	base_view_position.h=fy-base_view_position.origin.y;

	base_view_position.origin.x+=position.x;
	base_view_position.origin.y+=position.y;
}

void group_representation::trim() {

	if(!data.size()) {

		return;
	}

	auto topleftmost=data[0]->base_view_position.origin;

	for(const auto& node : data) {

		topleftmost.x=std::min(topleftmost.x, node->base_view_position.origin.x);
		topleftmost.y=std::min(topleftmost.y, node->base_view_position.origin.y);
	}

	for(auto& node : data) {

		node->go_to(node->get_position()-topleftmost);
	}
}
