#include <ldv/image.h>

using namespace ldv;

//!Creates an image from the surface.

//!The surface will be owned by the image from then on.

image::image(SDL_Surface * psurface)
	:surface(psurface)
{

}

//!Creates an image from a string path.
image::image(const std::string& path)
	:surface(ldv::load_image(path))
{

}

/* Use example:
std::string funk=tools::dump_file("file.jpg");
std::vector<unsigned char> raw(std::begin(funk), std::end(funk));

ldv::image img(raw);
ldv::texture tex(img);

ldv::rect r{0,0, tex.get_w(), tex.get_h()};
ldv::bitmap_representation bmp(tex, r, r);
*/

image::image(const std::vector<unsigned char>& _seq)
	:surface(ldv::load_image_from_memory(_seq)) {

}

image::image(const unsigned char * _seq, size_t _size)
	:surface(ldv::load_image_from_memory(_seq, _size)) {

}


//!Assignment operator.

//!Makes a copy of the underlying surface without changing the reference.

image& image::operator=(const image& o)
{
	surface::operator=(o);
	return *this;
}

//!Copy constructor.

//!Makes a copy of the underlying surface without changing the reference.

image::image(const image& o)
	:surface(o)
{

}

image::~image()
{

}
