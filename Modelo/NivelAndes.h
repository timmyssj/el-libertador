#ifndef NIVEL_ANDES_H
#define NIVEL_ANDES_H

#include "Nivel.h"
#include "SanMartin.h"
#include "Item.h" // ¡Ahora incluimos las curas!
#include <fstream>
#include <iostream>

class NivelAndes : public Nivel {
public:
	NivelAndes() {
		setCompletado(false);
		setArchivoFondo("fondos/andes.png"); 
		
		setTituloIntro("EL CRUCE DE LOS ANDES");
		addTextoIntro("Prueba de nivel basico.");
		addTextoIntro("Solo debes poder moverte por la nieve y agarrar curas.");
		
		cargarContenido();
	}
	
	void cargarContenido() override {
		// 1. LEER EL MAPA DIRECTAMENTE
		std::ifstream archivoEntrada("nivel4.dat", std::ios::binary);
		
		if (archivoEntrada.is_open()) {
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
			std::cout << "[NIVEL] Mapa Andes cargado desde nivel4.dat" << std::endl;
		} else {
			std::cerr << "[ERROR] No se encontro nivel4.dat." << std::endl;
			inicializarMapaVacio();
		}
		
		// 2. BUSCAR ENTIDADES EN EL MAPA
		bool heroeEncontrado = false;
		
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				int idCelda = getContenidoCelda(x, y);
				
				if (idCelda == 2) { // San Martín
					setHeroe(new SanMartin(x, y, 2)); 
					agregarEntidad(getHeroe());
					setCelda(x, y, 0); // Convertimos en piso
					heroeEncontrado = true;
				}
				else if (idCelda == 6) { // NUEVO: ¡Aparecen las curas!
					agregarEntidad(new Item(x, y, "ITEM_CURACION"));
					setCelda(x, y, 0); // Lo volvemos piso (el objeto visual queda flotando)
				}
				// La salida (4) y las paredes (1) quedan intactas en el mapa
			}
		}
		
		// 3. PARCHE ANTI-CRASHEO
		if (!heroeEncontrado) {
			std::cout << "[ALERTA] Forzando spawn de San Martin en el medio." << std::endl;
			setHeroe(new SanMartin(15, 10, 2)); 
			agregarEntidad(getHeroe());
		}
	}
	
	void actualizar() override {
		if (enCinematica()) return;
		
		for (Entidad* e : getEntidades()) {
			if (e->estaVivo()) e->actualizar();
		}
		
		// Verificamos si pisó la zona dorada de victoria (SALIDA_NIVEL = 4)
		if (getHeroe() && getHeroe()->estaVivo()) {
			int hx = (int)getHeroe()->getX();
			int hy = (int)getHeroe()->getY();
			if (hx >= 0 && hx < 30 && hy >= 0 && hy < 20) {
				if (getContenidoCelda(hx, hy) == SALIDA_NIVEL) {
					setCompletado(true);
				}
			}
		}
	}
};

#endif
