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
	// Delay de 15 frames (más lento que el jugador)
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
		// NOMBRES CORREGIDOS SEGÚN TU LISTA
		cargarSecuencia(animDerecha, "realista_derecho", 4);
		cargarSecuencia(animIzquierda, "realista_izquierdo", 4);
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
			
			// IA SIMPLIFICADA PARA MOVERSE POR BLOQUES
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
}; // <--- ¡AQUÍ ESTABA EL ERROR! Faltaba esta llave y punto y coma.

#endif
