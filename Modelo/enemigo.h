#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>

class Enemigo : public Personaje {
private:
	SanMartin* heroe; 
	// Vectores originales de caminata
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
	
	// --- NUEVO: Vectores de Ataque ---
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
		// --- Cargar Caminata ---
		cargarSecuencia(animDerecha, "realista_derecho", 1);
		cargarSecuencia(animIzquierda, "realista_izquierdo", 1);
		cargarSecuencia(animArriba, "realista_atras", 1);
		cargarSecuencia(animAbajo, "realista_frente", 1);
		
		// --- NUEVO: Cargar Ataques ---
		cargarSecuencia(atkDerecha, "realista_ataque_derecho", 1);
		cargarSecuencia(atkIzquierda, "realista_ataque_izquierdo", 1);
		cargarSecuencia(atkArriba, "realista_ataque_atras", 1);
		cargarSecuencia(atkAbajo, "realista_ataque_frente", 1);
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		bool atacando = false;
		
		Entidad* objetivoMasCercano = nullptr;
		float distMinima = 9999.0f;
		
		if (getMapaEntidades() != nullptr) {
			for (Entidad* e : *getMapaEntidades()) {
				if ((e->getTipo() == "PROCER" || e->getTipo() == "GRANADERO" || e->getTipo() == "ALIADO") && e->estaVivo()) {
					float dx = e->getX() - getX();
					float dy = e->getY() - getY();
					float dist = std::sqrt(dx*dx + dy*dy);
					if (dist < distMinima) { distMinima = dist; objetivoMasCercano = e; }
				}
			}
		}
		
		if (!objetivoMasCercano && heroe && heroe->estaVivo()) {
			objetivoMasCercano = heroe;
			float dx = heroe->getX() - getX();
			float dy = heroe->getY() - getY();
			distMinima = std::sqrt(dx*dx + dy*dy);
		}
		
		if (objetivoMasCercano && distMinima <= 1.5f && cooldownAtaque == 0) {
			Personaje* victima = static_cast<Personaje*>(objetivoMasCercano);
			victima->recibirDanio(10.0f); 
			cooldownAtaque = 80; 
			
			// --- NUEVO: Dar más tiempo para reproducir la animación completa ---
			setTimerAtaque(20); 
			atacando = true;
			
			float dx = objetivoMasCercano->getX() - getX();
			float dy = objetivoMasCercano->getY() - getY();
			if (std::abs(dx) > std::abs(dy)) setDireccion(dx > 0 ? DERECHA : IZQUIERDA);
			else setDireccion(dy > 0 ? ABAJO : ARRIBA);
		}
		
		if (!atacando && objetivoMasCercano) {
			if (distMinima <= 5.0f && distMinima > 1.2f) { 
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
		
		// --- NUEVO: LÓGICA DE DIBUJADO DE ATAQUE ---
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			
			// Detenemos el desplazamiento artificial. Ahora el sprite hace el trabajo visual.
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
