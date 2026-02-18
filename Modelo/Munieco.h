#ifndef MUNIECO_H
#define MUNIECO_H

#include "Personaje.h"
#include <iostream>
#include <vector>

class Munieco : public Personaje {
private:
	// Necesitamos guardar la textura en memoria aquí
	sf::Texture texturaMunieco;
	
public:
	// Velocidad 0.0f porque es estático
	Munieco(float x, float y) : Personaje(x, y, 30, 0.0f) { 
		cargarTextura();
	}
	
	std::string getTipo() override { return "PRACTICA"; }
	
	// --- ESTA ERA LA FUNCIÓN QUE FALTABA ---
	void cargarTextura() override {
		// Intenta cargar "munieco.png". Si no existe, usa "sable.png" o se queda blanco.
		if (!texturaMunieco.loadFromFile("munieco.png")) {
			// Fallback: Si no tienes imagen de muñeco, probamos con otra o avisamos
			if (!texturaMunieco.loadFromFile("sable.png")) {
				std::cerr << "Falta imagen para Munieco (munieco.png)" << std::endl;
			}
		}
		
		// Asignamos la textura al sprite del Padre (Personaje)
		sprite.setTexture(texturaMunieco);
	}
	
	void actualizar() override {
		// Como es un objeto inerte, no necesitamos animarlo ni moverlo.
		// El sprite ya tiene la textura fija cargada desde el principio.
	}
};

#endif
