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
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
	// --- NUEVO: Vectores de Ataque ---
	std::vector<sf::Texture> atkDerecha, atkIzquierda, atkArriba, atkAbajo;
	int cooldownAtaque;
	
public:
	Granadero(float x, float y, SanMartin* heroe, int posicionFlanco) : Personaje(x, y, 120, 4) { 
		lider = heroe;
		flanco = posicionFlanco; 
		cooldownAtaque = 0;
		cargarTextura();
	}
	
	std::string getTipo() override { return "GRANADERO"; }
	
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
		cargarSecuencia(animDerecha, "granadero_derecho", 1);
		cargarSecuencia(animIzquierda, "granadero_izquierdo", 1);
		cargarSecuencia(animArriba, "granadero_atras", 1);
		cargarSecuencia(animAbajo, "granadero_frente", 1);
		
		// Ataques
		cargarSecuencia(atkDerecha, "granadero_ataque_derecho", 1);
		cargarSecuencia(atkIzquierda, "granadero_ataque_izquierdo", 1);
		cargarSecuencia(atkArriba, "granadero_ataque_atras", 1);
		cargarSecuencia(atkAbajo, "granadero_ataque_frente", 1);
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		Entidad* objetivoMasCercano = nullptr;
		float distMinima = 9999.0f;
		
		if (getMapaEntidades() != nullptr) {
			for (Entidad* e : *getMapaEntidades()) {
				if ((e->getTipo() == "FRANCES" || e->getTipo() == "REALISTA") && e->estaVivo()) {
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
		
		// 2. ATACAR SOLO EN DEFENSA PROPIA (Reducido a 1.5f)
		if (objetivoMasCercano && distMinima <= 1.5f && cooldownAtaque == 0) {
			Personaje* enemigo = static_cast<Personaje*>(objetivoMasCercano);
			enemigo->recibirDanio(20.0f);
			cooldownAtaque = 60; 
			setTimerAtaque(20); 
			atacando = true;
			
			float dx = objetivoMasCercano->getX() - getX();
			float dy = objetivoMasCercano->getY() - getY();
			if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
			else setDireccion(dy > 0 ? ABAJO : ARRIBA);
		}
		
		// 3. IA DE ESCOLTA (No persiguen, solo acompañan)
		if (!atacando) {
			float destX = getX();
			float destY = getY();
			bool debeMoverse = false;
			
			if (lider && lider->estaVivo()) {
				// Formación de V invertida detrás de San Martín
				// Los de flanco -1 y 1 van un paso atrás. Los de -2 y 2 van dos pasos atrás.
				float posIdealX = lider->getX() + (flanco * 1.5f); 
				float posIdealY = lider->getY() + std::abs(flanco); // Siempre Y positivo (Atrás)
				
				float dx = posIdealX - getX();
				float dy = posIdealY - getY();
				float distAFormacion = std::sqrt(dx*dx + dy*dy);
				
				// Si se alejan de su puesto, lo corrigen
				if (distAFormacion > 1.0f) {
					destX = posIdealX;
					destY = posIdealY;
					debeMoverse = true;
				}
			}
			
			if (debeMoverse) {
				float dx = destX - getX();
				float dy = destY - getY();
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
