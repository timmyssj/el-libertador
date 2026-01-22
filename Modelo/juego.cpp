#include "juego.h"
#include <cmath>

// 1. Constructor
Juego::Juego() {
	salud = 100;
	moral = 100;
	suministros = 50;
	capituloActual = 0; 
	juegoPausado = false; 
	
	cargarNivel(capituloActual);
}

// 2. Cargar Nivel
void Juego::cargarNivel(int capitulo) {
	// Limpiamos todo a SUELO
	for(int i = 0; i < 20; i++) {
		for(int j = 0; j < 30; j++) {
			mapa[i][j] = SUELO;
		}
	}
	
	// Bordes (PARED)
	for(int i = 0; i < 20; i++) { mapa[i][0] = PARED; mapa[i][29] = PARED; }
	for(int j = 0; j < 30; j++) { mapa[0][j] = PARED; mapa[19][j] = PARED; }
	
	// Posición inicial
	smX = 5; 
	smY = 5;
	
	// Limpiamos enemigos viejos
	realistas.clear();
	
	if (capitulo == 0) { // Tutorial
		smX = 5; smY = 5;
		
		// SPAWN DE ENEMIGO DE PRUEBA
		Enemigo soldado;
		soldado.x = 15;
		soldado.y = 10;
		soldado.salud = 30;
		soldado.contadorPasos = 0;
		realistas.push_back(soldado); // Lo guardamos en la lista
	}
}

// 3. Movimiento
void Juego::intentarMover(int dx, int dy) {
	if (juegoPausado) return; 
	
	int nuevoX = smX + dx;
	int nuevoY = smY + dy;
	
	// Validación simple de colisión
	if (mapa[nuevoY][nuevoX] != PARED) {
		smX = nuevoX;
		smY = nuevoY;
	}
}

// 4. Pausa
void Juego::pausarOReanudar() {
	juegoPausado = !juegoPausado;
}

bool Juego::estaEnPausa() {
	return juegoPausado;
}

// 5. Actualizar (Esta función faltaba y daba error)
void Juego::actualizar() {
	if (juegoPausado) return;
	
	// Recorremos la lista de enemigos para moverlos uno por uno
	for (size_t i = 0; i < realistas.size(); i++) {
		Enemigo& e = realistas[i]; // Referencia para modificarlo
		
		// Truco para que se muevan más lento que San Martín (cada 10 frames del juego)
		e.contadorPasos++;
		if (e.contadorPasos < 10) continue; 
		e.contadorPasos = 0; // Reseteamos contador
		
		// IA BÁSICA: PERSECUCIÓN
		// Calculamos hacia dónde ir para acercarse a San Martín
		int dx = 0;
		int dy = 0;
		
		if (e.x < smX) dx = 1;  // Si San Martín está a la derecha, voy derecha
		if (e.x > smX) dx = -1; // Si está a la izquierda, voy izquierda
		if (e.y < smY) dy = 1;  // Idem arriba/abajo
		if (e.y > smY) dy = -1;
		
		// Intentamos mover en X primero
		if (mapa[e.y][e.x + dx] != PARED) {
			e.x += dx;
		} 
		// Si no, intentamos en Y
		else if (mapa[e.y + dy][e.x] != PARED) {
			e.y += dy;
		}
	}
}
