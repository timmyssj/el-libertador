#ifndef JUEGO_H
#define JUEGO_H

#include "Nivel.h"

// NUEVO: Estados posibles del juego
enum EstadoJuego {
	JUGANDO,
	GANADO,
	PERDIDO
};

class Juego {
private:
	Nivel* nivelActual; 
	int capitulo;
	bool juegoPausado;
	
	// NUEVO: Variable para saber en qué estado estamos
	EstadoJuego estadoActual;
	
public:
	Juego();
	~Juego();
	
	void cargarNivel(int numCap);
	
	// Control
	void intentarMoverSanMartin(int dx, int dy);
	void atacarConSanMartin();
	void pausarOReanudar();
	
	// Getters
	bool estaEnPausa() const;
	
	// NUEVO GETTER: Para que la Ventana sepa si debe mostrar "Game Over" o "Victoria"
	EstadoJuego getEstado() const { return estadoActual; }
	
	Nivel* getNivel() { return nivelActual; }
	int getContenidoCelda(int x, int y);
	
	// Lógica principal
	void actualizar(); 
};

#endif
