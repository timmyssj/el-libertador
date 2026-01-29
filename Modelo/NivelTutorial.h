#ifndef NIVEL_TUTORIAL_H
#define NIVEL_TUTORIAL_H

#include "Nivel.h"

class NivelTutorial : public Nivel {
public:
	// Al crear el tutorial, cargamos su contenido automáticamente
	NivelTutorial() {
		// 1. Definir la Historia (Contexto 1812)
		textoIntro.push_back("25 de Febrero de 1778");
		textoIntro.push_back("Jose Francisco de San Martin nace en Yapeyú,\n"
		"una antigua mision jesuitica a orillas del rio Uruguay,\n"
		"en la actual provincia de Corrientes.");
		textoIntro.push_back("Traslado a España, 1783-1785");
		textoIntro.push_back("Allí, el joven José comenzó sus estudios primarios \n"
		"en la Escuela de Temporalidades de Málaga,\n" 
		"donde recibió una educación básica en letras,\nmatemáticas y rudimentos de humanidades.");
		textoIntro.push_back("Ingreso a la Escuela Militar: El Cadete de Murcia");
		textoIntro.push_back("Apodado 'El Leal', San Martín forjó un carácter metódico \ny disciplinado.");
		textoIntro.push_back("Durante estos años:\n");
		textoIntro.push_back("Estudió táctica militar, matemáticas aplicadas a balística\ny geografía.");
		textoIntro.push_back("Aprendió francés,\nlo que le permitió acceder a la literatura de la Ilustración\n"
		"y a los manuales militares de Napoleón más adelante.\n");
		textoIntro.push_back("Ascendió rápidamente gracias a su valor en combate; \n"
		"a los 15 años ya era Segundo Subteniente.");
		cargarContenido();
	}
	
	void cargarContenido() override {
		// 1. Usamos el helper del padre para limpiar
		inicializarMapaVacio();
		
		// 2. Diseño específico del Tutorial (Paredes internas)
		for(int i = 5; i < 15; i++) { mapa[i][10] = PARED; }
		
		// 3. Meta
		mapa[18][28] = SALIDA_NIVEL;
		
		// 4. Entidades
		// Nota: Agregamos a 'entidades' directamente porque es 'protected'
		referenciaHeroe = new SanMartin(2, 2);
		entidades.push_back(referenciaHeroe);
		
		entidades.push_back(new Enemigo(15, 5, referenciaHeroe));
		entidades.push_back(new Enemigo(15, 15, referenciaHeroe));
	}
};

#endif
