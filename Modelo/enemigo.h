#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include "SanMartin.h"
#include <cmath>

class Enemigo : public Personaje {
private:
	SanMartin* objetivo;
	float rangoVision;
	int cooldownAtaque; // <--- NUEVO: Contador para esperar entre golpes
	
public:
	Enemigo(float x, float y, SanMartin* heroe) 
		: Personaje(x, y, 50, 0.03f) { // 50 HP, Velocidad lenta
		
		objetivo = heroe;
		rangoVision = 6.0f;
		cooldownAtaque = 0; // Al principio está listo para atacar
	}
	
	std::string getTipo() override { return "REALISTA"; }
	
	void actualizar() override {
		// Bajar el contador si es mayor a 0
		if (cooldownAtaque > 0) cooldownAtaque--;
		
		if (!objetivo || !objetivo->estaVivo()) return;
		
		// --- IA DE PERSECUCIÓN ---
		float dx = objetivo->getX() - x;
		float dy = objetivo->getY() - y;
		float dist = std::sqrt(dx*dx + dy*dy);
		
		// Si te ve, te persigue
		if (dist < rangoVision && dist > 0.8f) { // Se detiene un poco antes de chocarte
			float dirX = dx / dist;
			float dirY = dy / dist;
			moverse(dirX, dirY);
		}
	}
	
	// --- NUEVO MÉTODO: Intentar atacar ---
	// Devuelve true si pegó el golpe, false si estaba recargando
	bool intentarAtacar() {
		if (cooldownAtaque == 0) {
			cooldownAtaque = 60; // Espera 60 frames (aprox 1 segundo) para volver a pegar
			return true;
		}
		return false;
	}
};

#endif
