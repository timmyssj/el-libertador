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
	
	std::vector<sf::Texture> animAtaqueDerecha;
	std::vector<sf::Texture> animAtaqueIzquierda;
	
public:
	SanMartin(float x, float y) : Personaje(x, y, 100, 6) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		// --- ANIMACIONES DE CAMINAR ---
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/san_martin_frente_1.png")) animAbajo.push_back(tFrente);
		
		cargarSecuencia(animArriba, "san_martin_atras", 1);
		
		sf::Texture tDer;
		if(tDer.loadFromFile("sprites/san_martin_derecho_1.png")) animDerecha.push_back(tDer);
		
		sf::Texture tIzq;
		if(tIzq.loadFromFile("sprites/san_martin_izquierdo_1.png")) animIzquierda.push_back(tIzq);
		
		// --- SPRITES DE ATAQUE ---
		sf::Texture tAtaqueDer;
		if(tAtaqueDer.loadFromFile("sprites/san_martin_ataque_derecho_2.png")) animAtaqueDerecha.push_back(tAtaqueDer);
		
		sf::Texture tAtaqueIzq;
		if(tAtaqueIzq.loadFromFile("sprites/san_martin_ataque_izquierdo_2.png")) animAtaqueIzquierda.push_back(tAtaqueIzq);
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			
			// 1. LA ESTOCADA: Ahora dura 20 frames (más visible)
			float desplazamiento = (getTimerAtaque() > 10) ? 0.4f : 0.0f; 
			switch (getDireccion()) {
			case DERECHA:   setOffsetX(desplazamiento); setOffsetY(0); break;
			case IZQUIERDA: setOffsetX(-desplazamiento); setOffsetY(0); break;
			case ABAJO:     setOffsetX(0); setOffsetY(desplazamiento); break;
			case ARRIBA:    setOffsetX(0); setOffsetY(-desplazamiento); break;
			}
			
			// 2. EL SPRITE DE ATAQUE
			std::vector<sf::Texture>* animActiva = nullptr;
			switch (getDireccion()) {
			case DERECHA:   animActiva = &animAtaqueDerecha; break;
			case IZQUIERDA: animActiva = &animAtaqueIzquierda; break;
			// PLAN B: Si no tienes ataque arriba/abajo, usa el sprite normal para que no desaparezca
			case ARRIBA:    animActiva = &animArriba; break; 
			case ABAJO:     animActiva = &animAbajo; break;
			}
			
			if (animActiva != nullptr && !animActiva->empty()) {
				// Al tener 1 sola imagen de ataque, forzamos el índice [0] para evitar cálculos raros
				getSprite().setTexture((*animActiva)[0], true);
			}
			
		} else {
			// COMPORTAMIENTO NORMAL
			setOffsetX(0);
			setOffsetY(0);
			
			switch (getDireccion()) {
			case DERECHA:   reproducirAnimacion(animDerecha); break;
			case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
			case ARRIBA:    reproducirAnimacion(animArriba); break;
			case ABAJO:     reproducirAnimacion(animAbajo); break;
			}
		}
	}
	
	void atacar(const std::vector<Entidad*>& entidadesEnElMapa) {
		// --- SOLUCIÓN METRALLETA: Si ya está atacando, ignorar el botón ---
		if (getTimerAtaque() > 0) return;
		
		// Aumentamos a 20 frames (1/3 de segundo) para que se vea el impacto
		setTimerAtaque(20); 
		
		bool golpeoAlgo = false;
		
		for (Entidad* e : entidadesEnElMapa) {
			if (e == this) continue; 
			if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA") && e->estaVivo()) {
				float dx = e->getX() - this->getX(); 
				float dy = e->getY() - this->getY();
				float dist = std::sqrt(dx*dx + dy*dy);
				
				if (dist <= 2.2f) { 
					Personaje* enemigo = static_cast<Personaje*>(e);
					enemigo->recibirDanio(15.0f); 
					golpeoAlgo = true;
				}
			}
		}
		
		if (golpeoAlgo) {
			std::cout << "[SAN MARTIN] ¡Sablazo! Impacto al enemigo." << std::endl;
		}
	}
};

#endif
