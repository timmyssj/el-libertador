#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath> 

class SanMartin : public Personaje {
private:
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba; 
	std::vector<sf::Texture> animAbajo;  
	
	std::vector<sf::Texture> animAtaqueDerecha;
	std::vector<sf::Texture> animAtaqueIzquierda;
	std::vector<sf::Texture> animAtaqueAbajo;  // Se mantiene solo frente
	
public:
	SanMartin(float x, float y) : Personaje(x, y, 100, 6) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
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
		// --- CAMINAR ---
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/san_martin_frente_1.png")) 
			animAbajo.push_back(tFrente);
		
		cargarSecuencia(animArriba, "san_martin_atras", 1);
		
		sf::Texture tDer;
		if(tDer.loadFromFile("sprites/san_martin_derecho_1.png")) 
			animDerecha.push_back(tDer);
		
		sf::Texture tIzq;
		if(tIzq.loadFromFile("sprites/san_martin_izquierdo_1.png")) 
			animIzquierda.push_back(tIzq);
		
		// --- ATAQUES ---
		sf::Texture tAtaqueDer;
		if(tAtaqueDer.loadFromFile("sprites/san_martin_ataque_derecho_2.png")) 
			animAtaqueDerecha.push_back(tAtaqueDer);
		
		sf::Texture tAtaqueIzq;
		if(tAtaqueIzq.loadFromFile("sprites/san_martin_ataque_izquierdo_2.png")) 
			animAtaqueIzquierda.push_back(tAtaqueIzq);
		
		// SOLO ataque frontal
		sf::Texture tAtaqueAbajo;
		if(tAtaqueAbajo.loadFromFile("sprites/san_martin_ataque_frente_1.png")) 
			animAtaqueAbajo.push_back(tAtaqueAbajo);
		
		if (!animAbajo.empty()) 
			getSprite().setTexture(animAbajo[0]);
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
			
			std::vector<sf::Texture>* animActiva = nullptr;
			switch (getDireccion()) {
			case DERECHA:   animActiva = &animAtaqueDerecha; break;
			case IZQUIERDA: animActiva = &animAtaqueIzquierda; break;
			case ABAJO:     animActiva = &animAtaqueAbajo; break;
			
			// ARRIBA usa sprite normal
			case ARRIBA:    animActiva = &animArriba; break;
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
			
			if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA") && e->estaVivo()) {
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
		
		if (golpeoAlgo) {
			std::cout << "[SAN MARTIN] �Sablazo! Impacto al enemigo." << std::endl;
		}
	}
};

#endif
