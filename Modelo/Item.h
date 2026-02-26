#ifndef ITEM_H
#define ITEM_H

#include "Entidad.h"
#include <SFML/Graphics.hpp>
#include <string>

class Item : public Entidad {
private:
	sf::Sprite sprite;
	sf::Texture textura;
	std::string tipoItem;
	bool recogido;
	
public:
	Item(float x, float y, std::string tipo) : Entidad(x, y), tipoItem(tipo) {
		recogido = false;
		
		if (tipo == "SABLE") {
			textura.loadFromFile("sprites/sable.png"); 
		} else if (tipo == "CURACION") {
			textura.loadFromFile("sprites/curacion.png"); // Puede ser un botiquín o un mate
		}
		sprite.setTexture(textura, true);
	}
	
	std::string getTipo() override { return "ITEM_" + tipoItem; }
	
	void actualizar() override {} // Un item tirado no hace nada
	
	sf::Sprite* getSpriteRender() override { return &sprite; }
	
	// Si ya lo recogimos, lo marcamos como "muerto" para que el juego deje de dibujarlo
	bool estaVivo() override { return !recogido; }
	
	void recoger() { recogido = true; }
};

#endif
