#ifndef FRANCES_H
#define FRANCES_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Frances : public Personaje {
private:
	SanMartin* heroe; 
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
	// --- NUEVO: Vectores de Ataque ---
	std::vector<sf::Texture> atkDerecha, atkIzquierda, atkArriba, atkAbajo;
	int cooldownAtaque;
	
public:
	Frances(float x, float y, SanMartin* h) : Personaje(x, y, 70, 5) {
		heroe = h;
		cooldownAtaque = 0;
		cargarTextura();
	}
	
	std::string getTipo() override { return "FRANCES"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		// Caminata
		cargarSecuencia(animDerecha, "frances_derecho", 1);
		cargarSecuencia(animIzquierda, "frances_izquierdo", 1);
		cargarSecuencia(animArriba, "frances_atras", 1);
		cargarSecuencia(animAbajo, "frances_frente", 1);
		
		// Ataques
		cargarSecuencia(atkDerecha, "frances_ataque_derecho", 1);
		cargarSecuencia(atkIzquierda, "frances_ataque_izquierdo", 1);
		cargarSecuencia(atkArriba, "frances_ataque_atras", 1);
		cargarSecuencia(atkAbajo, "frances_ataque_frente", 1);
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		Entidad* objetivoMasCercano = nullptr;
		float distMinima = 9999.0f;
		
		if (getMapaEntidades() != nullptr) {
			for (Entidad* e : *getMapaEntidades()) {
				if ((e->getTipo() == "PROCER" || e->getTipo() == "ALIADO") && e->estaVivo()) {
					float dx = e->getX() - getX();
					float dy = e->getY() - getY();
					float dist = std::sqrt(dx*dx + dy*dy);
					if (dist < distMinima) { distMinima = dist; objetivoMasCercano = e; }
				}
			}
		}
		
		if (objetivoMasCercano && distMinima <= 1.5f && cooldownAtaque == 0) {
			Personaje* victima = static_cast<Personaje*>(objetivoMasCercano);
			victima->recibirDanio(10.0f); 
			cooldownAtaque = 80; 
			setTimerAtaque(20); // Animación extendida
			atacando = true;
			
			float dx = objetivoMasCercano->getX() - getX();
			float dy = objetivoMasCercano->getY() - getY();
			if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
			else setDireccion(dy > 0 ? ABAJO : ARRIBA);
		}
		
		if (!atacando && objetivoMasCercano) {
			if (distMinima <= 8.0f && distMinima > 1.2f) { 
				float dx = objetivoMasCercano->getX() - getX();
				float dy = objetivoMasCercano->getY() - getY();
				float movX = 0, movY = 0;
				
				if (std::abs(dx) > std::abs(dy)) movX = (dx > 0) ? 1 : -1;
				else movY = (dy > 0) ? 1 : -1;
				
				float oldX = getX(); float oldY = getY();
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
		
		// --- ANIMACIONES ---
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			setOffsetX(0); 
			setOffsetY(0); 
			switch (getDireccion()) {
			case DERECHA: reproducirAnimacion(atkDerecha.empty() ? animDerecha : atkDerecha); break;
			case IZQUIERDA: reproducirAnimacion(atkIzquierda.empty() ? animIzquierda : atkIzquierda); break;
			case ABAJO: reproducirAnimacion(atkAbajo.empty() ? animAbajo : atkAbajo); break;
			case ARRIBA: reproducirAnimacion(atkArriba.empty() ? animArriba : atkArriba); break;
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
