#ifndef NIVEL_ANDES_H
#define NIVEL_ANDES_H

#include "Nivel.h"
#include "SanMartin.h"
#include "Enemigo.h"
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
		framesFrio = 0;
		tiempoRestante = 140; // Tu límite exacto de 140 segundos
		
		std::vector<std::string> historia = {
			"El frio extremo congela la sangre y agota las fuerzas.",
				"Tienes 140 segundos antes de morir de hipotermia.",
				"La vida baja constantemente. Desviate del camino principal",
				"para encontrar suministros y curarte.",
				"Encuentra la salida del laberinto andino."
		};
		setTextoIntro("EL CRUCE DE LOS ANDES", historia);
		
		cargarContenido();
	}
	
	void cargarContenido() override {
		int mapa[20][30] = {0}; 
		
		// Asegúrate de que el nombre del archivo coincida con el que escupe tu generador
		std::ifstream archivo("niveles/nivel_andes.dat", std::ios::binary);
		
		if (archivo.is_open()) {
			archivo.read(reinterpret_cast<char*>(mapa), sizeof(mapa));
			archivo.close();
			std::cout << "[INFO] Mapa de los Andes cargado desde binario." << std::endl;
		} else {
			std::cout << "[ERROR] No se pudo cargar el archivo binario del nivel." << std::endl;
			return; 
		}
		
		// --- TRADUCCIÓN DEL MAPA (Ajusta estos IDs a los que use tu generador) ---
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				
				int idCelda = mapa[y][x];
				
				switch (idCelda) {
				case 1: // PARED / NIEVE
					setContenidoCelda(x, y, 1); 
					break;
					
				case 2: // SAN MARTÍN
					setHeroe(new SanMartin(x, y, 2)); // Traje 2 (Abrigado)
					agregarEntidad(getHeroe());
					break;
					
				case 3: // ENEMIGO REALISTA
					if (getHeroe()) agregarEntidad(new Enemigo(x, y, getHeroe()));
					break;
					
				case 4: // CURA (+20 de Vida)
					agregarEntidad(new Item(x, y, "ITEM_CURACION"));
					break;
					
				case 9: // ZONA DE SALIDA
					setContenidoCelda(x, y, SALIDA_NIVEL);
					break;
				}
			}
		}
	}
	
	int getTiempoRestante() const { return tiempoRestante; }
	
	void actualizar() override {
		if (enCinematica()) return;
		
		for (Entidad* e : getEntidades()) {
			if (e->estaVivo()) e->actualizar();
		}
		
		// --- EL RELOJ DE LA MUERTE (Frío) ---
		if (getHeroe() && getHeroe()->estaVivo()) {
			framesFrio++;
			
			// Si el juego corre a 60 FPS, 60 frames = 1 segundo real
			if (framesFrio >= 60) { 
				framesFrio = 0;
				tiempoRestante--;
				
				// Restamos 1 punto de vida exacto por segundo
				getHeroe()->recibirDanio(1.0f); 
				
				// Opcional: imprimir en consola para testear
				std::cout << "Tiempo: " << tiempoRestante << "s | Vida: " << getHeroe()->getVida() << std::endl;
				
				// Condición de derrota por tiempo/frío
				if (tiempoRestante <= 0) {
					getHeroe()->recibirDanio(999.0f); // Muerte instantánea
					std::cout << "¡El frio de los Andes te ha vencido!" << std::endl;
				}
			}
			
			// Condición de victoria
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
