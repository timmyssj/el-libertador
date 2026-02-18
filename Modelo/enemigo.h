#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h" 
#include <cmath>
#include <iostream>
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
	// VELOCIDAD AUMENTADA: 0.10f
	Enemigo(float x, float y, SanMartin* heroe) 
		: Personaje(x, y, 50, 0.10f) {
		objetivo = heroe;
		cooldownAtaque = 0;
		cargarTextura(); 
	}
	
	std::string getTipo() override { return "REALISTA"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			// RUTA CORREGIDA: sprites/
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) vector.push_back(t);
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "realista_derecha", 4);
		cargarSecuencia(animIzquierda, "realista_isquierda", 4); // Tu archivo dice 'isquierda'
		cargarSecuencia(animArriba, "realista_atras", 4);
		
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/realista_frente.png")) { 
			animAbajo.push_back(tFrente);
		}
		
		if (!animAbajo.empty()) sprite.setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		
		if (objetivo && objetivo->estaVivo()) {
			float dx = objetivo->getX() - x;
			float dy = objetivo->getY() - y;
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist < 8.0f && dist > 0.8f) { // Aumenté un poco la visión también
				moverse(dx/dist, dy/dist);
			} else {
				resetearMovimiento(); 
			}
		}
		
		switch (direccionActual) {
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
