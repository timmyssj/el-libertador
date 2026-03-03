#ifndef NIVEL_ANDES_H
#define NIVEL_ANDES_H

#include "Nivel.h"
#include "SanMartin.h"
#include "Item.h"
#include <fstream>
#include <iostream>
#include <SFML/Audio.hpp> 

class NivelAndes : public Nivel {
private:
	int framesFrio;
	int tiempoRestante;
	
	sf::SoundBuffer bufferViento;
	sf::Sound sonidoViento;
	bool vientoIniciado;
	
public:
	NivelAndes() {
		setCompletado(false);
		setArchivoFondo("fondos/andes.png"); 
		framesFrio = 0;
		tiempoRestante = 140; 
		vientoIniciado = false;
		
		if (bufferViento.loadFromFile("sonidos/viento.wav")) {
			sonidoViento.setBuffer(bufferViento);
			sonidoViento.setLoop(true); 
			sonidoViento.setVolume(50.0f); 
		}
		
		// --- INTRODUCCIÓN CORREGIDA (Estilo Nivel España) ---
		setTituloIntro("NIVEL 3: CRUCE DE LOS ANDES");
		addTextoIntro("Ano 1817. Cordillera de los Andes.");
		addTextoIntro("El Ejercito de los Andes enfrenta su mayor desafio.");
		addTextoIntro("A mas de 4000 metros de altura, el frio extremo\ny la falta de oxigeno son letales.");
		addTextoIntro("Mision: Cruzar el paso en 140s. Recoge botiquines\npara sobrevivir a la hipotermia.");
		
		cargarContenido();
	}
	
	~NivelAndes() {
		if (sonidoViento.getStatus() == sf::Sound::Playing) {
			sonidoViento.stop();
		}
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Sobrevive a la hipotermia y ve a la salida"; 
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
			if (e->estaVivo() && e->getTipo() == "ITEM_CURACION") {
				contador++;
			}
		}
		return contador;
	}
	
	void actualizar() override {
		if (enCinematica()) return;
		
		if (!vientoIniciado) {
			sonidoViento.play();
			vientoIniciado = true;
		}
		
		for (Entidad* e : getEntidades()) {
			if (e->estaVivo()) e->actualizar();
		}
		
		if (getHeroe() && getHeroe()->estaVivo()) {
			
			// --- SISTEMA DE FRÍO EXTREMO ---
			framesFrio++;
			if (framesFrio >= 60) { 
				framesFrio = 0;
				tiempoRestante--;
				
				getHeroe()->recibirDanio(5.0f); 
				
				if (tiempoRestante <= 0) {
					getHeroe()->recibirDanio(999.0f); 
					sonidoViento.stop(); 
				}
			}
			
			int hx = (int)getHeroe()->getX();
			int hy = (int)getHeroe()->getY();
			if (hx >= 0 && hx < 30 && hy >= 0 && hy < 20) {
				if (getContenidoCelda(hx, hy) == SALIDA_NIVEL || getContenidoCelda(hx, hy) == 4) {
					setCompletado(true);
					sonidoViento.stop(); 
				}
			}
		} else {
			sonidoViento.stop();
		}
	}
};

#endif
