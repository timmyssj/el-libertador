#ifndef ALIADO_H
#define ALIADO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Aliado : public Personaje {
private:
	SanMartin* lider;
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	int cooldownAtaque;
	
public:
	Aliado(float x, float y, SanMartin* heroe) : Personaje(x, y, 100, 7) { 
		lider = heroe;
		cooldownAtaque = 0;
		cargarTextura();
	}
	
	std::string getTipo() override { return "REALISTA_ALIADO"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "realista_derecho", 4);
		cargarSecuencia(animIzquierda, "realista_izquierdo", 4);
		cargarSecuencia(animArriba, "realista_atras", 4);
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/realista_frente.png")) animAbajo.push_back(tFrente);
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		// --- AHORA USAMOS getMapaEntidades() ---
		if (getMapaEntidades() != nullptr && cooldownAtaque == 0) {
			for (Entidad* e : *getMapaEntidades()) {
				if (e->getTipo() == "FRANCES" && e->estaVivo()) {
					float dist = std::sqrt(std::pow(e->getX() - getX(), 2) + std::pow(e->getY() - getY(), 2));
					if (dist <= 2.2f) {
						Personaje* enemigo = static_cast<Personaje*>(e);
						enemigo->recibirDanio(15.0f); 
						cooldownAtaque = 60; 
						setTimerAtaque(12); 
						atacando = true;
						break; 
					}
				}
			}
		}
		
		if (!atacando && lider && lider->estaVivo()) {
			float dx = lider->getX() - this->getX();
			float dy = lider->getY() - this->getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist > 3.5f && dist < 20.0f) { 
				float movX = 0, movY = 0;
				if (std::abs(dx) > std::abs(dy)) movX = (dx > 0) ? 1 : -1;
				else movY = (dy > 0) ? 1 : -1;
				
				moverse(movX, movY);
				
				// --- AHORA USAMOS getCooldownMovimiento() ---
				if (getCooldownMovimiento() == 0) {
					if (movX != 0) moverse(0, (dy > 0) ? 1 : -1);
					else moverse((dx > 0) ? 1 : -1, 0);
				}
			} else {
				resetearMovimiento(); 
			}
		}
		
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			float desplazamiento = (getTimerAtaque() > 6) ? 0.4f : 0.0f; 
			switch (getDireccion()) {
			case DERECHA: setOffsetX(desplazamiento); setOffsetY(0); break;
			case IZQUIERDA: setOffsetX(-desplazamiento); setOffsetY(0); break;
			case ABAJO: setOffsetX(0); setOffsetY(desplazamiento); break;
			case ARRIBA: setOffsetX(0); setOffsetY(-desplazamiento); break;
			}
		} else {
			setOffsetX(0); setOffsetY(0);
			switch (getDireccion()) {
			case DERECHA: reproducirAnimacion(animDerecha); break;
			case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
			case ARRIBA: reproducirAnimacion(animArriba); break;
			case ABAJO: reproducirAnimacion(animAbajo); break;
			}
		}
	}
};

#endif
