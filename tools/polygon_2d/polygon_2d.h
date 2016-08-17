#ifndef LIBDANSDL2_POLIGON_2D_H
#define LIBDANSDL2_POLIGON_2D_H

#include <vector>
#include <stdexcept>
#include <array>

#include "../point_2d/point_2d.h"
#include "../vector_2d/vector_2d.h"
#include "../tools/tools.h"

//El polígono se asume que está declarado en el sentido de las agujas del reloj.
//Aunque

namespace ldt
{

/**
* Versión simple definida a partir de los vértices. 
*/

template<typename T>
class polygon_2d_vertexes
{
	public:

	typedef	point_2d<T>		tpoint;

					polygon_2d_vertexes()
	{
	
	}

	
					polygon_2d_vertexes(const std::vector<tpoint>& pts, tpoint c)
		:center(c), vertexes(pts)
	{

	}

	bool				is_concave() const {return is_concave(vertexes);}
	bool				is_clockwise() const {return is_clockwise(vertexes);}
	size_t				size() const {return vertexes.size();}

	virtual void			move(tpoint v)
	{
		center+=v;
		for(auto &p : vertexes) p+=v;
	}

	virtual ldt::vector_2d<double>	center_in(tpoint v)
	{
		auto vec=for_points_cartesian(this->center.x, this->center.y, v.x, v.y, false);
		for(auto &p : vertexes) p+={vec.x, vec.y};
		center=v;
		return vec;
	}

	virtual void			rotate(T grados)
	{
		for(auto &p : vertexes) p.rotate(grados, center);
	}

	virtual void			add_vertex(const tpoint& p)
	{
		if(!size()) center=p;
		vertexes.push_back(p);
	}

	tpoint				get_center() const{return center;}
	const std::vector<tpoint>&	get_vertexes() const {return vertexes;}
	const tpoint&			get_vertex(size_t v) const {return vertexes.at(v);}
	tpoint				get_vertex(size_t v) {return vertexes[v];}
	tpoint&				ref_vertex(size_t v) {return vertexes[v];}
	void				set_center(const tpoint& p) {center=p;}

	protected:

	tpoint				center;
	std::vector<tpoint>		vertexes;
};

/**
* Definición de segmento... Dos puntos y el vector de dirección.
*/

template<typename T>
struct segment_2d
{
	typedef	point_2d<T>		tpoint;
	point_2d<T>			v1, v2;
	vector_2d<T>			direction;

					segment_2d<T>(point_2d<T> pv1, point_2d<T> pv2)
		:v1(pv1), v2(pv2), direction(for_points(v1.x, v1.y, v2.x, v2.y))
	{}

					segment_2d<T>(const segment_2d<T>& o)
	:v1(o.v1), v2(o.v2), direction(o.direction)
	{}

	void				move(tpoint p)
	{
		v1+=p;
		v2+=p;
	}
};

/**
* Definición de proyección de un polígono 2d. No es más que el rango "min-max"
* de proyección contra un axis.
*/

template<typename T>
struct polygon_projection{T min, max;};

template<typename T>
bool	is_projection_overlap(const polygon_projection<T>& pa, const polygon_projection<T>& pb, bool unit_is_collision=false)
{
	return ldt::segments_overlap(pa.min, pa.max, pb.min, pb.max, unit_is_collision);
}

/**
* Definición de polígono complejo: además de los de los vértices se guardan 
* también los segments que lo componen.
*/

//Un par de forwards...
template<typename T> class polygon_2d;
template<typename T> bool SAT_collision_check(const polygon_2d<T>& a,const polygon_2d<T>& b, bool=false);

template<typename T>
class polygon_2d:
	public polygon_2d_vertexes<T>
{
	public:

	typedef	point_2d<T>		tpoint;

					polygon_2d()
		:polygon_2d_vertexes<T>()
	{
	
	}

	
					polygon_2d(const std::vector<tpoint>& pts, tpoint c)
		:polygon_2d_vertexes<T>(pts, c)
	{
		recreate_segments();
	}

	virtual void			move(tpoint v)
	{
		polygon_2d_vertexes<T>::move(v);
		for(auto &s : segments) s.move(v);
	}

	virtual ldt::vector_2d<double>	center_in(tpoint v)
	{
		auto res=polygon_2d_vertexes<T>::center_in(v);
		for(auto &s : segments) s.move({res.x, res.y});
		return res;
	}

	virtual void			rotate(T deg)
	{
		polygon_2d_vertexes<T>::rotate(deg);
		recreate_segments();
		
	}

	virtual void			add_vertex(const tpoint& p)
	{
		polygon_2d_vertexes<T>::add_vertex(p);
		if(this->vertexes.size() > 1) 
			create_segment(this->vertexes[this->vertexes.size()-2], p);

	}

	void				close()
	{
		create_segment(this->vertexes[this->vertexes.size()-1], this->vertexes[0]);
	}

	polygon_projection<T>		project(vector_2d<T> axis) const
	{
		axis.normalize();

		//Precálculo...
		const auto &vertex=this->vertexes[0];
		T vmin=cross_product(vector_2d<T>{vertex.x, vertex.y}, axis), vmax=vmin;

		for(const auto& v : this->vertexes)
		{
			if(v==*std::begin(this->vertexes)) continue; //Saltamos la primera iteración....
			T pro=cross_product(vector_2d<T>{v.x, v.y}, axis);
			if(pro < vmin) vmin=pro; 
			if (pro > vmax) vmax=pro;
		}
		return polygon_projection<T>{vmin, vmax};
	}

	const std::vector<segment_2d<T> >&	get_segments() const {return segments;}

	private:

	void				create_segment(tpoint p1, tpoint p2) {segments.push_back(segment_2d<T>{p1, p2});}
	void				recreate_segments()
	{
		if(this->vertexes.size() < 3) throw std::runtime_error("Invalid polygon defined: less than three vertexes were created.");

		segments.clear();
		size_t i=0;
		while(i < this->vertexes.size()-1)
		{
			create_segment(this->vertexes[i], this->vertexes[i+1]);
			++i;
		}

		close();
	}

	std::vector<segment_2d<T> > segments;
	friend bool SAT_collision_check<T>(const polygon_2d<T>& a,const polygon_2d<T>& b, bool);
};

template<typename T>
bool SAT_collision_check(const polygon_2d<T>& a,const polygon_2d<T>& b, bool sanity)
{
	if(sanity && (a.get_vertexes().size() != a.get_segments().size() || b.get_vertexes().size() != b.get_segments().size()))
	{
		throw std::runtime_error("ERROR: vertexes size is different from segments size.");
	}

	auto f=[](const polygon_2d<T>& pa, const polygon_2d<T>& pb)
	{
		for(const auto& s : pa.segments)
		{
			auto axis=s.direction.perpendicular(); 				//Normal del vector...
			auto proy_a=pa.project(axis), proy_b=pb.project(axis); 	//Localizar proyecciones en la normal...
			if(!is_projection_overlap(proy_a, proy_b)) return false;
		}
		return true;
	};

	if(!f(a, b)) return false;
	else if(!f(b, a)) return false;
	else return true;
}

//Adaptado de https://github.com/siebenschlaefer/line-segments-intersect/blob/included_line_segments/js/line-segments-intersect.js
template<typename T>
bool segments_intersect(const segment_2d<T>& a, const segment_2d<T>& b)
{
	auto scalar_product=[](const point_2d<T>& pa, const point_2d<T>& pb)
	{
		return (pa.x * pb.y) - (pa.y * pb.x);
	};

	auto are_equal=[](bool pa, bool pb, bool pc, bool pd)
	{
		return pa==pb && pa==pc && pa==pd;
	};

	point_2d<T> r=a.v2 - a.v1;
	point_2d<T> s=b.v2 - b.v1;
	point_2d<T> bminusa=b.v1-a.v1;

	T uNumerator=scalar_product(bminusa, r);
	T denominator=scalar_product(r, s);

	//Son parte de la misma línea.
	if(uNumerator==0.0 && denominator==0.0) 
	{
		//Si alguno de los puntos son el mismo...
		if(a.v1==b.v1 || a.v1==b.v2 || a.v2==b.v1 || a.v2==b.v2)
		{
			return true;
		}

		//Comprobación de superposicion... ¿Tienen todos los puntos en la dirección de turno el mismo signo?.
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

	//Las líneas son paralelas.
	if(denominator == 0) 
	{
		return false;
	}

	point_2d<T> npt=b.v1-a.v1;
	T u=uNumerator / denominator;
	T t=scalar_product(npt, s) / denominator;

	return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

template<typename T>
bool intersection_segment_polygon(const segment_2d<T>& s, const polygon_2d<T> p)
{
	const auto& segs=p.get_segments();

	for(const auto& sg : segs)
	{
		if(segments_intersect(s, sg)) return true;
	}

	return false;
}

//Adaptado de https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
template<typename T>
bool point_in_polygon(const polygon_2d<T> p, const point_2d<T> pt)
{
	bool res=false;
	T testx=pt.x, testy=pt.y;
	const auto& vertexes=p.get_vertexes();
	size_t i=0, j=0, nvert=vertexes.size();

	for(i = 0, j = nvert-1; i < nvert; j = i++) 
	{
		if( 
			((vertexes[i].y > testy) != (vertexes[j].y > testy)) &&
			(testx < ( vertexes[j].x-vertexes[i].x) * (testy - vertexes[i].y) / (vertexes[j].y-vertexes[i].y) + vertexes[i].x) )
		{
			res=!res;
		}
	}
	return res;
}

template<typename T>
bool is_concave(const std::vector<point_2d<T>>& vertexes)
{
	if(vertexes.size() <= 3) return false;

	size_t i=0;
	while(i < vertexes.size())
	{
		auto ptc=vertexes.at(i),
			pt1=ptc, pt2=ptc;

		if(i==0)
		{
			pt1=vertexes.at(vertexes.size()-1);
			pt2=vertexes.at(i+1);				
		}
		else if(i==vertexes.size()-1)
		{
			pt1=vertexes.at(i-1);
			pt2=vertexes.at(0);
		}
		else
		{
			pt1=vertexes.at(i-1);
			pt2=vertexes.at(i+1);
		}

		auto vector_1=for_points_cartesian(ptc.x, ptc.y, pt1.x, pt1.y);
		auto vector_2=for_points_cartesian(ptc.x, ptc.y, pt2.x, pt2.y);
		double dot=cross_product(vector_1, vector_2);
		double det=determinant(vector_1, vector_2);
		double angle=ldt::rad_to_deg(atan2(det, dot));

		angle=angle < 0.0 ? 360.0-angle : -angle;
		if(angle > 180.0) return true;
		++i;
	}

	return false;
}

/*
http://stackoverflow.com/questions/1165647/how-to-determine-if-a-list-of-polygon-points-are-in-clockwise-order

point[0] = (5,0)
point[1] = (6,4)
point[2] = (4,5)
point[3] = (1,5)
point[4] = (1,0)

point[0] = (5,0)   edge[0]: (6-5)(4+0) =   4
point[1] = (6,4)   edge[1]: (4-6)(5+4) = -18
point[2] = (4,5)   edge[2]: (1-4)(5+5) = -30
point[3] = (1,5)   edge[3]: (1-1)(0+5) =   0
point[4] = (1,0)   edge[4]: (5-1)(0+0) =   0
                                         ---
                                         -44  counter-clockwise


*/

template<typename T>
bool is_clockwise(const std::vector<point_2d<T>>& vertexes)
{
	size_t tam=vertexes.size();
	if(tam < 3) return false;

	int sum=0;
	for(size_t i=1; i<tam; ++i)
	{
		sum+=(vertexes[i].x-vertexes[i-1].x)*(vertexes[i].y+vertexes[i-1].y);
	}

	sum+=(vertexes[0].x-vertexes[tam-1].x)*(vertexes[0].y+vertexes[tam-1].y);
	return sum >= 0;	
}

}

#endif

