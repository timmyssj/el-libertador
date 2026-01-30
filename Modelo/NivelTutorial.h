#ifndef NIVEL_TUTORIAL_H
#define NIVEL_TUTORIAL_H

#include "Nivel.h"
#include "Granadero.h"

class NivelTutorial : public Nivel {
public:
	// Al crear el tutorial, cargamos su contenido automáticamente
	NivelTutorial() {
		textoIntro.push_back("25 de Febrero de 1778");
		textoIntro.push_back("José Francisco de San Martín nace en Yapeyú,\n\n"
		"una antigua misión jesuítica a orillas del río Uruguay,\n\n"
		"en la actual provincia de Corrientes.");
		textoIntro.push_back("Traslado a España, 1783-1785");
		textoIntro.push_back("Allí, el joven José comenzó sus estudios primarios \n\n"
		"en la Escuela de Temporalidades de Málaga,\n\n" 
		"donde recibió una educación básica en letras,\n\nmatemáticas y rudimentos de humanidades.");
		textoIntro.push_back("Ingreso a la Escuela Militar: El Cadete de Murcia");
		textoIntro.push_back("Apodado 'El Leal', San Martín forjó un carácter metódico \n\ny disciplinado.");
		textoIntro.push_back("Durante estos años:\n");
		textoIntro.push_back("Estudió táctica militar, matemáticas aplicadas a balística\n\ny geografía.");
		textoIntro.push_back("Aprendió francés,\n\nlo que le permitió acceder a la literatura de la Ilustración\n\n"
		"y a los manuales militares de Napoleón más adelante.\n\n");
		textoIntro.push_back("Ascendió rápidamente gracias a su valor en combate;\n\n"
		"a los 15 años ya era Segundo Subteniente.");
		tituloIntro = "TUTORIAL";
		textoIntro.push_back(
		"w -> arriba\n\n"
		"a -> izquierda\n\n"
		"s -> abajo\n\n"
		"d -> derecha\n\n"
		"espacio -> atacar");
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
		
		entidades.push_back(new Granadero(2, 4, referenciaHeroe));
	}
};

#endif
