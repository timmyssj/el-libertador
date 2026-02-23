#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Entidad.h"
#include <SFML/Graphics.hpp>
#include <string>

class Obstaculo : public Entidad {
private:
	sf::Sprite sprite;
	sf::Texture textura;
	std::string tipoObjeto;
	
public:
	Obstaculo(float x, float y, std::string tipo) : Entidad(x, y), tipoObjeto(tipo) {
		// Carga la textura según el tipo
		if (tipo == "ARBOL") {
			textura.loadFromFile("sprites/arbol.png"); // Reemplaza por tu archivo real
		} else if (tipo == "ROCA") {
			textura.loadFromFile("sprites/roca.png");  // Reemplaza por tu archivo real
		}
		
		sprite.setTexture(textura, true);
	}
	
	std::string getTipo() override { return "OBSTACULO_" + tipoObjeto; }
	
	// Un obstáculo no hace nada, es estático
	void actualizar() override {} 
	
	// Le prestamos el sprite a la ventana para que lo dibuje
	sf::Sprite* getSpriteRender() override { return &sprite; }
};

#endif
