#ifndef NIVEL_TUTORIAL_H
#define NIVEL_TUTORIAL_H

#include "Nivel.h"
#include "Munieco.h"

class NivelTutorial : public Nivel {
public:
	NivelTutorial() {
		addTextoIntro("25 de Febrero de 1778");
		addTextoIntro("Jose Francisco de San Martin nace en Yapeyu,\n\nuna antigua mision jesuitica a orillas del rio Uruguay,\n\nen la actual provincia de Corrientes.");
		addTextoIntro("Traslado a Espana, 1783-1785");
		addTextoIntro("Alli, el joven Jose comenzo sus estudios primarios \n\nen la Escuela de Temporalidades de Malaga,\n\ndonde recibio una educacion basica en letras,\n\nmatematicas y rudimentos de humanidades.");
		addTextoIntro("Ingreso a la Escuela Militar: El Cadete de Murcia");
		addTextoIntro("Apodado 'El Leal', San Martin forjo un caracter metodico \n\ny disciplinado.");
		addTextoIntro("Durante estos anos:\n");
		addTextoIntro("Estudio tactica militar, matematicas aplicadas a balistica\n\ny geografia.");
		addTextoIntro("Aprendio frances,\n\nlo que le permitio acceder a la literatura de la Ilustracion\n\ny a los manuales militares de Napoleon mas adelante.\n\n");
		addTextoIntro("Ascendio rapidamente gracias a su valor en combate;\n\na los 15 anos ya era Segundo Subteniente.");
		setTituloIntro("TUTORIAL");
		addTextoIntro("w -> arriba\n\na -> izquierda\n\ns -> abajo\n\nd -> derecha\n\nespacio -> atacar");
		cargarContenido();
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Destruye los maniquies (ESPACIO)"; 
	}
	
	void cargarContenido() override {
		inicializarMapaVacio(); 
		
		for(int x = 5; x < 25; x++) { 
			setCelda(x, 6, PARED);   
			setCelda(x, 14, PARED);  
		}
		for(int y = 6; y < 15; y++) { setCelda(25, y, PARED); } 
		setCelda(5, 14, PARED); 
		
		setCelda(28, 18, SALIDA_NIVEL);
		
		setHeroe(new SanMartin(2, 2, 0)); 
		agregarEntidad(getHeroe());
		
		agregarEntidad(new Munieco(10, 3));  
		agregarEntidad(new Munieco(20, 10)); 
		agregarEntidad(new Munieco(10, 17)); 
		
		for (Entidad* e : getEntidades()) {
			Personaje* p = dynamic_cast<Personaje*>(e);
			if (p) p->setMapaEntidades(&getEntidades());
		}
	}
	
	void actualizar() override {
		for (Entidad* e : getEntidades()) {
			if (e->estaVivo()) e->actualizar();
		}
		
		if (getHeroe() && getHeroe()->estaVivo()) {
			int x = (int)getHeroe()->getX();
			int y = (int)getHeroe()->getY();
			if (x >= 0 && x < 30 && y >= 0 && y < 20) {
				if (getContenidoCelda(x, y) == SALIDA_NIVEL) {
					setCompletado(true);
				}
			}
		}
	}
};

#endif
