#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <SFML/Graphics.hpp>
#include <string>

class Entidad {
private: // <--- ¡CERO PROTECTED! Ahora es estrictamente privado
	float x;
	float y;
	
public:
	Entidad(float x, float y) : x(x), y(y) {}
	virtual ~Entidad() {}
	
	// --- GETTERS ---
	float getX() const { return x; }
	float getY() const { return y; }
	
	// --- SETTERS ---
	void setX(float nuevoX) { x = nuevoX; }
	void setY(float nuevoY) { y = nuevoY; }
	
	virtual std::string getTipo() = 0;
	virtual void actualizar() = 0;
	virtual bool estaVivo() { return true; } 
	// --- NUEVO MÉTODO PARA RENDERIZADO GENÉRICO ---
	// Retorna nullptr por defecto. Si un hijo tiene sprite, debe sobreescribir esto.
	virtual sf::Sprite* getSpriteRender() { return nullptr; }
};

#endif
