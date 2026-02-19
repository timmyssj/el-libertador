#ifndef MUNIECO_H
#define MUNIECO_H

#include "Personaje.h"
#include <iostream>

class Munieco : public Personaje {
private:
	sf::Texture texturaMunieco;
	
public:
	// Delay de 999 porque el muñeco no camina
	Munieco(float x, float y) : Personaje(x, y, 30, 999) { 
		cargarTextura();
	}
	
	std::string getTipo() override { return "PRACTICA"; }
	
	void cargarTextura() override {
		// Intento 1: Cargar la imagen del sable o muñeco
		if (!texturaMunieco.loadFromFile("sprites/munieco.png")) {
			
			// PLAN B (Fallback): Si no hay sable, usamos un realista y lo pintamos de marrón
			if (texturaMunieco.loadFromFile("sprites/realista_frente.png")) {
				sprite.setColor(sf::Color(139, 69, 19)); // Código de color marrón madera
			} else {
				std::cerr << "Falta imagen para el Munieco" << std::endl;
			}
		}
		
		sprite.setTexture(texturaMunieco);
	}
	
	void actualizar() override {
		// Estático, espera pacientemente a ser destruido
	}
};

#endif
