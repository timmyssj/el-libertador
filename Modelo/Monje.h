#ifndef MONJE_H
#define MONJE_H

#include "Personaje.h"
#include <vector>
#include <cstdlib> 

class Monje : public Personaje {
private:
	int timerMovimiento;
	
	struct Texturas {
		std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
		bool cargadas = false;
	};
	static Texturas& getTexturas() { static Texturas t; return t; }
	
public:
		Monje(float x, float y) : Personaje(x, y, 50, 10) { 
			timerMovimiento = std::rand() % 100 + 50; 
			cargarTextura();
		}
		
		std::string getTipo() override { return "MONJE"; }
		
		void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
			for (int i = 1; i <= cantidad; i++) {
				sf::Texture t;
				if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) vector.push_back(t);
				else if (t.loadFromFile("sprites/" + nombreBase + ".png")) vector.push_back(t);
			}
		}
		
		void cargarTextura() override {
			Texturas& t = getTexturas();
			if (!t.cargadas) {
				cargarSecuencia(t.animDerecha, "monje_derecho", 1);
				cargarSecuencia(t.animIzquierda, "monje_izquierdo", 1);
				cargarSecuencia(t.animArriba, "monje_atras", 1);
				cargarSecuencia(t.animAbajo, "monje_frente", 1);
				if (t.animAbajo.empty()) {
					sf::Texture tex;
					if (tex.loadFromFile("sprites/monje.png")) t.animAbajo.push_back(tex);
				}
				t.cargadas = true;
			}
			if (!t.animAbajo.empty()) getSprite().setTexture(t.animAbajo[0], true);
		}
		
		void actualizar() override {
			if (!estaVivo()) return;
			
			if (timerMovimiento > 0) {
				timerMovimiento--; resetearMovimiento(); 
			} else {
				int dir = std::rand() % 4;
				float movX = 0, movY = 0;
				if (dir == 0) movY = -1; else if (dir == 1) movY = 1;  
				else if (dir == 2) movX = -1; else if (dir == 3) movX = 1;  
				
				moverse(movX, movY);
				timerMovimiento = std::rand() % 150 + 60; 
			}
			
			Texturas& t = getTexturas();
			setOffsetX(0); setOffsetY(0);
			switch (getDireccion()) {
			case DERECHA: reproducirAnimacion(t.animDerecha); break;
			case IZQUIERDA: reproducirAnimacion(t.animIzquierda); break;
			case ARRIBA: reproducirAnimacion(t.animArriba); break;
			case ABAJO: reproducirAnimacion(t.animAbajo); break;
			}
		}
};

#endif
