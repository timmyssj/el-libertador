#ifndef MUNIECO_H
#define MUNIECO_H

#include "Personaje.h"

class Munieco : public Personaje {
public:
	// Constructor: Le damos 30 de vida y 0 velocidad
	Munieco(float x, float y) 
		: Personaje(x, y, 30, 0.0f) { 
	}
	
	// Tipo único para pintarlo de otro color
	std::string getTipo() override { return "PRACTICA"; }
	
	// Su IA es... no hacer nada. Es un objeto inerte.
	void actualizar() override {
		// No se mueve.
		// No ataca.
		// Solo existe para recibir espadazos.
	}
};

#endif
