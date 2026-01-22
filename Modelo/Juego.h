#ifndef JUEGO_H
#define JUEGO_H

#include "nivel.h" // Asegúrate de que coincida mayúscula/minúscula con tu archivo

class Juego {
private:
	Nivel* nivelActual; 
	int capitulo;
	bool juegoPausado;
	
public:
	Juego();
	~Juego(); // Destructor para limpiar memoria
	
	void cargarNivel(int numCap);
	void actualizar(); 
	
	// --- CONTROLES ---
	void intentarMoverSanMartin(int dx, int dy);
	void atacarConSanMartin();
	
	// --- AQUÍ ESTÁ LA FUNCIÓN QUE FALTABA ---
	void pausarOReanudar();
	bool estaEnPausa() const; // El const ayuda a que no dé error en getters
	
	// --- GETTERS ---
	Nivel* getNivel() { return nivelActual; }
	
	// Esta también faltaba declarar y daba error en el cpp
	int getContenidoCelda(int x, int y);
};

#endif
