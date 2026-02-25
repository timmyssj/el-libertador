#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 

class SanMartin : public Personaje {
private:
	int idTraje; // 0 = Tutorial, 1 = España, 2 = San Lorenzo/Andes
	
	// Vectores de caminata
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;  
	
	// Vectores de ataque (Estandarizados como "atk")
	std::vector<sf::Texture> atkDerecha, atkIzquierda, atkArriba, atkAbajo; 
	
public:
	SanMartin(float x, float y, int traje = 0) : Personaje(x, y, 100, 6) {
		idTraje = traje;
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	// --- FUNCIÓN RECUPERADA: Lista para el futuro ---
	// Si hoy le pasas '1', carga 1 sprite. Si mañana tienes 4, le pasas '4' y listo.
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			// Primero intenta cargar "nombre_1.png"
			if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) {
				vector.push_back(t);
			} 
			// Si no existe, intenta cargar "nombre.png" (como respaldo para tus sprites actuales)
			else if (t.loadFromFile("sprites/" + nombreBase + ".png")) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		std::string prefijo = "";
		
		// CORRECCIÓN: Usamos idTraje
		if (idTraje == 0) prefijo = "san_martin_espana"; 
		else if (idTraje == 1) prefijo = "san_martin_espana";          
		else if (idTraje == 2) prefijo = "san_martin"; 
		
		// Pasamos el número '1' para mantener tu lógica de un solo sprite
		cargarSecuencia(animDerecha, prefijo + "_derecho", 1);
		cargarSecuencia(animIzquierda, prefijo + "_izquierdo", 1);
		cargarSecuencia(animArriba, prefijo + "_atras", 1);
		cargarSecuencia(animAbajo, prefijo + "_frente", 1);
		
		cargarSecuencia(atkDerecha, prefijo + "_ataque_derecho", 1);
		cargarSecuencia(atkIzquierda, prefijo + "_ataque_izquierdo", 1);
		cargarSecuencia(atkArriba, prefijo + "_ataque_atras", 1);
		cargarSecuencia(atkAbajo, prefijo + "_ataque_frente", 1);
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (getTimerAtaque() > 0) {
			setTimerAtaque(getTimerAtaque() - 1);
			float desplazamiento = (getTimerAtaque() > 10) ? 0.4f : 0.0f; 
			switch (getDireccion()) {
			case DERECHA:   setOffsetX(desplazamiento); setOffsetY(0); break;
			case IZQUIERDA: setOffsetX(-desplazamiento); setOffsetY(0); break;
			case ABAJO:     setOffsetX(0); setOffsetY(desplazamiento); break;
			case ARRIBA:    setOffsetX(0); setOffsetY(-desplazamiento); break;
			}
			
			// CORRECCIÓN: Usamos los vectores atk directamente
			std::vector<sf::Texture>* animActiva = nullptr;
			switch (getDireccion()) {
			case DERECHA:   animActiva = &atkDerecha; break;
			case IZQUIERDA: animActiva = &atkIzquierda; break;
			case ARRIBA:    animActiva = &atkArriba; break; 
			case ABAJO:     animActiva = &atkAbajo; break;
			}
			if (animActiva != nullptr && !animActiva->empty()) {
				getSprite().setTexture((*animActiva)[0], true);
			}
		} else {
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
	
	void atacar(const std::vector<Entidad*>& mapaEntidades) {
		float targetX = getX();
		float targetY = getY();
		
		switch (getDireccion()) {
		case ARRIBA:    targetY -= 1.0f; break;
		case ABAJO:     targetY += 1.0f; break;
		case IZQUIERDA: targetX -= 1.0f; break;
		case DERECHA:   targetX += 1.0f; break;
		}
		
		for (Entidad* e : mapaEntidades) {
			if (e != this && e->estaVivo() && 
				(e->getTipo() == "FRANCES" || e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA")) {
				
				float dx = e->getX() - targetX;
				float dy = e->getY() - targetY;
				float dist = std::sqrt(dx*dx + dy*dy);
				
				if (dist <= 1.0f) { 
					Personaje* enemigo = dynamic_cast<Personaje*>(e);
					if (enemigo) {
						enemigo->recibirDanio(15.0f); 
					}
				}
			}
		}
		
		setTimerAtaque(12); 
	}
	
	
};

#endif
