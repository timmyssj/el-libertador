#ifndef GRANADERO_H
#define GRANADERO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Granadero : public Personaje {
private:
	SanMartin* lider;
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	int cooldownAtaque;
	
public:
	// Le damos 120 de vida y un poco más de velocidad (6) por ser tropas de élite
	Granadero(float x, float y, SanMartin* heroe) : Personaje(x, y, 120, 6) { 
		lider = heroe;
		cooldownAtaque = 0;
		cargarTextura();
	}
	
	std::string getTipo() override { return "REALISTA_ALIADO"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			// Busca sprites llamados granadero_derecho_1.png, etc.
			if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "granadero_derecho", 4);
		cargarSecuencia(animIzquierda, "granadero_izquierdo", 4);
		cargarSecuencia(animArriba, "granadero_atras", 4);
		
		sf::Texture tFrente;
		// Intenta cargar la textura de frente
		if(tFrente.loadFromFile("sprites/granadero_frente_1.png")) {
			animAbajo.push_back(tFrente);
		} else if(tFrente.loadFromFile("sprites/granadero_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		
		// --- CORRECCIÓN: Usar getSprite() en lugar de sprite directo ---
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		// 1. IA DE COMBATE
		if (getMapaEntidades() != nullptr && cooldownAtaque == 0) {
			for (Entidad* e : *getMapaEntidades()) {
				if ((e->getTipo() == "FRANCES" || e->getTipo() == "REALISTA") && e->estaVivo()) {
					
					// --- CORRECCIÓN: Usar getX() y getY() en lugar de x e y ---
					float dx = e->getX() - getX();
					float dy = e->getY() - getY();
					float dist = std::sqrt(dx*dx + dy*dy);
					
					if (dist <= 2.2f) {
						Personaje* enemigo = static_cast<Personaje*>(e);
						enemigo->recibirDanio(20.0f); // Pegan un poco más fuerte
						cooldownAtaque = 60; 
						setTimerAtaque(12); 
						atacando = true;
						break; 
					}
				}
			}
		}
		
		// 2. IA DE MOVIMIENTO Y ESQUIVE
		if (!atacando && lider && lider->estaVivo()) {
			float dx = lider->getX() - getX();
			float dy = lider->getY() - getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist > 3.5f && dist < 20.0f) { 
				float movX = 0, movY = 0;
				if (std::abs(dx) > std::abs(dy)) movX = (dx > 0) ? 1 : -1;
				else movY = (dy > 0) ? 1 : -1;
				
				moverse(movX, movY);
				
				if (getCooldownMovimiento() == 0) {
					if (movX != 0) moverse(0, (dy > 0) ? 1 : -1);
					else moverse((dx > 0) ? 1 : -1, 0);
				}
			} else {
				resetearMovimiento(); 
			}
		}
		
		// 3. ANIMACIONES
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			float desplazamiento = (getTimerAtaque() > 6) ? 0.4f : 0.0f; 
			
			// --- CORRECCIÓN: Usar getDireccion() ---
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
