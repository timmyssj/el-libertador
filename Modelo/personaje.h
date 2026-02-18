#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum Direccion { ABAJO, ARRIBA, IZQUIERDA, DERECHA };

class Personaje : public Entidad {
protected:
	float vida;
	Direccion direccionActual;
	
	// Animación
	sf::Sprite sprite;
	int frameActual;
	float tiempoFrame;
	float duracionFrame;
	bool estaMoviendose;
	
	// MOVIMIENTO POR BLOQUES
	int cooldownMovimiento; 
	int tiempoEntrePasos; 
	
public:
	// Constructor actualizado
	Personaje(float x, float y, float vida, int delayPasos = 8) 
		: Entidad(x, y), vida(vida) {
		
		direccionActual = ABAJO;
		frameActual = 0;
		tiempoFrame = 0;
		duracionFrame = 0.15f; 
		estaMoviendose = false;
		
		cooldownMovimiento = 0;
		tiempoEntrePasos = delayPasos; 
	}
	
	virtual ~Personaje() {}
	
	sf::Sprite& getSprite() { return sprite; }
	float getVida() { return vida; }
	Direccion getDireccion() { return direccionActual; }
	
	void recibirDanio(float cantidad) { vida -= cantidad; if (vida < 0) vida = 0; }
	void curarCompleto() { vida = 100; }
	
	void resetearMovimiento() { 
		estaMoviendose = false; 
		if (cooldownMovimiento > 0) cooldownMovimiento--; 
	}
	
	void moverse(float dx, float dy) {
		if (cooldownMovimiento == 0) {
			x += dx; 
			y += dy;
			cooldownMovimiento = tiempoEntrePasos;
			estaMoviendose = true;
			
			if (dx > 0) direccionActual = DERECHA;
			if (dx < 0) direccionActual = IZQUIERDA;
			if (dy > 0) direccionActual = ABAJO;
			if (dy < 0) direccionActual = ARRIBA;
		}
	}
	
	void reproducirAnimacion(const std::vector<sf::Texture>& animacion) {
		if (animacion.empty()) return;
		
		if (cooldownMovimiento > 0 || estaMoviendose) {
			tiempoFrame += 1.0f / 60.0f; 
			if (tiempoFrame >= duracionFrame) {
				tiempoFrame = 0;
				frameActual++;
				if (frameActual >= (int)animacion.size()) frameActual = 0;
			}
			sprite.setTexture(animacion[frameActual]);
		} else {
			frameActual = 0;
			sprite.setTexture(animacion[0]);
		}
	}
	
	virtual void cargarTextura() = 0; 
};

#endif
