#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h" 
#include <cmath>
#include <vector>

class Enemigo : public Personaje {
private:
	SanMartin* objetivo;
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	
	int cooldownAtaque;
	
public:
	Enemigo(float x, float y, SanMartin* heroe) 
		: Personaje(x, y, 50, 15) {
		objetivo = heroe;
		cooldownAtaque = 0;
		cargarTextura(); 
	}
	
	std::string getTipo() override { return "REALISTA"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) vector.push_back(t);
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "realista_derecho", 1);
		cargarSecuencia(animIzquierda, "realista_izquierdo", 1);
		cargarSecuencia(animArriba, "realista_atras", 1);
		
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/realista_frente_1.png")) { 
			animAbajo.push_back(tFrente);
		}
		
		// CORRECCIÓN AQUÍ: Usamos getSprite()
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		
		if (objetivo && objetivo->estaVivo()) {
			// CORRECCIÓN AQUÍ: Usamos getX() y getY()
			float dx = objetivo->getX() - this->getX();
			float dy = objetivo->getY() - this->getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist < 8.0f && dist > 1.0f) { 
				if (std::abs(dx) > std::abs(dy)) {
					if (dx > 0) moverse(1, 0); else moverse(-1, 0);
				} else {
					if (dy > 0) moverse(0, 1); else moverse(0, -1);
				}
			} else {
				resetearMovimiento(); 
			}
		}
		
		// CORRECCIÓN AQUÍ: Usamos getDireccion()
		switch (getDireccion()) {
		case DERECHA:   reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA:    reproducirAnimacion(animArriba); break;
		case ABAJO:     reproducirAnimacion(animAbajo); break;
		}
	}
	
	bool intentarAtacar() {
		if (cooldownAtaque == 0) {
			cooldownAtaque = 60; 
			return true;
		}
		return false;
	}
};

#endif
