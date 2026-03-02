#ifndef NIVEL_ANDES_H
#define NIVEL_ANDES_H

#include "Nivel.h"
#include "SanMartin.h"
#include "Item.h"
#include <fstream>
#include <iostream>

class NivelAndes : public Nivel {
private:
	int framesFrio;
	int tiempoRestante;
	
public:
	NivelAndes() {
		setCompletado(false);
		setArchivoFondo("fondos/andes.png"); 
		framesFrio = 0;
		tiempoRestante = 140; 
		
		setTituloIntro("EL CRUCE DE LOS ANDES");
		addTextoIntro("El frio extremo congela la sangre y agota las fuerzas.");
		addTextoIntro("Tienes 140 segundos antes de morir de hipotermia.");
		addTextoIntro("La vida baja constantemente. Desviate del camino principal");
		addTextoIntro("para encontrar suministros (+10 de vida).");
		addTextoIntro("Encuentra la salida a Chile.");
		
		cargarContenido();
	}
	
	void cargarContenido() override {
		std::ifstream archivoEntrada("nivel4.dat", std::ios::binary);
		if (archivoEntrada.is_open()) {
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
		} else {
			inicializarMapaVacio();
		}
		
		bool heroeEncontrado = false;
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				int idCelda = getContenidoCelda(x, y);
				
				if (idCelda == 2) { 
					setHeroe(new SanMartin(x, y, 2)); 
					agregarEntidad(getHeroe());
					setCelda(x, y, 0); 
					heroeEncontrado = true;
				}
				else if (idCelda == 6) { 
					agregarEntidad(new Item(x, y, "ITEM_CURACION"));
					setCelda(x, y, 0); 
				}
			}
		}
		
		if (!heroeEncontrado) {
			setHeroe(new SanMartin(15, 10, 2)); 
			agregarEntidad(getHeroe());
		}
	}
	
	int getTiempoRestante() override { return tiempoRestante; }
	
	int getCurasRestantes() override {
		int contador = 0;
		for (Entidad* e : getEntidades()) {
			// Si la entidad está viva y es una cura, la sumamos al contador
			if (e->estaVivo() && e->getTipo() == "ITEM_CURACION") {
				contador++;
			}
		}
		return contador;
	}
	
	void actualizar() override {
		if (enCinematica()) return;
		
		for (Entidad* e : getEntidades()) {
			if (e->estaVivo()) e->actualizar();
		}
		
		if (getHeroe() && getHeroe()->estaVivo()) {
			
			// --- SISTEMA DE FRÍO EXTREMO ---
			framesFrio++;
			if (framesFrio >= 60) { 
				framesFrio = 0;
				tiempoRestante--;
				getHeroe()->recibirDanio(5.0f); // Resta 1 HP por segundo
				
				if (tiempoRestante <= 0) {
					getHeroe()->recibirDanio(999.0f); // Muerte por tiempo
				}
			}
			
			int hx = (int)getHeroe()->getX();
			int hy = (int)getHeroe()->getY();
			if (hx >= 0 && hx < 30 && hy >= 0 && hy < 20) {
				if (getContenidoCelda(hx, hy) == SALIDA_NIVEL || getContenidoCelda(hx, hy) == 4) {
					setCompletado(true);
				}
			}
		}
	}
};

#endif
