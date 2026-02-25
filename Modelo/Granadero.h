#ifndef GRANADERO_H
#define GRANADERO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Granadero : public Personaje {
private:
	SanMartin* lider;
	int flanco; // --- NUEVO: -2 (Lejos Izq), -1 (Izq), 1 (Der), 2 (Lejos Der)
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	int cooldownAtaque;
	
public:
	// Bajamos un poquito la velocidad (de 6 a 4) para que marchen con más disciplina
	Granadero(float x, float y, SanMartin* heroe, int posicionFlanco) : Personaje(x, y, 120, 4) { 
		lider = heroe;
		flanco = posicionFlanco; 
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
		cargarSecuencia(animDerecha, "granadero_derecho", 4);
		cargarSecuencia(animIzquierda, "granadero_izquierdo", 4);
		cargarSecuencia(animArriba, "granadero_atras", 4);
		
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/granadero_frente_1.png")) animAbajo.push_back(tFrente);
		else if(tFrente.loadFromFile("sprites/granadero_frente.png")) animAbajo.push_back(tFrente);
		
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
		
		if (objetivoMasCercano && distMinima <= 2.2f && cooldownAtaque == 0) {
			Personaje* enemigo = static_cast<Personaje*>(objetivoMasCercano);
			enemigo->recibirDanio(20.0f);
			cooldownAtaque = 60; 
			setTimerAtaque(12); 
			atacando = true;
			
			float dx = objetivoMasCercano->getX() - getX();
			float dy = objetivoMasCercano->getY() - getY();
			if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
			else setDireccion(dy > 0 ? ABAJO : ARRIBA);
		}
		
		if (!atacando) {
			float destX = getX();
			float destY = getY();
			bool debeMoverse = false;
			
			// PRIORIDAD A: Autodefensa (Solo rompen formación si el enemigo está a 3 casillas o menos)
			if (objetivoMasCercano && distMinima <= 3.0f) {
				destX = objetivoMasCercano->getX();
				destY = objetivoMasCercano->getY();
				debeMoverse = (distMinima > 1.2f); 
			} 
			// PRIORIDAD B: Mantener la formación de Pinza
			else if (lider && lider->estaVivo()) {
				// El destino ideal es "X casillas al lado de San Martín" y "1 casilla atrás"
				float posIdealX = lider->getX() + (flanco * 2.0f); 
				float posIdealY = lider->getY() + 1.0f; 
				
				float dx = posIdealX - getX();
				float dy = posIdealY - getY();
				float distAFormacion = std::sqrt(dx*dx + dy*dy);
				
				// Si están lejos de su puesto en la formación, marchan hacia allí
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
		
		// 4. ANIMACIONES (Igual que antes)
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
