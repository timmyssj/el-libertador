#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Enemigo : public Personaje {
private:
	SanMartin* heroe; 
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
	std::vector<sf::Texture> atkDerecha, atkIzquierda, atkArriba, atkAbajo;
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
		cargarSecuencia(animDerecha, "realista_derecho", 1);
		cargarSecuencia(animIzquierda, "realista_izquierdo", 1);
		cargarSecuencia(animArriba, "realista_atras", 1);
		cargarSecuencia(animAbajo, "realista_frente", 1);
		
		cargarSecuencia(atkDerecha, "realista_ataque_derecho", 1);
		cargarSecuencia(atkIzquierda, "realista_ataque_izquierdo", 1);
		cargarSecuencia(atkArriba, "realista_ataque_atras", 1);
		cargarSecuencia(atkAbajo, "realista_ataque_frente", 1);
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		Entidad* mejorObjetivo = nullptr;
		float mejorPuntaje = 9999.0f;
		float distFisicaAlMejor = 9999.0f;
		
		// --- SISTEMA DE PUNTUACIÓN DE COMBATE (El nuevo 1v1 orgánico) ---
		if (getMapaEntidades() != nullptr) {
			for (Entidad* e : *getMapaEntidades()) {
				if ((e->getTipo() == "PROCER" || e->getTipo() == "GRANADERO" || e->getTipo() == "ALIADO") && e->estaVivo()) {
					
					float dx = e->getX() - getX();
					float dy = e->getY() - getY();
					float distReal = std::sqrt(dx*dx + dy*dy);
					
					if (distReal <= 8.0f) { // Solo evalúa a los que están a 8 bloques o menos
						
						// Preguntamos: ¿Cuántos realistas están pegados a este objetivo?
						int colegasPeleando = 0;
						for (Entidad* comp : *getMapaEntidades()) {
							if (comp->getTipo() == "REALISTA" && comp != this && comp->estaVivo()) {
								float cdx = e->getX() - comp->getX();
								float cdy = e->getY() - comp->getY();
								if (std::sqrt(cdx*cdx + cdy*cdy) <= 1.8f) { // Si un colega está muy cerca de él
									colegasPeleando++;
								}
							}
						}
						
						// LA MAGIA TÁCTICA: Puntaje = Distancia + (Penalización por amontonamiento)
						// Si San Martín está a 2 pasos pero ya pelea con 1 realista, su puntaje es 2 + 10 = 12.
						// Si un Granadero libre está a 6 pasos, su puntaje es 6. El realista elegirá al Granadero (6 < 12).
						float puntaje = distReal + (colegasPeleando * 10.0f);
						
						if (puntaje < mejorPuntaje) {
							mejorPuntaje = puntaje;
							mejorObjetivo = e;
							distFisicaAlMejor = distReal;
						}
					}
				}
			}
		}
		
		// Respaldo: si no vio a nadie, va por San Martín por defecto
		if (!mejorObjetivo && heroe && heroe->estaVivo()) {
			mejorObjetivo = heroe;
			float dx = heroe->getX() - getX();
			float dy = heroe->getY() - getY();
			distFisicaAlMejor = std::sqrt(dx*dx + dy*dy);
		}
		
		// --- ACTUAR SOBRE EL MEJOR OBJETIVO DEL MOMENTO ---
		if (mejorObjetivo && distFisicaAlMejor <= 8.0f) {
			float dx = mejorObjetivo->getX() - getX();
			float dy = mejorObjetivo->getY() - getY();
			
			// A. ATACAR
			if (distFisicaAlMejor <= 1.5f && cooldownAtaque == 0) {
				Personaje* victima = static_cast<Personaje*>(mejorObjetivo);
				victima->recibirDanio(10.0f); 
				cooldownAtaque = 80; 
				setTimerAtaque(20); 
				atacando = true;
				registrarAtaque();
				
				if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
				else setDireccion(dy > 0 ? ABAJO : ARRIBA);
			}
			
			// B. PERSEGUIR
			if (!atacando) {
				if (distFisicaAlMejor > 1.2f) { 
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
			}
		} else {
			resetearMovimiento();
		}
		
		// --- ANIMACIONES ---
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			setOffsetX(0); setOffsetY(0); 
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
