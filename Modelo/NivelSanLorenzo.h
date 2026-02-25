#ifndef NIVEL_SAN_LORENZO_H
#define NIVEL_SAN_LORENZO_H

#include "Nivel.h"
#include "Obstaculo.h"
#include "enemigo.h"   // <--- Tu clase Enemigo (Realista)
#include "Granadero.h" // <--- Tu clase Granadero
#include <fstream>  
#include <iostream> 

class NivelSanLorenzo : public Nivel {
public:
	NivelSanLorenzo() {
		setCompletado(false); 
		
		// El pasto a las afueras del Convento de San Carlos
		setArchivoFondo("fondos/suelo_sanlorenzo.png"); 
		
		setTituloIntro("NIVEL 2: SAN LORENZO");
		addTextoIntro("3 de Febrero de 1813. Provincia de Santa Fe.");
		addTextoIntro("Una expedicion realista desembarca cerca\n\ndel Convento de San Carlos.");
		addTextoIntro("El Coronel San Martin los espera oculto\n\ncon su nuevo regimiento:");
		addTextoIntro("Los Granaderos a Caballo.");
		addTextoIntro("Es su bautismo de fuego en territorio americano.");
		addTextoIntro("Mision: Rechazar el avance realista hacia el convento.");
		
		cargarContenido();
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Vence a los realistas y asegura la zona"; 
	}
	
	void cargarContenido() override {
		std::ifstream archivoEntrada("nivel2.dat", std::ios::binary);
		
		if (archivoEntrada.is_open()) {
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
			std::cout << "[NIVEL] Mapa San Lorenzo cargado desde nivel2.dat" << std::endl;
		} else {
			std::cerr << "[ERROR] No se encontro nivel2.dat. Cargando mapa por defecto." << std::endl;
			inicializarMapaVacio();
			// Borde perimetral de rocas (3) para evitar crasheos
			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 30; x++) {
					if (y == 0 || y == 19 || x == 0 || x == 29) setCelda(x, y, 3);
				}
			}
			setCelda(15, 0, SALIDA_NIVEL);
		}
		
		// Sembrar obstáculos 3D
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				if (getContenidoCelda(x, y) == 2) agregarEntidad(new Obstaculo(x, y, "ARBOL"));
				else if (getContenidoCelda(x, y) == 3) agregarEntidad(new Obstaculo(x, y, "ROCA"));
			}
		}
		
		// --- 1. Nace el Héroe ---
		setHeroe(new SanMartin(15, 17, 1)); 
		agregarEntidad(getHeroe()); 
		
		// --- 2. Nacen los Granaderos en Formación de Pinza ---
		// Le pasamos -1 (Izquierda), 1 (Derecha), -2 (Extremo Izquierdo), 2 (Extremo Derecho)
		agregarEntidad(new Granadero(13, 17, getHeroe(), -1));
		agregarEntidad(new Granadero(17, 17, getHeroe(), 1));
		agregarEntidad(new Granadero(11, 18, getHeroe(), -2));
		agregarEntidad(new Granadero(19, 18, getHeroe(), 2));
		
		// --- 3. Nace el Ejército Realista (Reemplazando a los Franceses) ---
		// Nota: Asegúrate de que el constructor de Enemigo reciba (x, y, heroe)
		agregarEntidad(new Enemigo(10, 5, getHeroe()));
		agregarEntidad(new Enemigo(15, 4, getHeroe()));
		agregarEntidad(new Enemigo(20, 5, getHeroe()));
		agregarEntidad(new Enemigo(12, 3, getHeroe()));
		agregarEntidad(new Enemigo(18, 3, getHeroe()));
		
		// --- Activar las colisiones para todos ---
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
