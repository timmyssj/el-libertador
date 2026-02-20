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
	SanMartin(float x, float y) : Personaje(x, y, 100, 6) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	
	
	void cargarTextura() override {
		
		animDerecha.clear();
		animIzquierda.clear();
		animArriba.clear();
		animAbajo.clear();
		
		// ABAJO (frente)
		sf::Texture tFrente;
		if (tFrente.loadFromFile("sprites/san_martin_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		
		// ARRIBA (atrás)
		sf::Texture tAtras;
		if (tAtras.loadFromFile("sprites/san_martin_atras.png")) {
			animArriba.push_back(tAtras);
		}
		
		// DERECHA
		sf::Texture tDerecha;
		if (tDerecha.loadFromFile("sprites/san_martin_derecho.png")) {
			animDerecha.push_back(tDerecha);
		}
		
		// IZQUIERDA
		sf::Texture tIzquierda;
		if (tIzquierda.loadFromFile("sprites/san_martin_izquierdo.png")) {
			animIzquierda.push_back(tIzquierda);
		}
		
		// Textura inicial
		if (!animAbajo.empty()) {
			sprite.setTexture(animAbajo[0]);
			sprite.setOrigin(24.f, 24.f); // si tus sprites son 48x48
		}
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
