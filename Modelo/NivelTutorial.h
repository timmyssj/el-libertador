#ifndef NIVEL_TUTORIAL_H
#define NIVEL_TUTORIAL_H

#include "Nivel.h"
#include "Munieco.h"
#include "Obstaculo.h" 
#include "Item.h" // <--- NUEVO: Incluimos la clase Item
#include <fstream>
#include <iostream>

class NivelTutorial : public Nivel {
public:
	NivelTutorial() {
		setCompletado(false); 
		
		setArchivoFondo("fondos/fondo_tutorial.png");
		
		addTextoIntro("25 de Febrero de 1778");
		addTextoIntro("Jose Francisco de San Martín nace en Yapeyú,\n\nuna antigua misión jesuítica a orillas del río Uruguay,\n\nen la actual provincia de Corrientes.");
		addTextoIntro("Traslado a España, 1783-1785");
		addTextoIntro("Alli, el joven José comenzó sus estudios primarios \n\nen la Escuela de Temporalidades de Málaga,\n\ndonde recibió una educación básica en letras,\n\nmatemáticas y rudimentos de humanidades.");
		addTextoIntro("Ingreso a la Escuela Militar: El Cadete de Murcia");
		addTextoIntro("Apodado 'El Leal', San Martin forjó un caracter metódico \n\ny disciplinado.");
		addTextoIntro("Durante estos años:\n");
		addTextoIntro("Estudio táctica militar, matemáticas aplicadas a balística\n\ny geografáa.");
		addTextoIntro("Aprendio francés,\n\nlo que le permitió acceder a la literatura de la Ilustración\n\ny a los manuales militares de Napoleon mas adelante.\n\n");
		addTextoIntro("Ascendió rápidamente gracias a su valor en combate;\n\na los 15 anos ya era Segundo Subteniente.");
		setTituloIntro("TUTORIAL");
		addTextoIntro("w -> arriba\n\na -> izquierda\n\ns -> abajo\n\nd -> derecha\n\nespacio -> atacar");
		cargarContenido();
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Recoge el sable y destruye los muñecos"; 
	}
	
	void cargarContenido() override {
		// --- 1. LECTURA DEL MAPA BINARIO (nivel0.dat) ---
		std::ifstream archivoEntrada("nivel0.dat", std::ios::binary);
		
		if (archivoEntrada.is_open()) {
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
			std::cout << "[NIVEL] Mapa Tutorial cargado desde nivel0.dat" << std::endl;
		}
		
		// --- 2. SEMBRAR OBJETOS DEL MAPA BINARIO ---
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				if (getContenidoCelda(x, y) == 2) agregarEntidad(new Obstaculo(x, y, "ARBOL"));
				else if (getContenidoCelda(x, y) == 3) agregarEntidad(new Obstaculo(x, y, "ROCA"));
			}
		}
		
		// --- 3. CREAR ENTIDADES ---
		setHeroe(new SanMartin(2, 2, 0)); 
		agregarEntidad(getHeroe());
		
		// --- NUEVO: PLANTAMOS EL SABLE ---
		// Lo ponemos en X=5, Y=2 (A tres pasos a la derecha de donde nace el héroe)
		agregarEntidad(new Item(5, 2, "ITEM_SABLE")); 
		
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
					if (!hayEnemigosVivos()) {
						setCompletado(true);
					}
				}
			}
		}
	}
};

#endif
