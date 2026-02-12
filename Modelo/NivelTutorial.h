#ifndef NIVEL_TUTORIAL_H
#define NIVEL_TUTORIAL_H

#include "Nivel.h"
#include "Munieco.h"

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
		inicializarMapaVacio(); 
		
		// 1. DISEÑO EN ZIG-ZAG (Serpiente)
		// Paredes horizontales largas para obligar a recorrer todo
		for(int x = 5; x < 25; x++) { 
			mapa[6][x] = PARED;   // Muro superior
			mapa[14][x] = PARED;  // Muro inferior
		}
		
		// Paredes verticales para cerrar los pasillos y forzar la vuelta
		for(int y = 6; y < 15; y++) { mapa[y][25] = PARED; } // Cierre derecho
		mapa[14][5] = PARED; // Un bloque para forzar la entrada
		
		// 2. LA SALIDA (Abajo a la derecha)
		mapa[18][28] = SALIDA_NIVEL;
		
		// 3. EL HÉROE (Arriba a la izquierda)
		referenciaHeroe = new SanMartin(2, 2); 
		entidades.push_back(referenciaHeroe);
		
		// 4. MUÑECOS (Estratégicamente ubicados en el camino)
		entidades.push_back(new Munieco(10, 3));  // En el primer pasillo
		entidades.push_back(new Munieco(20, 10)); // En el centro (curva)
		entidades.push_back(new Munieco(10, 17)); // En el pasillo final
	}
};

#endif
