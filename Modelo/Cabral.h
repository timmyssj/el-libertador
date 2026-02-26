#ifndef CABRAL_H
#define CABRAL_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>

class Cabral : public Personaje {
private:
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
	SanMartin* lider;
	
public:
	Cabral(float x, float y, SanMartin* sanMartin) : Personaje(x, y, 100, 8) {
		lider = sanMartin;
		cargarTextura();
	}
	
	std::string getTipo() override { return "GRANADERO"; } 
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase) {
		sf::Texture t;
		if (t.loadFromFile("sprites/" + nombreBase + ".png")) {
			vector.push_back(t);
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animAbajo, "cabral_frente");
		cargarSecuencia(animArriba, "cabral_atras");
		cargarSecuencia(animDerecha, "cabral_derecho");
		cargarSecuencia(animIzquierda, "cabral_izquierdo");
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (!estaVivo()) return;
		
		if (lider && lider->estaVivo()) {
			float destX = lider->getX() + 1.2f; 
			float destY = lider->getY(); 
			
			float dx = destX - getX();
			float dy = destY - getY();
			
			bool seMovio = false;
			if (std::abs(dx) > 1.0f || std::abs(dy) > 1.0f) {
				if (std::abs(dx) > std::abs(dy)) moverse((dx > 0) ? 1 : -1, 0);
				else moverse(0, (dy > 0) ? 1 : -1);
				seMovio = true;
			} else {
				resetearMovimiento();
			}
			
			// --- ANIMACIONES SEGÚN LA DIRECCIÓN ---
			if (seMovio) {
				switch (getDireccion()) {
				case DERECHA:   reproducirAnimacion(animDerecha); break;
				case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
				case ARRIBA:    reproducirAnimacion(animArriba); break;
				case ABAJO:     reproducirAnimacion(animAbajo); break;
				}
			} else {
				// Si está quieto, muestra el sprite correcto según hacia donde mire
				switch (getDireccion()) {
				case DERECHA:   if(!animDerecha.empty()) getSprite().setTexture(animDerecha[0], true); break;
				case IZQUIERDA: if(!animIzquierda.empty()) getSprite().setTexture(animIzquierda[0], true); break;
				case ARRIBA:    if(!animArriba.empty()) getSprite().setTexture(animArriba[0], true); break;
				case ABAJO:     if(!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true); break;
				}
			}
		}
	}
	
	void darLaVida() { recibirDanio(999.0f); }
};

#endif
