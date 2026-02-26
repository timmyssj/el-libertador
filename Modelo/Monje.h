#ifndef MONJE_H
#define MONJE_H

#include "Personaje.h"
#include <vector>
#include <cstdlib> // Para generar números aleatorios (rand)

class Monje : public Personaje {
private:
	std::vector<sf::Texture> animDerecha, animIzquierda, animArriba, animAbajo;
	int timerMovimiento;
	
public:
	// Le damos 50 de vida (aunque nadie los atacará) y 10 de lentitud
	Monje(float x, float y) : Personaje(x, y, 50, 10) { 
		// Arrancan con un tiempo de espera aleatorio antes de dar su primer paso
		timerMovimiento = std::rand() % 100 + 50; 
		cargarTextura();
	}
	
	// Su etiqueta única. Como nadie busca "MONJE" en su función de atacar, están a salvo.
	std::string getTipo() override { return "MONJE"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			// Intenta cargar la versión con número
			if (t.loadFromFile("sprites/" + nombreBase + "_" + std::to_string(i) + ".png")) {
				vector.push_back(t);
			} 
			// Si no la tiene, intenta cargar la versión base (ej. "monje.png")
			else if (t.loadFromFile("sprites/" + nombreBase + ".png")) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		// --- CARGAR LATERALES Y VERTICALES ---
		
		// 1. Cargar Sprite Derecho
		// Busca "monje_derecho_1.png" o "monje_derecho.png"
		cargarSecuencia(animDerecha, "monje_derecho", 1);
		
		// 2. Cargar Sprite Izquierdo
		// Busca "monje_izquierdo_1.png" o "monje_izquierdo.png"
		cargarSecuencia(animIzquierda, "monje_izquierdo", 1);
		
		// 3. Cargar Sprite de Atrás (Vertical)
		// Busca "monje_atras_1.png" o "monje_atras.png"
		cargarSecuencia(animArriba, "monje_atras", 1);
		
			// 4. Cargar Sprite de Frente (Vertical, el que ya tenías)
			// Busca "monje_frente_1.png" o "monje_frente.png"
			cargarSecuencia(animAbajo, "monje_frente", 1);
		
		// Si no se cargó nada en frente, intentamos cargar "monje.png" como respaldo final
		if (animAbajo.empty()) {
			sf::Texture t;
			if (t.loadFromFile("sprites/monje.png")) animAbajo.push_back(t);
		}
		
		// Establecer textura inicial por defecto
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0], true);
	}
	
	void actualizar() override {
		if (!estaVivo()) return;
		
		// --- IA PACÍFICA DE DEAMBULAR ---
		if (timerMovimiento > 0) {
			timerMovimiento--;
			resetearMovimiento(); // Se queda quieto rezando o asustado
		} else {
			// Es hora de moverse. Decide una dirección al azar (0 a 3)
			int dir = std::rand() % 4;
			float movX = 0, movY = 0;
			
			if (dir == 0) movY = -1; // Arriba
			else if (dir == 1) movY = 1;  // Abajo
			else if (dir == 2) movX = -1; // Izquierda
			else if (dir == 3) movX = 1;  // Derecha
			
			moverse(movX, movY);
			
			// Reseteamos el timer para que espere un rato (entre 1 y 3 segundos) antes de volver a caminar
			timerMovimiento = std::rand() % 150 + 60; 
		}
		
		// --- ANIMACIONES ---
		setOffsetX(0); setOffsetY(0);
		switch (getDireccion()) {
		case DERECHA: reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA: reproducirAnimacion(animArriba); break;
		case ABAJO: reproducirAnimacion(animAbajo); break;
		}
	}
};

#endif
