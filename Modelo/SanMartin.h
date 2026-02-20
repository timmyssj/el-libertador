#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath> // Necesario para std::sqrt en el ataque

class SanMartin : public Personaje {
private:
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba; 
	std::vector<sf::Texture> animAbajo;  
	
public:
	SanMartin(float x, float y) : Personaje(x, y, 100, 6) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	// --- CARGA DE GRÁFICOS ---
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		// 1. FRENTE (1 sola imagen)
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/san_martin_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		
		// 2. ATRÁS 
		cargarSecuencia(animArriba, "san_martin_atras", 4);
		
		// 3. DERECHA (1 sola imagen estática)
		sf::Texture tDer;
		if(tDer.loadFromFile("sprites/san_martin_derecho_1.png")) {
			animDerecha.push_back(tDer);
		}
		
		// 4. IZQUIERDA (1 sola imagen estática)
		sf::Texture tIzq;
		if(tIzq.loadFromFile("sprites/san_martin_izquierdo_1.png")) {
			animIzquierda.push_back(tIzq);
		}
		
		// Foto inicial
		if (!animAbajo.empty()) sprite.setTexture(animAbajo[0]);
	}
	
	// --- ACTUALIZACIÓN DE MOVIMIENTO ---
	void actualizar() override {
		switch (direccionActual) {
		case DERECHA:   reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA:    reproducirAnimacion(animArriba); break;
		case ABAJO:     reproducirAnimacion(animAbajo); break;
		}
	}
	
	// --- NUEVO: ATAQUE ENCAPSULADO ---
	void atacar(const std::vector<Entidad*>& entidadesEnElMapa) {
		bool golpeoAlgo = false;
		
		for (Entidad* e : entidadesEnElMapa) {
			if (e == this) continue; 
			
			if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA") && e->estaVivo()) {
				
				float dx = e->getX() - this->x; 
				float dy = e->getY() - this->y;
				float dist = std::sqrt(dx*dx + dy*dy);
				
				if (dist <= 2.2f) { 
					Personaje* enemigo = static_cast<Personaje*>(e);
					enemigo->recibirDanio(15.0f); 
					golpeoAlgo = true;
				}
			}
		}
		
		if (golpeoAlgo) {
			std::cout << "[SAN MARTIN] ¡Sablazo! Impacto al enemigo." << std::endl;
		}
	}
};

#endif
