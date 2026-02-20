#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

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
		
		// FRENTE
		sf::Texture tFrente;
		if (tFrente.loadFromFile("sprites/san_martin_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		
		// ATR�S
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
			sprite.setOrigin(24.f, 24.f); // si us�s sprites 48x48
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
	
	void atacar(const std::vector<Entidad*>& entidadesEnElMapa) {
		bool golpeoAlgo = false;
		
		for (Entidad* e : entidadesEnElMapa) {
			if (e == this) continue;
			
			if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA") && e->estaVivo()) {
				
				float dx = e->getX() - this->x;
				float dy = e->getY() - this->y;
				float dist = std::sqrt(dx*dx + dy*dy);
				
				if (dist <= 2.2f) {
					Personaje* enemigo = static_cast<Personaje*>(e);
					enemigo->recibirDanio(15.0f);
					golpeoAlgo = true;
				}
			}
		}
		
		if (golpeoAlgo) {
			std::cout << "[SAN MARTIN] �Sablazo! Impacto al enemigo." << std::endl;
		}
	}
};

#endif
