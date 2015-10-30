#ifndef REPRESENTACION_TTF_H
#define REPRESENTACION_TTF_H

#include "../representacion_grafica.h"
#include "../../../fuente_ttf/fuente_ttf.h"
#include "../../../textura/textura.h"
#include "../../../textura/superficie.h"

namespace DLibV
{
class Representacion_TTF:
	public Representacion_grafica
{
	public:

					Representacion_TTF(const Fuente_TTF&, SDL_Color, const std::string&);
					Representacion_TTF(const Fuente_TTF&, int r, int g, int b, int a, const std::string&);
					Representacion_TTF(const Fuente_TTF&, SDL_Color);
					Representacion_TTF(const Fuente_TTF&, int r, int g, int b, int a);
					Representacion_TTF(const Representacion_TTF&);
	Representacion_TTF&		operator=(const Representacion_TTF&);
	virtual				~Representacion_TTF();

	const std::string& 		acc_cadena() const {return this->cadena;}
	int				acc_tamano_fuente() const {return fuente->acc_tamano_fuente();}
	const std::string&		acc_nombre_fuente() const {return fuente->acc_nombre_fuente();}

	void 				preparar(const SDL_Renderer * renderer);
	void 				modificar_fuente(const Fuente_TTF&);
	virtual void 			asignar(const char);
	virtual void 			asignar(const char *);
	virtual void 			asignar(const std::string& p_param);
	virtual bool 			es_estatica() const=0;

	private:

	std::unique_ptr<Textura>	textura;

	//Esto es un puntero a conciencia, para poderlo cambiar.
	const Fuente_ttf *		fuente;
	std::string			cadena;
	SDL_Color			color;
};

#endif
