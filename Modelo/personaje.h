#ifndef PERSONAJE_H
#define PERSONAJE_H

#include "entidad.h"

class Personaje : public Entidad {
protected:
	float vida;
	float velocidad;
	
public:
	Personaje(float startX, float startY, float vidaInicial, float vel) 
		: Entidad(startX, startY), vida(vidaInicial), velocidad(vel) {}
	
	// Implementación básica de recibir daño
	void recibirDanio(float cantidad) {
		vida -= cantidad;
		if (vida <= 0) {
			vida = 0;
			vivo = false;
		}
	}
	
	// Método para mover
	void moverse(float dx, float dy) {
		x += dx * velocidad;
		y += dy * velocidad;
	}
	
	float getVida() const { return vida; }
};

#endif
