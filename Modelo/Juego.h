#ifndef JUEGO_H
#define JUEGO_H

#include "Nivel.h"

// NUEVO: Estados posibles del juego
enum EstadoJuego {
	MENU_PRINCIPAL,
	SELECCION_NIVEL,
	CONFIGURACION,
	JUGANDO,
	PAUSA,
	PERDIDO,
	GANADO,
	SALIR // Nuevo estado para que el main sepa cuándo cerrar la ventana
};

class Juego {
private:
	Nivel* nivelActual; 
	int capitulo;
	bool juegoPausado;
	
	// NUEVO: Variable para saber en qué estado estamos
	EstadoJuego estadoActual;
	
	// --- VARIABLES DE MENÚ ---
	int opcionSeleccionada; // 0, 1, 2... (Índice del botón actual)
	
	// --- VARIABLES DE CONFIGURACIÓN ---
	int volumenMusica; // 0 a 100
	int volumenSonidos; // 0 a 100
	
public:
	Juego();
	~Juego();
	
	void cargarNivel(int numCap);
	
	// Control
	void intentarMoverSanMartin(int dx, int dy);
	void atacarConSanMartin();
	void pausarOReanudar();
	
	// --- NUEVOS MÉTODOS DE CONTROL PARA EL MENÚ ---
	// Tu compañero llamará a esto cuando presione flecha ARRIBA/ABAJO
	void navegarMenu(int direccion); // direccion: -1 (arriba), 1 (abajo)
	
	// Tu compañero llamará a esto cuando presione ENTER
	void confirmarSeleccion();
	
	// Tu compañero llamará a esto cuando presione ESCAPE
	void teclaEscape(); // Para pausar o volver atrás
	
	// --- GETTERS PARA LA VISTA ---
	int getOpcionSeleccionada() const { return opcionSeleccionada; }
	int getVolumenMusica() const { return volumenMusica; }
	int getVolumenSonidos() const { return volumenSonidos; }
	
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
