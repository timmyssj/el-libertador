#ifndef FRANCES_H
#define FRANCES_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Frances : public Personaje {
private:
	SanMartin* heroe; 
	int cooldownAtaque;
	
	// --- OPTIMIZACIÓN: CACHÉ COMPARTIDA DE TEXTURAS ---
	struct Texturas {
		std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
		std::vector<sf::Texture> atkDerecha, atkIzquierda, atkArriba, atkAbajo;
		bool cargadas = false;
	};
	static Texturas& getTexturas() { static Texturas t; return t; }
	
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
			Texturas& t = getTexturas();
			if (!t.cargadas) {
				cargarSecuencia(t.animDerecha, "frances_derecho", 1);
				cargarSecuencia(t.animIzquierda, "frances_izquierdo", 1);
				cargarSecuencia(t.animArriba, "frances_atras", 1);
				cargarSecuencia(t.animAbajo, "frances_frente", 1);
				cargarSecuencia(t.atkDerecha, "frances_ataque_derecho", 1);
				cargarSecuencia(t.atkIzquierda, "frances_ataque_izquierdo", 1);
				cargarSecuencia(t.atkArriba, "frances_ataque_atras", 1);
				cargarSecuencia(t.atkAbajo, "frances_ataque_frente", 1);
				t.cargadas = true;
			}
			if (!t.animAbajo.empty()) getSprite().setTexture(t.animAbajo[0], true);
		}
		
		void actualizar() override {
			if (cooldownAtaque > 0) cooldownAtaque--;
			bool atacando = false;
			
			Entidad* mejorObjetivo = nullptr;
			float mejorPuntaje = 9999.0f;
			float distFisicaAlMejor = 9999.0f;
			
			if (getMapaEntidades() != nullptr) {
				for (Entidad* e : *getMapaEntidades()) {
					if ((e->getTipo() == "PROCER" || e->getTipo() == "ALIADO") && e->estaVivo()) {
						float dx = e->getX() - getX(); float dy = e->getY() - getY();
						float distReal = std::sqrt(dx*dx + dy*dy);
						if (distReal <= 8.0f) { 
							int colegasPeleando = 0;
							for (Entidad* comp : *getMapaEntidades()) {
								if (comp->getTipo() == "FRANCES" && comp != this && comp->estaVivo()) {
									float cdx = e->getX() - comp->getX(); float cdy = e->getY() - comp->getY();
									if (std::sqrt(cdx*cdx + cdy*cdy) <= 1.8f) colegasPeleando++;
								}
							}
							float puntaje = distReal + (colegasPeleando * 10.0f);
							if (puntaje < mejorPuntaje) { mejorPuntaje = puntaje; mejorObjetivo = e; distFisicaAlMejor = distReal; }
						}
					}
				}
			}
			
			if (!mejorObjetivo && heroe && heroe->estaVivo()) {
				mejorObjetivo = heroe;
				distFisicaAlMejor = std::sqrt(std::pow(heroe->getX() - getX(), 2) + std::pow(heroe->getY() - getY(), 2));
			}
			
			if (mejorObjetivo && distFisicaAlMejor <= 8.0f) {
				float dx = mejorObjetivo->getX() - getX(); float dy = mejorObjetivo->getY() - getY();
				if (distFisicaAlMejor <= 1.5f && cooldownAtaque == 0) {
					static_cast<Personaje*>(mejorObjetivo)->recibirDanio(10.0f); 
					cooldownAtaque = 80; setTimerAtaque(20); atacando = true; registrarAtaque();
					if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
					else setDireccion(dy > 0 ? ABAJO : ARRIBA);
				}
				if (!atacando && distFisicaAlMejor > 1.2f) { 
					float movX = 0, movY = 0;
					if (std::abs(dx) > std::abs(dy)) movX = (dx > 0) ? 1 : -1;
					else movY = (dy > 0) ? 1 : -1;
					float oldX = getX(); float oldY = getY();
					moverse(movX, movY);
					if (std::abs(getX() - oldX) < 0.01f && std::abs(getY() - oldY) < 0.01f && getCooldownMovimiento() == 0) {
						if (movX != 0) moverse(0, (dy > 0) ? 1 : -1); else moverse((dx > 0) ? 1 : -1, 0); 
					}
				} else if (!atacando) resetearMovimiento();
			} else resetearMovimiento();
			
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
