#ifndef GRANADERO_H
#define GRANADERO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Granadero : public Personaje {
private:
	SanMartin* lider;
	int flanco; 
	int cooldownAtaque;
	
	struct Texturas {
		std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
		std::vector<sf::Texture> atkDerecha, atkIzquierda, atkArriba, atkAbajo;
		bool cargadas = false;
	};
	static Texturas& getTexturas() { static Texturas t; return t; }
	
public:
		Granadero(float x, float y, SanMartin* heroe, int posicionFlanco) : Personaje(x, y, 120, 4) { 
			lider = heroe; flanco = posicionFlanco; cooldownAtaque = 0; cargarTextura();
		}
		
		std::string getTipo() override { return "GRANADERO"; }
		
		void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
			for (int i = 1; i <= cantidad; i++) {
				sf::Texture t;
				if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) vector.push_back(t);
			}
		}
		
		void cargarTextura() override {
			Texturas& t = getTexturas();
			if (!t.cargadas) {
				cargarSecuencia(t.animDerecha, "granadero_derecho", 1);
				cargarSecuencia(t.animIzquierda, "granadero_izquierdo", 1);
				cargarSecuencia(t.animArriba, "granadero_atras", 1);
				cargarSecuencia(t.animAbajo, "granadero_frente", 1);
				cargarSecuencia(t.atkDerecha, "granadero_ataque_derecho", 1);
				cargarSecuencia(t.atkIzquierda, "granadero_ataque_izquierdo", 1);
				cargarSecuencia(t.atkArriba, "granadero_ataque_atras", 1);
				cargarSecuencia(t.atkAbajo, "granadero_ataque_frente", 1);
				t.cargadas = true;
			}
			if (!t.animAbajo.empty()) getSprite().setTexture(t.animAbajo[0], true);
		}
		
		void actualizar() override {
			if (cooldownAtaque > 0) cooldownAtaque--;
			bool atacando = false;
			
			Entidad* objetivoMasCercano = nullptr;
			float distMinima = 9999.0f;
			
			if (getMapaEntidades() != nullptr) {
				for (Entidad* e : *getMapaEntidades()) {
					if ((e->getTipo() == "FRANCES" || e->getTipo() == "REALISTA") && e->estaVivo()) {
						float dx = e->getX() - getX(); float dy = e->getY() - getY();
						float dist = std::sqrt(dx*dx + dy*dy);
						if (dist < distMinima) { distMinima = dist; objetivoMasCercano = e; }
					}
				}
			}
			
			if (objetivoMasCercano && distMinima <= 1.5f && cooldownAtaque == 0) {
				static_cast<Personaje*>(objetivoMasCercano)->recibirDanio(20.0f);
				cooldownAtaque = 60; setTimerAtaque(20); atacando = true; registrarAtaque();
				float dx = objetivoMasCercano->getX() - getX(); float dy = objetivoMasCercano->getY() - getY();
				if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
				else setDireccion(dy > 0 ? ABAJO : ARRIBA);
			}
			
			if (!atacando) {
				float destX = getX(); float destY = getY(); bool debeMoverse = false;
				if (lider && lider->estaVivo()) {
					float posIdealX = lider->getX() + (flanco * 1.5f); 
					float posIdealY = lider->getY() + std::abs(flanco); 
					float dx = posIdealX - getX(); float dy = posIdealY - getY();
					if (std::sqrt(dx*dx + dy*dy) > 1.0f) { destX = posIdealX; destY = posIdealY; debeMoverse = true; }
				}
				if (debeMoverse) {
					float dx = destX - getX(); float dy = destY - getY();
					float movX = 0, movY = 0;
					if (std::abs(dx) > std::abs(dy)) movX = (dx > 0) ? 1 : -1;
					else movY = (dy > 0) ? 1 : -1;
					float oldX = getX(); float oldY = getY();
					moverse(movX, movY); 
					if (std::abs(getX() - oldX) < 0.01f && std::abs(getY() - oldY) < 0.01f && getCooldownMovimiento() == 0) {
						if (movX != 0) moverse(0, (dy > 0) ? 1 : -1); else moverse((dx > 0) ? 1 : -1, 0); 
					}
				} else resetearMovimiento(); 
			}
			
			Texturas& t = getTexturas();
			if (getTimerAtaque() > 0) {
				setTimerAtaque(getTimerAtaque() - 1); setOffsetX(0); setOffsetY(0); 
				switch (getDireccion()) {
				case DERECHA: reproducirAnimacion(t.atkDerecha.empty() ? t.animDerecha : t.atkDerecha); break;
				case IZQUIERDA: reproducirAnimacion(t.atkIzquierda.empty() ? t.animIzquierda : t.atkIzquierda); break;
				case ABAJO: reproducirAnimacion(t.atkAbajo.empty() ? t.animAbajo : t.atkAbajo); break;
				case ARRIBA: reproducirAnimacion(t.atkArriba.empty() ? t.animArriba : t.atkArriba); break;
				}
			} else {
				setOffsetX(0); setOffsetY(0);
				switch (getDireccion()) {
				case DERECHA: reproducirAnimacion(t.animDerecha); break;
				case IZQUIERDA: reproducirAnimacion(t.animIzquierda); break;
				case ARRIBA: reproducirAnimacion(t.animArriba); break;
				case ABAJO: reproducirAnimacion(t.animAbajo); break;
				}
			}
		}
};

#endif
