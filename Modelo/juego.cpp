#include "juego.h"

// 1. Constructor
Juego::Juego() {
	salud = 100;
	moral = 100;
	suministros = 50;
	capituloActual = 0; 
	juegoPausado = false; 
	
	cargarNivel(capituloActual);
}

// 2. Cargar Nivel (Con diseño del Tutorial)
void Juego::cargarNivel(int capitulo) {
	// Limpiamos todo a SUELO
	for(int i = 0; i < 20; i++) {
		for(int j = 0; j < 30; j++) {
			mapa[i][j] = SUELO;
		}
	}
	
	// Bordes Indestructibles
	for(int i = 0; i < 20; i++) { mapa[i][0] = PARED; mapa[i][29] = PARED; }
	for(int j = 0; j < 30; j++) { mapa[0][j] = PARED; mapa[19][j] = PARED; }
	
	// Configuración del Tutorial
	if (capitulo == 0) {
		smX = 2; smY = 2; // Inicio
		
		// Muro de prueba
		for(int i = 5; i < 15; i++) { mapa[i][10] = PARED; }
		
		// Zona de agua
		mapa[5][5] = AGUA; mapa[5][6] = AGUA;
		mapa[6][5] = AGUA; mapa[6][6] = AGUA;
	}
}

// 3. Movimiento (BLINDADO: No permite salir del mapa)
void Juego::intentarMover(int dx, int dy) {
	if (juegoPausado) return; 
	
	int nuevoX = smX + dx;
	int nuevoY = smY + dy;
	
	// SEGURIDAD: Evitar que se salga de la matriz (Crasheo)
	if (nuevoX < 0 || nuevoX >= 30 || nuevoY < 0 || nuevoY >= 20) {
		return; 
	}
	
	// LÓGICA: Chocar con pared o agua
	if (mapa[nuevoY][nuevoX] != PARED && mapa[nuevoY][nuevoX] != AGUA) {
		smX = nuevoX;
		smY = nuevoY;
	}
}

// 4. Funciones de Control
void Juego::pausarOReanudar() {
	juegoPausado = !juegoPausado;
}

bool Juego::estaEnPausa() {
	return juegoPausado;
}

void Juego::actualizar() {
	// Vacío por ahora, porque decidimos no poner enemigos todavía.
	// Al estar vacío, no da error de compilación.
}

// 5. Getter del mapa
int Juego::getContenidoCelda(int x, int y) {
	if(x >= 0 && x < 30 && y >= 0 && y < 20) {
		return mapa[y][x];
	}
	return PARED; 
}
