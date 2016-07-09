#ifndef REPRESENTACION_AGRUPADA_H
#define REPRESENTACION_AGRUPADA_H

#include "../representacion.h"
#include "../../../herramientas/herramientas_sdl/herramientas_sdl.h"
#include <vector>

namespace DLibV
{

/*Tiene un vector de varias representaciones. Cuando se lo pasamos a la pantalla
las vuelca en el orden en que se han insertado. Las representacions deben
ser estáticas  o dinámicas, según el tipo de grupo. 

Una vez insertadas, el grupo toma posesión de las representaciones y se encarga 
de destruirlas al finalizar su tiempo de vida.*/

class Representacion_agrupada:public Representacion
{
	public:

				Representacion_agrupada(int, int, bool=true);
				Representacion_agrupada(const Representacion_agrupada&);
	virtual 		~Representacion_agrupada();
	void	 		insertar_representacion(Representacion *);

	void 			mut_imponer_alpha(bool v) {impone_alpha=v;}
	void 			mut_imponer_modo_blend(bool v) {impone_modo_blend=v;}
	void 			vaciar_grupo();

	unsigned int 		obtener_cuenta() const {return grupo.size();}

	virtual void 		ir_a(int x, int y);
	virtual Punto		obtener_posicion() const;
	virtual Rect		obtener_base_posicion_vista() const;

	std::vector<Representacion *>&	obtener_grupo() {return grupo;}

	virtual void		volcar(Pantalla&, const Camara&);
	virtual void		volcar(Pantalla&);

	private:

	void			volcado_interno(Pantalla&, Camara const *);

	int			x, y;
	bool			posee_las_representaciones,
				impone_alpha,
				impone_modo_blend;

	Representacion_agrupada& 	operator=(const Representacion_agrupada &);	//OJO: Es incopiable!.

	protected:

	std::vector<Representacion *> 	grupo;

	virtual void		volcado();
};

}

#endif
