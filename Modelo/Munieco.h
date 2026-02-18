#ifndef MUNIECO_H
#define MUNIECO_H

#include "Personaje.h"
#include <iostream>

class Munieco : public Personaje {
private:
	sf::Texture texturaMunieco;
	
public:
	Munieco(float x, float y) : Personaje(x, y, 30, 999) { // 999 porque no se mueve
		// ...
	}
	std::string getTipo() override { return "PRACTICA"; }
	
	// ESTA FUNCIÓN FALTABA
	void cargarTextura() override {
		// En tu lista veo un archivo llamado "sable". Usaremos ese por ahora.
		// Si más tarde agregas "munieco.png", cambia el nombre aquí.
		if (!texturaMunieco.loadFromFile("sprites/sable.png")) {
			std::cerr << "Falta imagen sprites/sable.png" << std::endl;
		}
		sprite.setTexture(texturaMunieco);
	}
	
	void actualizar() override {
		// Estático
	}
};

#endif
