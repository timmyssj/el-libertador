#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum Direccion { ABAJO, ARRIBA, IZQUIERDA, DERECHA };

class Personaje : public Entidad {
protected:
	float vida;
	float velocidad;
	Direccion direccionActual;
	
	// Animación
	sf::Sprite sprite;
	int frameActual;
	float tiempoFrame;
	float duracionFrame;
	bool estaMoviendose;
	
public:
	Personaje(float x, float y, float vida, float velocidad) 
		: Entidad(x, y), vida(vida), velocidad(velocidad) {
		direccionActual = ABAJO;
		frameActual = 0;
		tiempoFrame = 0;
		duracionFrame = 0.15f; 
		estaMoviendose = false;
	}
	
	virtual ~Personaje() {} // Destructor virtual importante
	
	sf::Sprite& getSprite() { return sprite; }
	float getVida() { return vida; }
	Direccion getDireccion() { return direccionActual; }
	
	void recibirDanio(float cantidad) { vida -= cantidad; if (vida < 0) vida = 0; }
	void curarCompleto() { vida = 100; }
	
	void resetearMovimiento() { estaMoviendose = false; }
	
	void moverse(float dx, float dy) {
		x += dx * velocidad;
		y += dy * velocidad;
		estaMoviendose = true;
		
		if (dx > 0) direccionActual = DERECHA;
		if (dx < 0) direccionActual = IZQUIERDA;
		if (dy > 0) direccionActual = ABAJO;
		if (dy < 0) direccionActual = ARRIBA;
	}
	
	void reproducirAnimacion(const std::vector<sf::Texture>& animacion) {
		if (animacion.empty()) return;
		
		if (!estaMoviendose) {
			frameActual = 0;
			sprite.setTexture(animacion[0]);
			return;
		}
		
		tiempoFrame += 1.0f / 60.0f; // Asumiendo 60 FPS
		if (tiempoFrame >= duracionFrame) {
			tiempoFrame = 0;
			frameActual++;
			if (frameActual >= (int)animacion.size()) frameActual = 0;
		}
		sprite.setTexture(animacion[frameActual]);
	}
	
	// Al definir esto puro (=0), obligamos a los hijos a tener la función.
	virtual void cargarTextura() = 0; 
};

#endif
