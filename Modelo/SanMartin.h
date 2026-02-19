#ifndef SANMARTIN_H
#define SANMARTIN_H

#include "Personaje.h"
#include <iostream>
#include <vector>
#include <string>

class SanMartin : public Personaje {
private:
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba; 
	std::vector<sf::Texture> animAbajo;  
	
public:
	SanMartin(float x, float y) : Personaje(x, y, 100, 6) {
		cargarTextura();
	}
	
	std::string getTipo() override { return "PROCER"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) {
				vector.push_back(t);
			}
		}
	}
	
	void cargarTextura() override {
		// 1. FRENTE (1 sola imagen)
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/san_martin_frente.png")) {
			animAbajo.push_back(tFrente);
		}
		
		// 2. ATRÁS (Mantenemos la secuencia si aún tienes las 4 imágenes de espalda)
		// Si también dejaste solo 1, puedes cambiar esto para que sea como el de frente.
		cargarSecuencia(animArriba, "san_martin_atras", 4);
		
		// 3. DERECHA (1 sola imagen estática)
		sf::Texture tDer;
		// ATENCIÓN: Si renombraste el archivo, quítale el "_1" a la ruta de abajo
		if(tDer.loadFromFile("sprites/san_martin_derecho_1.png")) {
			animDerecha.push_back(tDer);
		}
		
		// 4. IZQUIERDA (1 sola imagen estática)
		sf::Texture tIzq;
		// ATENCIÓN: Si renombraste el archivo, quítale el "_1" a la ruta de abajo
		if(tIzq.loadFromFile("sprites/san_martin_izquierdo_1.png")) {
			animIzquierda.push_back(tIzq);
		}
		
		// Foto inicial al aparecer en el nivel
		if (!animAbajo.empty()) sprite.setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		switch (direccionActual) {
		case DERECHA:   reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA:    reproducirAnimacion(animArriba); break;
		case ABAJO:     reproducirAnimacion(animAbajo); break;
		}
	}
};

#endif
