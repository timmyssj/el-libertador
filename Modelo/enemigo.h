#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include <cmath>
#include <string>

class Enemigo : public Personaje {
private:
	float rangoVision;
	Personaje* objetivo; // <-- NUEVO: Puntero al héroe para saber a quién perseguir
	
public:
	// Constructor actualizado: Ahora pide el "target" (objetivo)
	Enemigo(float startX, float startY, Personaje* target) 
		: Personaje(startX, startY, 30.0f, 0.05f) { // Velocidad muy baja (0.05) para que no sea imposible
		rangoVision = 10.0f;
		objetivo = target;
	}
	
	// --- IA: CEREBRO DEL ENEMIGO ---
	void actualizar() override {
		if (!objetivo || !objetivo->estaVivo()) return; 
		
		// 1. Calcular distancia
		float dx = objetivo->getX() - x;
		float dy = objetivo->getY() - y;
		float distancia = std::sqrt(dx*dx + dy*dy);
		
		// 2. LÓGICA DE COMBATE
		// Si está "pegado" al héroe (distancia < 1.0), ataca
		if (distancia < 1.0f) {
			// Le quitamos vida al objetivo (San Martín)
			// Usamos un valor pequeño (0.5) porque esto se ejecuta 60 veces por segundo
			objetivo->recibirDanio(0.5f); 
		} 
		// 3. SI NO, PERSEGUIR (Si lo ve pero está lejos)
		else if (distancia < rangoVision) {
			if (std::abs(dx) > std::abs(dy)) {
				if (dx > 0) x += velocidad; else x -= velocidad;
			} else {
				if (dy > 0) y += velocidad; else y -= velocidad;
			}
		}
	}
	
	std::string getTipo() override {
		return "REALISTA"; 
	}
};

#endif // ENEMIGO_H
