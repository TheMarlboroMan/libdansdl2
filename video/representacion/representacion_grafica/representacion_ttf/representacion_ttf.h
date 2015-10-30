#ifndef REPRESENTACION_TTF_H
#define REPRESENTACION_TTF_H

#include "../representacion_grafica.h"
#include "../../../fuente_ttf/fuente_ttf.h"
#include "../../../textura/textura.h"
#include "../../../textura/superficie.h"

namespace DLibV
{

//TODO: Debe ser muy parecida a la representación de texto. Si podemos poner la
//misma interface pública, mejor aún :D!.

//TODO: Hay que reparar el bug que hay en la representación de texto, que 
//revienta la memoria al reasignar ¬¬...

class Representacion_TTF:
	public Representacion_grafica
{
	public:

	//TODO: El constructor debe recibir fuente, texto, color.

	private:

	std::unique_ptr<Textura>	textura;

	Fuente_ttf&			fuente;
	std::string			texto;
	SDL_Color			color;
	

/*

	SDL_Color color = { 255, 255, 255, 255 };

	//Cuando vayamos a dibujar, como no estará preparada, haremos esto... 
	//Tendremos un acceso al renderer mediante la pantalla...

	//TODO: Hay varios tipos de renderText..

	Superficie s(TTF_RenderText_Blended(fuente.acc_fuente(), message.c_str(), color));

	//TODO: La textura tendría que ser un puntero... No podemos tenerla sin
	//asignar. Quizás podamos usar un unique_ptr.
	textura.reset(new Textura(surf->acc_superficie(), pantalla.acc_renderer()));
*/
};

#endif
