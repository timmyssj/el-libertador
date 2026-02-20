#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>

class Enemigo : public Personaje {
private:
	SanMartin* objetivo;
	
	sf::Texture texDerecha;
	sf::Texture texIzquierda;
	sf::Texture texArriba;
	sf::Texture texAbajo;
	
	int cooldownAtaque;
	
public:
	
	Enemigo(float x, float y, SanMartin* heroe)
		: Personaje(x, y, 50, 15)
	{
		objetivo = heroe;
		cooldownAtaque = 0;
		cargarTextura();
	}
	
	std::string getTipo() override { return "REALISTA"; }
	
	void cargarTextura() override {
		
		texDerecha.loadFromFile("sprites/realista_derecho.png");
		texIzquierda.loadFromFile("sprites/realista_izquierdo.png");
		texArriba.loadFromFile("sprites/realista_atras.png");
		texAbajo.loadFromFile("sprites/realista_frente.png");
		
		sprite.setTexture(texAbajo); // inicial
		sprite.setOrigin(24.f, 24.f); // si son 48x48
	}
	
	void actualizar() override {
		
		if (cooldownAtaque > 0) cooldownAtaque--;
		
		if (objetivo && objetivo->estaVivo()) {
			
			float dx = objetivo->getX() - x;
			float dy = objetivo->getY() - y;
			float dist = std::sqrt(dx * dx + dy * dy);
			
			if (dist < 8.0f && dist > 1.0f) {
				
				if (std::abs(dx) > std::abs(dy)) {
					if (dx > 0) {
						moverse(1, 0);
						direccionActual = DERECHA;
					}
					else {
						moverse(-1, 0);
						direccionActual = IZQUIERDA;
					}
				}
				else {
					if (dy > 0) {
						moverse(0, 1);
						direccionActual = ABAJO;
					}
					else {
						moverse(0, -1);
						direccionActual = ARRIBA;
					}
				}
			}
			else {
				resetearMovimiento();
			}
		}
		
		// Solo cambia la textura según dirección
		switch (direccionActual) {
		case DERECHA:   sprite.setTexture(texDerecha); break;
		case IZQUIERDA: sprite.setTexture(texIzquierda); break;
		case ARRIBA:    sprite.setTexture(texArriba); break;
		case ABAJO:     sprite.setTexture(texAbajo); break;
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
