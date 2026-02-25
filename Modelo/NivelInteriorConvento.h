#ifndef NIVEL_INTERIOR_CONVENTO_H
#define NIVEL_INTERIOR_CONVENTO_H

#include "Nivel.h"
#include "Obstaculo.h"
#include "enemigo.h"
#include "SanMartin.h"
#include <fstream>  

class NivelInteriorConvento : public Nivel {
public:
	NivelInteriorConvento() {
		setCompletado(false); 
		// Si tienes una textura de suelo de madera/piedra ponla aquí, sino usa el pasto:
		setArchivoFondo("fondos/pasto.png"); 
		
		setTituloIntro("INTERIOR DEL CONVENTO");
		addTextoIntro("Los realistas han tomado el claustro.");
		addTextoIntro("Debes despejar el área para asegurar");
		addTextoIntro("la posición y salvar a los monjes.");
		
		cargarContenido();
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Elimina a los realistas del claustro"; 
	}
	
	void cargarContenido() override {
		std::ifstream archivoEntrada("nivel3.dat", std::ios::binary);
		if (archivoEntrada.is_open()) {
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
		} else {
			inicializarMapaVacio();
		}
		
		// Héroe (Aparece abajo, mirando hacia arriba, como si acabara de entrar)
		setHeroe(new SanMartin(15, 17, 2)); 
		agregarEntidad(getHeroe()); 
		
		// Enemigos atrincherados
		agregarEntidad(new Enemigo(10, 8, getHeroe()));
		agregarEntidad(new Enemigo(20, 8, getHeroe()));
		agregarEntidad(new Enemigo(15, 5, getHeroe()));
		
		// Cargar los obstáculos (Muros internos o pilares que pusimos en el nivel3.dat)
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				// --- CORRECCIÓN: Cambiamos "ROCA" por "PILAR" ---
				if (getContenidoCelda(x, y) == 3) agregarEntidad(new Obstaculo(x, y, "PILAR"));
			}
		}
		
		// Darle a los personajes el radar del mapa
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
			
			// Si llega a la salida dorada del fondo y no hay enemigos, ganamos de verdad
			if (x >= 0 && x < 30 && y >= 0 && y < 20) {
				if (getContenidoCelda(x, y) == SALIDA_NIVEL) {
					if (!hayEnemigosVivos()) setCompletado(true);
				}
			}
		}
	}
};

#endif
