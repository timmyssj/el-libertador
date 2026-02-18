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
	// Delay de 6 frames (ágil)
	SanMartin(float x, float y) : Personaje(x, y, 100, 6) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) {
				vector.push_back(t);
			} else {
				std::cerr << "Falta: " << ruta << std::endl;
			}
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "san_martin_derecho", 4);
		cargarSecuencia(animIzquierda, "san_martin_izquierdo", 4); 
		cargarSecuencia(animArriba, "san_martin_atras", 4);
		
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/san_martin_frente.png")) {
			animAbajo.push_back(tFrente);
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
