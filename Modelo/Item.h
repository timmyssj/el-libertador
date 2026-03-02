#ifndef ITEM_H
#define ITEM_H

#include "Entidad.h"
#include <string>
#include <SFML/Graphics.hpp>

class Item : public Entidad {
private:
	sf::Texture textura;
	sf::Sprite sprite;
	std::string tipoItem; // Variable propia para guardar qué ítem es
	bool activo;          // Variable propia para saber si sigue en el piso
	
public:
	// Le pasamos la X y la Y directamente al constructor del padre (Entidad)
	Item(float startX, float startY, std::string t) : Entidad(startX, startY) {
		this->tipoItem = t;
		this->activo = true;
		
		// --- CARGA DE TEXTURAS ---
		if (this->tipoItem == "ITEM_SABLE") {
			textura.loadFromFile("sprites/sable.png"); 
			sprite.setTexture(textura);
		}
		else if (this->tipoItem == "ITEM_CURACION") {
			// RECUERDA: Pon aquí el nombre exacto de tu imagen .png
			textura.loadFromFile("sprites/curas.png"); 
			sprite.setTexture(textura);
		}
	}
	
	// Le respondemos al motor qué tipo de ítem somos
	std::string getTipo() override { 
		return tipoItem; 
	}
	
	// Le respondemos al motor si seguimos vivos/en el mapa
	bool estaVivo() override {
		return activo;
	}
	
	void actualizar() override { 
		// Los items estáticos no hacen nada en su actualización
	}
	
	sf::Sprite* getSpriteRender() override {
		return &sprite;
	}
	
	// Función para que desaparezca cuando San Martín lo toca
	void recoger() {
		this->activo = false; 
	}
};

#endif
