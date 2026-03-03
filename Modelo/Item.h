#ifndef ITEM_H
#define ITEM_H

#include "Entidad.h"
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

class Item : public Entidad {
private:
	sf::Sprite sprite;
	std::string tipoItem; 
	bool activo;          
	
	// --- OPTIMIZACIÓN: Texturas Estáticas ---
	static sf::Texture& getTexSable() {
		static sf::Texture texSable;
		static bool cargada = false;
		if (!cargada) {
			if (!texSable.loadFromFile("sprites/sable.png")) std::cout << "[ALERTA] No se encontro sable.png" << std::endl;
			cargada = true;
		}
		return texSable;
	}
	
	static sf::Texture& getTexCura() {
		static sf::Texture texCura;
		static bool cargada = false;
		if (!cargada) {
			if (!texCura.loadFromFile("sprites/curas.png")) std::cout << "[ALERTA] No se encontro curas.png" << std::endl;
			cargada = true;
		}
		return texCura;
	}
	
public:
		Item(float startX, float startY, std::string t) : Entidad(startX, startY) {
			this->tipoItem = t;
			this->activo = true;
			
			if (this->tipoItem == "ITEM_SABLE") {
				sprite.setTexture(getTexSable());
			}
			else if (this->tipoItem == "ITEM_CURACION") {
				sprite.setTexture(getTexCura());
			}
		}
		
		std::string getTipo() override { return tipoItem; }
		bool estaVivo() override { return activo; }
		void actualizar() override { }
		sf::Sprite* getSpriteRender() override { return &sprite; }
		void recoger() { this->activo = false; }
};

#endif
