#ifndef MUNIECO_H
#define MUNIECO_H

#include "Personaje.h"
#include <iostream>

class Munieco : public Personaje {
private:
	sf::Texture texturaMunieco;
	
public:
	Munieco(float x, float y) : Personaje(x, y, 30, 999) { 
		cargarTextura();
	}
	
	std::string getTipo() override { return "PRACTICA"; }
	
	void cargarTextura() override {
		if (!texturaMunieco.loadFromFile("sprites/munieco.png")) {
			if (texturaMunieco.loadFromFile("sprites/realista_frente.png")) {
				// CORRECCIÓN AQUÍ: Usamos getSprite()
				getSprite().setColor(sf::Color(139, 69, 19)); 
			} else {
				std::cerr << "Falta imagen para el Munieco" << std::endl;
			}
		}
		
		// CORRECCIÓN AQUÍ: Usamos getSprite()
		getSprite().setTexture(texturaMunieco);
	}
	
	void actualizar() override {
		// Estático
	}
};

#endif
