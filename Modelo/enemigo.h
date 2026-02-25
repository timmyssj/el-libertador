#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Enemigo : public Personaje {
private:
	SanMartin* heroe; 
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	int cooldownAtaque;
	
public:
	Enemigo(float x, float y, SanMartin* h) : Personaje(x, y, 80, 5) {
		heroe = h;
		cooldownAtaque = 0;
		cargarTextura();
	}
	
	std::string getTipo() override { return "REALISTA"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "enemigo_derecho", 4);
		cargarSecuencia(animIzquierda, "enemigo_izquierdo", 4);
		cargarSecuencia(animArriba, "enemigo_atras", 4);
		
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/enemigo_frente_1.png")) {
			animAbajo.push_back(tFrente);
		} else if(tFrente.loadFromFile("sprites/enemigo_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		Entidad* objetivoMasCercano = nullptr;
		float distMinima = 9999.0f;
		
		// 1. RADAR: BUSCAR A SAN MARTÍN O A UN GRANADERO
		if (getMapaEntidades() != nullptr) {
			for (Entidad* e : *getMapaEntidades()) {
				if ((e->getTipo() == "PROCER" || e->getTipo() == "REALISTA_ALIADO" || e->getTipo() == "ALIADO") && e->estaVivo()) {
					float dx = e->getX() - getX();
					float dy = e->getY() - getY();
					float dist = std::sqrt(dx*dx + dy*dy);
					
					if (dist < distMinima) {
						distMinima = dist;
						objetivoMasCercano = e;
					}
				}
			}
		}
		
		// Plan de respaldo: Si falla el radar, buscar a San Martín por defecto
		if (!objetivoMasCercano && heroe && heroe->estaVivo()) {
			objetivoMasCercano = heroe;
			float dx = heroe->getX() - getX();
			float dy = heroe->getY() - getY();
			distMinima = std::sqrt(dx*dx + dy*dy);
		}
		
		// 2. ATACAR SI ESTÁ A RANGO
		if (objetivoMasCercano && distMinima <= 1.5f && cooldownAtaque == 0) {
			Personaje* victima = static_cast<Personaje*>(objetivoMasCercano);
			victima->recibirDanio(10.0f); // Daño de la bayoneta realista
			cooldownAtaque = 80; 
			setTimerAtaque(12);
			atacando = true;
			
			float dx = objetivoMasCercano->getX() - getX();
			float dy = objetivoMasCercano->getY() - getY();
			if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
			else setDireccion(dy > 0 ? ABAJO : ARRIBA);
		}
		
		// 3. PERSEGUIR (Con Sistema Anti-Atascos)
		if (!atacando && objetivoMasCercano) {
			if (distMinima <= 5.0f && distMinima > 1.2f) { // Agro: 12 casillas
				float dx = objetivoMasCercano->getX() - getX();
				float dy = objetivoMasCercano->getY() - getY();
				float movX = 0, movY = 0;
				
				if (std::abs(dx) > std::abs(dy)) movX = (dx > 0) ? 1 : -1;
				else movY = (dy > 0) ? 1 : -1;
				
				float oldX = getX();
				float oldY = getY();
				
				moverse(movX, movY);
				
				if (std::abs(getX() - oldX) < 0.01f && std::abs(getY() - oldY) < 0.01f && getCooldownMovimiento() == 0) {
					if (movX != 0) moverse(0, (dy > 0) ? 1 : -1); 
					else moverse((dx > 0) ? 1 : -1, 0); 
				}
			} else {
				resetearMovimiento();
			}
		} else {
			resetearMovimiento();
		}
		
		// 4. ANIMACIONES
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
