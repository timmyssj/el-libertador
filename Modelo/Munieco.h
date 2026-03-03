#ifndef MUNIECO_H
#define MUNIECO_H

#include "Personaje.h"
#include <iostream>

class Munieco : public Personaje {
private:
	static sf::Texture& getTexturaEstatica() {
		static sf::Texture tex;
		static bool cargada = false;
		if (!cargada) {
			if (!tex.loadFromFile("sprites/munieco.png")) {
				if (!tex.loadFromFile("sprites/realista_frente.png")) {
					std::cerr << "Falta imagen para el Munieco" << std::endl;
				}
			}
			cargada = true;
		}
		return tex;
	}
	
public:
		Munieco(float x, float y) : Personaje(x, y, 30, 999) { 
			cargarTextura();
		}
		
		std::string getTipo() override { return "PRACTICA"; }
		
		void cargarTextura() override {
			getSprite().setTexture(getTexturaEstatica());
			getSprite().setColor(sf::Color(139, 69, 19)); // Color madera constante
		}
		
		void actualizar() override { }
};

#endif
