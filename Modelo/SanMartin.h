#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>

class SanMartin : public Personaje {
private:
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba; 
	std::vector<sf::Texture> animAbajo;  
	
public:
	// VELOCIDAD AUMENTADA: De 0.05f a 0.15f
	SanMartin(float x, float y) : Personaje(x, y, 100, 0.15f) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			// RUTA CORREGIDA: Agregamos "sprites/" al inicio
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			
			if (t.loadFromFile(ruta)) {
				vector.push_back(t);
			} else {
				std::cerr << "ERROR CARGA: " << ruta << std::endl;
			}
		}
	}
	
	void cargarTextura() override {
		// Cargamos buscando dentro de la carpeta sprites
		cargarSecuencia(animDerecha, "san_martin_derecha", 4);
		cargarSecuencia(animIzquierda, "san_martin_izquierda", 4); 
		cargarSecuencia(animArriba, "san_martin_espalda", 4);
		
		// Frente (carga manual porque es una sola imagen en tu lista)
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/san_martin_frente.png")) {
			animAbajo.push_back(tFrente);
		} else {
			std::cerr << "ERROR: sprites/san_martin_frente.png" << std::endl;
		}
		
		if (!animAbajo.empty()) sprite.setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		switch (direccionActual) {
		case DERECHA:   reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA:    reproducirAnimacion(animArriba); break;
		case ABAJO:     reproducirAnimacion(animAbajo); break;
		}
	}
};

#endif
