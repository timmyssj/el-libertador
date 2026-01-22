#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "personaje.h"

#include <vector>
#include <string>

class SanMartin : public Personaje {
private:
	float moral;
	std::vector<std::string> inventario; // Lista de objetos (ej: "Espada", "Mapa")
	
public:
	SanMartin(float startX, float startY) 
		: Personaje(startX, startY, 100.0f, 1.0f) { // 100 vida, 1.0 velocidad
		moral = 100.0f;
	}
	
	void actualizar() override {
		// Lógica específica de San Martín por frame
	}
	
	void arengarTropa() {
		moral += 10;
		if(moral > 100) moral = 100;
	}
	
	void montarCaballo() {
		velocidad = 2.0f; // Aumenta velocidad
	}
	
	float getMoral() const { return moral; }
};

#endif
