#ifndef REPRESENTACION_TEXTO_AUTO_H
#define REPRESENTACION_TEXTO_AUTO_H

#include "representacion_texto.h"

/*
Mucho ojo con esta: por cada vez que se asigna la cadena reconstruye de nuevo
lo que se dibuja. Normalmente no usaríamos esta clase para textos que puedan
cambiar: para esos es mejor usar una fija con el tamaño suficiente.
*/

namespace DLibV
{

	
class Representacion_texto_auto:public Representacion_texto
{
	private:

	void establecer_dimensiones_por_contenido();

	public:

	virtual void asignar(const char);
	virtual void asignar(const char *);
	virtual void asignar(std::string const& p_param);

	Representacion_texto_auto(const DLibV::Superficie * p_rep, const char p_char)
		:Representacion_texto(p_rep)
	{
		this->asignar(p_char);
	}

	Representacion_texto_auto(const DLibV::Superficie * p_rep, const char * p_char)
		:Representacion_texto(p_rep)
	{
		if(p_char) this->asignar(p_char);
	}

	Representacion_texto_auto(const DLibV::Superficie * p_rep, std::string const& p_cad)
		:Representacion_texto(p_rep)
	{
		this->asignar(p_cad);
	}

	Representacion_texto_auto(const Representacion_texto_auto&);
	virtual ~Representacion_texto_auto() {}
	Representacion_texto_auto& operator=(const Representacion_texto_auto&);
};

} //Fin namespace DLibV

#endif
