#ifndef NIVEL_ESPANA_H
#define NIVEL_ESPANA_H

#include "Nivel.h"
#include "Obstaculo.h"
#include "Frances.h"
#include "Aliado.h"
#include <fstream>  
#include <iostream> 

class NivelEspana : public Nivel {
public:
	NivelEspana() {
		setTituloIntro("NIVEL 1: BATALLA EN ESPANA");
		addTextoIntro("Ano 1808. Arjonilla, Espana.");
		addTextoIntro("Las tropas de Napoleon invaden la peninsula.");
		addTextoIntro("El joven Capitan San Martin lucha junto\nal ejercito espanol.");
		addTextoIntro("Mision: Emboscar a la patrulla francesa.");
		cargarContenido();
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Elimina a los realistas y ve a la salida"; 
	}
	
	void cargarContenido() override {
		std::ifstream archivoEntrada("nivel1.dat", std::ios::binary);
		
		if (archivoEntrada.is_open()) {
			// Usamos el Getter de Puntero para la lectura cruda
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
			std::cout << "[NIVEL] Mapa Espana cargado desde nivel1.dat" << std::endl;
		} else {
			std::cerr << "[ERROR] No se encontro nivel1.dat. Cargando mapa por defecto." << std::endl;
			inicializarMapaVacio();
			// Llenamos el mapa usando setCelda(columna, fila, valor)
			for(int y = 0; y < 20; y++) {
				if (y % 3 == 0) { setCelda(2, y, PARED); setCelda(27, y, PARED); }
				if (y % 4 == 0) { setCelda(5, y, PARED); setCelda(24, y, PARED); }
			}
			setCelda(14, 10, PARED); setCelda(15, 10, PARED);
			setCelda(14, 11, PARED); setCelda(15, 11, PARED);
			setCelda(15, 0, SALIDA_NIVEL);
		}
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				// Leemos con getContenidoCelda
				if (getContenidoCelda(x, y) == 2) agregarEntidad(new Obstaculo(x, y, "ARBOL"));
				else if (getContenidoCelda(x, y) == 3) agregarEntidad(new Obstaculo(x, y, "ROCA"));
			}
		}
		
		setHeroe(new SanMartin(10, 17, 1)); 
		agregarEntidad(getHeroe()); 
		
		agregarEntidad(new Aliado(11, 17, getHeroe()));
		agregarEntidad(new Aliado(12, 19, getHeroe()));
		agregarEntidad(new Aliado(13, 17, getHeroe()));
		agregarEntidad(new Aliado(14, 19, getHeroe()));
		agregarEntidad(new Aliado(16, 19, getHeroe()));
		agregarEntidad(new Aliado(17, 17, getHeroe()));
		agregarEntidad(new Aliado(18, 19, getHeroe()));
		agregarEntidad(new Aliado(19, 17, getHeroe()));
		
		agregarEntidad(new Frances(11, 6, getHeroe()));
		agregarEntidad(new Frances(13, 5, getHeroe()));
		agregarEntidad(new Frances(15, 6, getHeroe()));
		agregarEntidad(new Frances(15, 2, getHeroe()));
		agregarEntidad(new Frances(17, 5, getHeroe()));
		agregarEntidad(new Frances(19, 6, getHeroe()));
		
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
					
					// --- NUEVO: Solo ganas si el mapa está limpio ---
					if (!hayEnemigosVivos()) {
						setCompletado(true);
					}
					
				}
			}
		}
	}
};

#endif
