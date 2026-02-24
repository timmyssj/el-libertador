#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h"
#include <vector>
#include <cmath>
#include <string>

class Enemigo : public Personaje {
private:
	SanMartin* objetivo;
	
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	
	// Solo ataques laterales
	std::vector<sf::Texture> animAtaqueDerecha;
	std::vector<sf::Texture> animAtaqueIzquierda;
	
	int cooldownAtaque;
	
public:
	Enemigo(float x, float y, SanMartin* heroe)
		: Personaje(x, y, 50, 15),
		objetivo(heroe),
		cooldownAtaque(0) {
		cargarTextura();
	}
	
	std::string getTipo() override {
		return "REALISTA";
	}
	
	void cargarSecuencia(std::vector<sf::Texture>& vector,
						 const std::string& nombreBase,
						 int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" +
				std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) {
				vector.push_back(t);
			}
		}
	}
						 
						 void cargarTextura() override {
							 
							 // Caminar
							 cargarSecuencia(animDerecha, "realista_derecho", 1);
							 cargarSecuencia(animIzquierda, "realista_izquierdo", 1);
							 cargarSecuencia(animArriba, "realista_atras", 1);
							 
							 sf::Texture tFrente;
							 if (tFrente.loadFromFile("sprites/realista_frente_1.png")) {
								 animAbajo.push_back(tFrente);
							 }
							 
							 // Ataques laterales
							 sf::Texture tAtaqueDer;
							 if (tAtaqueDer.loadFromFile("sprites/realista_ataque_derecho_2.png")) {
								 animAtaqueDerecha.push_back(tAtaqueDer);
							 }
							 
							 sf::Texture tAtaqueIzq;
							 if (tAtaqueIzq.loadFromFile("sprites/realista_ataque_izquierdo_2.png")) {
								 animAtaqueIzquierda.push_back(tAtaqueIzq);
							 }
							 
							 if (!animAbajo.empty()) {
								 getSprite().setTexture(animAbajo[0]);
							 }
						 }
						 
						 void actualizar() override {
							 
							 if (cooldownAtaque > 0)
								 cooldownAtaque--;
							 
							 // --- SI EST� ATACANDO ---
							 if (getTimerAtaque() > 0) {
								 
								 setTimerAtaque(getTimerAtaque() - 1);
								 
								 std::vector<sf::Texture>* animActiva = nullptr;
								 
								 switch (getDireccion()) {
								 case DERECHA:
									 animActiva = &animAtaqueDerecha;
									 break;
								 case IZQUIERDA:
									 animActiva = &animAtaqueIzquierda;
									 break;
								 case ARRIBA:
									 animActiva = &animArriba;  // usa sprite normal
									 break;
								 case ABAJO:
									 animActiva = &animAbajo;   // usa sprite normal
									 break;
								 }
								 
								 if (animActiva && !animActiva->empty()) {
									 getSprite().setTexture((*animActiva)[0], true);
								 }
								 
								 return;
							 }
							 
							 // --- MOVIMIENTO NORMAL ---
							 if (objetivo && objetivo->estaVivo()) {
								 
								 float dx = objetivo->getX() - getX();
								 float dy = objetivo->getY() - getY();
								 float dist = std::sqrt(dx * dx + dy * dy);
								 
								 if (dist < 8.0f && dist > 1.0f) {
									 
									 if (std::abs(dx) > std::abs(dy)) {
										 if (dx > 0)
											 moverse(1, 0);
										 else
											 moverse(-1, 0);
									 }
									 else {
										 if (dy > 0)
											 moverse(0, 1);
										 else
											 moverse(0, -1);
									 }
								 }
								 else {
									 resetearMovimiento();
								 }
							 }
							 
							 switch (getDireccion()) {
							 case DERECHA:
								 reproducirAnimacion(animDerecha);
								 break;
							 case IZQUIERDA:
								 reproducirAnimacion(animIzquierda);
								 break;
							 case ARRIBA:
								 reproducirAnimacion(animArriba);
								 break;
							 case ABAJO:
								 reproducirAnimacion(animAbajo);
								 break;
							 }
						 }
						 
						 bool intentarAtacar() {
							 if (cooldownAtaque == 0) {
								 cooldownAtaque = 60;
								 setTimerAtaque(20); // duraci�n visual
								 return true;
							 }
							 return false;
						 }
};

#endif
#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "SanMartin.h" 
#include <cmath>
#include <vector>

class Enemigo : public Personaje {
private:
	SanMartin* objetivo;
	std::vector<sf::Texture> animDerecha;
	std::vector<sf::Texture> animIzquierda;
	std::vector<sf::Texture> animArriba;
	std::vector<sf::Texture> animAbajo;
	
	int cooldownAtaque;
	
public:
	Enemigo(float x, float y, SanMartin* heroe) 
		: Personaje(x, y, 50, 15) {
		objetivo = heroe;
		cooldownAtaque = 0;
		cargarTextura(); 
	}
	
	std::string getTipo() override { return "REALISTA"; }
	
	void cargarSecuencia(std::vector<sf::Texture>& vector, std::string nombreBase, int cantidad) {
		for (int i = 1; i <= cantidad; i++) {
			sf::Texture t;
			std::string ruta = "sprites/" + nombreBase + "_" + std::to_string(i) + ".png";
			if (t.loadFromFile(ruta)) vector.push_back(t);
		}
	}
	
	void cargarTextura() override {
		cargarSecuencia(animDerecha, "realista_derecho", 1);
		cargarSecuencia(animIzquierda, "realista_izquierdo", 1);
		cargarSecuencia(animArriba, "realista_atras", 1);
		
		sf::Texture tFrente;
		if(tFrente.loadFromFile("sprites/realista_frente_1.png")) { 
			animAbajo.push_back(tFrente);
		}
		
		// CORRECCI�N AQU�: Usamos getSprite()
		if (!animAbajo.empty()) getSprite().setTexture(animAbajo[0]);
	}
	
	void actualizar() override {
		if (cooldownAtaque > 0) cooldownAtaque--;
		
		if (objetivo && objetivo->estaVivo()) {
			// CORRECCI�N AQU�: Usamos getX() y getY()
			float dx = objetivo->getX() - this->getX();
			float dy = objetivo->getY() - this->getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist < 8.0f && dist > 1.0f) { 
				if (std::abs(dx) > std::abs(dy)) {
					if (dx > 0) moverse(1, 0); else moverse(-1, 0);
				} else {
					if (dy > 0) moverse(0, 1); else moverse(0, -1);
				}
			} else {
				resetearMovimiento(); 
			}
		}
		
		// CORRECCI�N AQU�: Usamos getDireccion()
		switch (getDireccion()) {
		case DERECHA:   reproducirAnimacion(animDerecha); break;
		case IZQUIERDA: reproducirAnimacion(animIzquierda); break;
		case ARRIBA:    reproducirAnimacion(animArriba); break;
		case ABAJO:     reproducirAnimacion(animAbajo); break;
		}
	}
	
	bool intentarAtacar() {
		if (cooldownAtaque == 0) {
			cooldownAtaque = 60; 
			return true;
		}
		return false;
	}
};

#endif
