#ifndef GRANADERO_H
#define GRANADERO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath> // Necesario para calcular distancias (raiz cuadrada)

class Granadero : public Personaje {
private:
	SanMartin* lider;      // A quién debe seguir
	float distanciaMinima; // Para mantener la formación
	
public:
	// Constructor: Pide posición X, Y y el puntero al Héroe
	Granadero(float x, float y, SanMartin* sanMartin) 
		: Personaje(x, y, 100, 0.045f) { // 100 Vida, Velocidad similar al héroe
		
		lider = sanMartin;
		distanciaMinima = 2.0f; // Se detiene a 2 bloques de distancia
	}
	
	// Identificador para que la Vista sepa pintarlo de AZUL
	std::string getTipo() override { return "ALIADO"; }
	
	void actualizar() override {
		// Seguridad: Si el líder murió o no existe, no hacemos nada
		if (!lider || !lider->estaVivo()) return;
		
		// 1. Calcular vector hacia el líder
		float dx = lider->getX() - x;
		float dy = lider->getY() - y;
		
		// Teorema de Pitágoras para saber la distancia real
		float distancia = std::sqrt(dx*dx + dy*dy);
		
		// 2. IA DE SEGUIMIENTO
		// Solo se mueve si el líder se aleja más de la cuenta
		if (distancia > distanciaMinima) {
			
			// Normalizar el vector (Dirección unitaria)
			float dirX = dx / distancia;
			float dirY = dy / distancia;
			
			// Moverse hacia el líder
			moverse(dirX, dirY);
		}
	}
};

#endif
