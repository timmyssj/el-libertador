#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum Direccion { ABAJO, ARRIBA, IZQUIERDA, DERECHA };

class Personaje : public Entidad {
private: // <--- TODO PRIVADO POR CONSIGNA
	float vida;
	float vidaMax;
	Direccion direccionActual;
	
	sf::Sprite sprite;
	int frameActual;
	float tiempoFrame;
	float duracionFrame;
	bool estaMoviendose;
	
	int cooldownMovimiento; 
	int tiempoEntrePasos; 
	
	int timerAtaque;
	float offsetX;
	float offsetY;
	
public:
	Personaje(float x, float y, float vida, int delayPasos = 8) 
		: Entidad(x, y), vida(vida), vidaMax(vida) {
		
		direccionActual = ABAJO;
		frameActual = 0;
		tiempoFrame = 0;
		duracionFrame = 0.15f; 
		estaMoviendose = false;
		cooldownMovimiento = 0;
		tiempoEntrePasos = delayPasos; 
		timerAtaque = 0;
		offsetX = 0.0f;
		offsetY = 0.0f;
	}
	
	virtual ~Personaje() {}
	
	// --- GETTERS (Acceso de lectura) ---
	sf::Sprite& getSprite() { return sprite; }
	float getVida() const { return vida; }
	float getVidaMax() const { return vidaMax; }
	Direccion getDireccion() const { return direccionActual; }
	
	virtual bool estaVivo() override { return vida > 0; }
	
	// --- MÉTODOS DE MODIFICACIÓN (Setters encapsulados) ---
	void recibirDanio(float cantidad) { 
		vida -= cantidad; 
		if (vida < 0) vida = 0; 
	}
	
	void curarCompleto() { vida = vidaMax; }
	
	void resetearMovimiento() { 
		estaMoviendose = false; 
		if (cooldownMovimiento > 0) cooldownMovimiento--; 
	}
	
	void moverse(float dx, float dy) {
		if (cooldownMovimiento == 0) {
			// AHORA USAMOS GETTERS Y SETTERS DEL PADRE (Entidad)
			setX(getX() + dx); 
			setY(getY() + dy);
			
			cooldownMovimiento = tiempoEntrePasos;
			estaMoviendose = true;
			
			if (dx > 0) direccionActual = DERECHA;
			if (dx < 0) direccionActual = IZQUIERDA;
			if (dy > 0) direccionActual = ABAJO;
			if (dy < 0) direccionActual = ARRIBA;
		}
	}
	
	// --- NUEVOS GETTERS Y SETTERS ---
	int getTimerAtaque() const { return timerAtaque; }
	float getOffsetX() const { return offsetX; }
	float getOffsetY() const { return offsetY; }
	
	void setTimerAtaque(int t) { timerAtaque = t; }
	void setOffsetX(float x) { offsetX = x; }
	void setOffsetY(float y) { offsetY = y; }
	
	void reproducirAnimacion(const std::vector<sf::Texture>& animacion) {
		if (animacion.empty()) return;
		
		if (cooldownMovimiento > 0 || estaMoviendose) {
			tiempoFrame += 1.0f / 60.0f; 
			if (tiempoFrame >= duracionFrame) {
				tiempoFrame = 0;
				frameActual++;
				if (frameActual >= (int)animacion.size()) frameActual = 0;
			}
			sprite.setTexture(animacion[frameActual], true);
		} else {
			frameActual = 0;
			sprite.setTexture(animacion[0], true);
		}
	}
	
	virtual void cargarTextura() = 0; 
};

#endif
