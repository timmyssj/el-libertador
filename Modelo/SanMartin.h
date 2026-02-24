#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 

class SanMartin : public Personaje {
private:
	int idTraje; // 0 = Tutorial (Uniforme Base), 1 = España, 2 = Andes, etc.
	
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba; 
	std::vector<sf::Texture> animAbajo;  
	
	std::vector<sf::Texture> animAtaqueDerecha;
	std::vector<sf::Texture> animAtaqueIzquierda;
	
public:
	// --- NUEVO CONSTRUCTOR: Ahora recibe el número del nivel/traje ---
	SanMartin(float x, float y, int traje = 0) : Personaje(x, y, 100, 6) {
		idTraje = traje;
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	// --- NUEVA FUNCIÓN INTELIGENTE DE CARGA CON RESPALDO ---
	void cargarImagenConRespaldo(std::vector<sf::Texture>& vector, std::string nombreIdeal, std::string nombreRespaldo) {
		sf::Texture t;
		// Intenta cargar el traje nuevo primero
		if (t.loadFromFile("sprites/" + nombreIdeal + ".png")) {
			vector.push_back(t);
		} 
		// Si falla (porque aún no lo dibujaste), carga el traje normal
		else if (t.loadFromFile("sprites/" + nombreRespaldo + ".png")) {
			vector.push_back(t);
		}
	}
	
	void cargarTextura() override {
		// Determinamos el prefijo según el nivel en el que estamos
		std::string prefijo = "san_martin";
		if (idTraje == 1) prefijo = "san_martin_espana"; // Nivel 1
		// if (idTraje == 2) prefijo = "san_martin_andes"; // Futuro Nivel 2
		
		// 1. ANIMACIONES DE CAMINAR (Busca ej: san_martin_espana_frente_1.png)
		cargarImagenConRespaldo(animAbajo, prefijo + "_frente_1", "san_martin_frente_1");
		cargarImagenConRespaldo(animArriba, prefijo + "_atras_1", "san_martin_atras_1");
		cargarImagenConRespaldo(animDerecha, prefijo + "_derecho_1", "san_martin_derecho_1");
		cargarImagenConRespaldo(animIzquierda, prefijo + "_izquierdo_1", "san_martin_izquierdo_1");
		
		// 2. SPRITES DE ATAQUE
		cargarImagenConRespaldo(animAtaqueDerecha, prefijo + "_ataque_derecho_2", "san_martin_ataque_derecho_2");
		cargarImagenConRespaldo(animAtaqueIzquierda, prefijo + "_ataque_izquierdo_2", "san_martin_ataque_izquierdo_2");
		
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	// ... (Mantén tus funciones actualizar() y atacar() exactamente igual que antes) ...
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
			
			std::vector<sf::Texture>* animActiva = nullptr;
			switch (getDireccion()) {
			case DERECHA:   animActiva = &animAtaqueDerecha; break;
			case IZQUIERDA: animActiva = &animAtaqueIzquierda; break;
			case ARRIBA:    animActiva = &animArriba; break; 
			case ABAJO:     animActiva = &animAbajo; break;
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
	
	void atacar(const std::vector<Entidad*>& entidadesEnElMapa) {
		if (getTimerAtaque() > 0) return;
		setTimerAtaque(20); 
		bool golpeoAlgo = false;
		for (Entidad* e : entidadesEnElMapa) {
			if (e == this) continue; 
			if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA" || e->getTipo() == "FRANCES") && e->estaVivo()) {
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
		if (golpeoAlgo) std::cout << "[SAN MARTIN] ¡Sablazo! Impacto al enemigo." << std::endl;
	}
};

#endif
