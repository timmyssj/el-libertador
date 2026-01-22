#ifndef ENTIDAD_H
#define ENTIDAD_H

#include <string>

class Entidad{
protected:
	float x, y;
	bool vivo;
	
public:
	Entidad(float startX, float startY) : x(startX), y(startY), vivo(true){}
	virtual ~Entidad() {} //Destructor virtual
	
	//Metodos virtuales puros
	virtual void actualizar() = 0;
	
	//cada hijo debe decir que es
	virtual std::string getTipo() = 0;
	
	//Getters
	float getX() const { return x;}
	float getY() const { return y;}
	bool estaVivo() const { return vivo; }
};
#endif 
