#ifndef LIBDANSDL2_GROUP_REPRESENTATION_H
#define LIBDANSDL2_GROUP_REPRESENTATION_H

#include <vector>

#include "../representation.h"
#include "../../../tools/sdl_tools/sdl_tools.h"

namespace ldv
{

/*Tiene un vector de varias representaciones. Cuando se lo pasamos a la screen
las vuelca en el orden en que se han insertado. Las representacions deben
ser estáticas  o dinámicas, según el tipo de grupo. 

Una vez insertadas, el grupo toma posesión de las representaciones y se encarga 
de destruirlas al finalizar su tiempo de vida.*/

class group_representation:
	public representation
{
	public:

				group_representation(point, bool=true);
				group_representation(const group_representation&);
	virtual 		~group_representation();
	void	 		insert(representation *);

	void 			clear();
	unsigned int 		size() const {return data.size();}

	virtual void 		go_to(int x, int y);
	virtual point		get_position() const;
	virtual rect		get_base_view_position() const;

	std::vector<representation *>&	get_data() {return data;}

	virtual void		draw(screen&, const camera&, bool=false);
	virtual void		draw(screen&, bool=false);

	private:

	void			draw_internal(screen&, camera const *);

	point			position;
	bool			owns_data;

	group_representation& 	operator=(const group_representation &);	//OJO: Es incopiable!.

	protected:

	std::vector<representation *> 	data;

	virtual void		do_draw();
};

}

#endif
