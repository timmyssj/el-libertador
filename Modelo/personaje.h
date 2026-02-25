#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "Entidad.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

enum Direccion { ABAJO, ARRIBA, IZQUIERDA, DERECHA };

class Personaje : public Entidad {
private: // --- ¡VOLVEMOS A LA PUREZA DEL ENCAPSULAMIENTO! ---
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
	
	const std::vector<Entidad*>* mapaEntidades; 
	
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
		mapaEntidades = nullptr;
	}
	
	virtual ~Personaje() {}
	
	// --- NUEVOS GETTERS PARA LOS HIJOS ---
	int getCooldownMovimiento() const { return cooldownMovimiento; }
	const std::vector<Entidad*>* getMapaEntidades() const { return mapaEntidades; }
	// -------------------------------------
	
	void setMapaEntidades(const std::vector<Entidad*>* mapa) { mapaEntidades = mapa; }
	
	bool casillaOcupada(float targetX, float targetY) {
		// --- CORRECCIÓN: Límites exactos de la matriz (0 a 29 y 0 a 19) ---
		if (targetX < 0 || targetX >= 30 || targetY < 0 || targetY >= 20) return true;
		
		if (mapaEntidades) {
			for (Entidad* e : *mapaEntidades) {
				if (e != this && e->estaVivo()) {
					if (std::abs(e->getX() - targetX) < 0.1f && std::abs(e->getY() - targetY) < 0.1f) {
						return true;
					}
				}
			}
		}
		return false;
	}
	
	sf::Sprite& getSprite() { return sprite; } 
	float getVida() const { return vida; }
	float getVidaMax() const { return vidaMax; }
	Direccion getDireccion() const { return direccionActual; }
	
	int getTimerAtaque() const { return timerAtaque; }
	float getOffsetX() const { return offsetX; }
	float getOffsetY() const { return offsetY; }
	sf::Sprite* getSpriteRender() override { return &sprite; }
	
	void setTimerAtaque(int t) { timerAtaque = t; }
	void setOffsetX(float x) { offsetX = x; }
	void setOffsetY(float y) { offsetY = y; }
	
	virtual bool estaVivo() override { return vida > 0; }
	
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
			float targetX = getX() + dx;
			float targetY = getY() + dy;
			
			if (!casillaOcupada(targetX, targetY)) {
				setX(targetX); 
				setY(targetY);
				cooldownMovimiento = tiempoEntrePasos;
				estaMoviendose = true;
			} else {
				estaMoviendose = false; 
			}
			
			if (dx > 0) direccionActual = DERECHA;
			if (dx < 0) direccionActual = IZQUIERDA;
			if (dy > 0) direccionActual = ABAJO;
			if (dy < 0) direccionActual = ARRIBA;
		} else {
			cooldownMovimiento--;
			estaMoviendose = true; 
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
			sprite.setTexture(animacion[frameActual], true); 
		} else {
			frameActual = 0;
			sprite.setTexture(animacion[0], true); 
		}
	}
	
	// Permite girar al personaje hacia un lado sin tener que caminar
	void setDireccion(Direccion nuevaDireccion) { direccionActual = nuevaDireccion; }
	
	virtual void cargarTextura() = 0; 
};

#endif
