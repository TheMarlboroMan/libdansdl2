#include "fuente_ttf.h"

using namespace DLibV;

Fuente_TTF::Fuente_TTF(const std::string& f, int t)
	:nombre_fuente(f), tamano_fuente(t),
	fuente(nullptr)
{
	fuente=TTF_OpenFont(nombre_fuente.c_str(), tamano_fuente);
	if(fuente==nullptr)
	{
		throw std::runtime_error("ERROR: Imposible cargar fuente "+nombre_fuente);
	}
}

Fuente_TTF::~Fuente_TTF()
{
	if(fuente)
	{
		TTF_CloseFont(fuente);
	}
}
