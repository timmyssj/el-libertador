#ifndef GRANADERO_H
#define GRANADERO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <cmath>
#include <vector>
#include <string>

class Granadero : public Personaje {
private:
	SanMartin* lider;
	
	// Vectores para la animación
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba; 
	std::vector<sf::Texture> animAbajo;  
	
public:
	Granadero(float x, float y, SanMartin* sanMartin) 
		: Personaje(x, y, 100, 0.045f) {
		lider = sanMartin;
		cargarTextura(); 
	}
	
	std::string getTipo() override { return "ALIADO"; }
	
	// Ayudante de carga
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) vector.push_back(t);
		}
	}
	
	void cargarTextura() override {
		// Usamos los nombres que vi en tu lista de archivos:
		cargarSecuencia(animDerecha, "granadero_derecha", 4);
		cargarSecuencia(animIzquierda, "granadero_izquierda", 4); 
		
		// No vi "granadero_espalda" en tu lista, así que usaremos frente o izquierda por ahora
		// Ojo: Si tienes "granadero_atras", cambia esto:
		cargarSecuencia(animArriba, "granadero_izquierda", 4); 
		
		// Frente
		sf::Texture tFrente;
		if(tFrente.loadFromFile("granadero_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		
		if (!animAbajo.empty()) sprite.setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		if (!lider || !lider->estaVivo()) return;
		
		// IA: Seguir a San Martín
		float dx = lider->getX() - x;
		float dy = lider->getY() - y;
		float distancia = std::sqrt(dx*dx + dy*dy);
		
		if (distancia > 2.0f) { // Mantener distancia
			moverse(dx/distancia, dy/distancia);
		} else {
			resetearMovimiento();
		}
		
		// Animación
		switch (direccionActual) {
		case DERECHA:   reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA:    reproducirAnimacion(animArriba); break;
		case ABAJO:     reproducirAnimacion(animAbajo); break;
		}
	}
};

#endif
