#ifndef NIVEL_H
#define NIVEL_H

#include <vector>
#include "Entidad.h"
#include "SanMartin.h" // Necesario para crear al héroe
#include "Enemigo.h"   // Necesario para crear enemigos

// Reutilizamos terrenos
enum TipoTerreno {
	SUELO, PARED, AGUA, EVENTO, SALIDA
};

class Nivel {
private:
	// La matriz del mapa (migrada desde Juego)
	int mapa[20][30];
	
	// Usamos punteros (Entidad*) para aprovechar el polimorfismo.
	// permite guardar San Martines y Enemigos mezclados.
	std::vector<Entidad*> entidades;
	
	// Un puntero directo a San Martín para acceso rápido (atajos)
	SanMartin* referenciaHeroe; 
	
public:
	Nivel();
	~Nivel(); // Destructor (Importante para borrar memoria)
	
	// Método del diagrama: Inicializa mapa y spawnea personajes
	void cargarMapa(int numeroCapitulo);
	
	//actualizar entidades del Nivel
	void actualizar();
	
	// Getters para que la Vista y el Juego consulten
	int getContenidoCelda(int x, int y);
	
	// Devuelve la lista completa para dibujarla
	const std::vector<Entidad*>& getEntidades() const { 
		return entidades; 
	}
	
	// Devuelve solo al héroe (útil para la cámara o UI)
	SanMartin* getHeroe() const {
		return referenciaHeroe;
	}
};

#endif
