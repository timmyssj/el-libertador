#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "personaje.h"
#include <cmath>

class Enemigo : public Personaje {
private:
	float rangoVision;
	
public:
	Enemigo(float startX, float startY) 
		: Personaje(startX, startY, 30.0f, 0.5f) { // 30 vida, 0.5 velocidad (más lento)
		rangoVision = 5.0f;
	}
	
	void actualizar() override {
		// Aquí iría la IA automática
	}
	
	bool detectarJugador(float jugadorX, float jugadorY) {
		// Distancia euclidiana
		float dx = x - jugadorX;
		float dy = y - jugadorY;
		float distancia = std::sqrt(dx*dx + dy*dy);
		
		return distancia < rangoVision;
	}
};

#endif
