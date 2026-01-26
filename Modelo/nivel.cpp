#include "Nivel.h"

Nivel::Nivel() {
	referenciaHeroe = nullptr;
	// Podrías llamar a cargarMapa(0) aquí o esperar a que Juego lo haga
}

// DESTRUCTOR: Muy importante en C++ cuando usas punteros "new"
Nivel::~Nivel() {
	for (Entidad* e : entidades) {
		delete e; // Borramos cada personaje de la memoria
	}
	entidades.clear();
}

void Nivel::cargarMapa(int numeroCapitulo) {
	// 1. Limpiamos el mapa (todo suelo por defecto)
	for(int y = 0; y < 20; y++) {
		for(int x = 0; x < 30; x++) {
			mapa[y][x] = SUELO;
		}
	}
	
	// 2. Bordes del mapa (Paredes)
	for(int i = 0; i < 20; i++) { mapa[i][0] = PARED; mapa[i][29] = PARED; }
	for(int j = 0; j < 30; j++) { mapa[0][j] = PARED; mapa[19][j] = PARED; }
	
	if (numeroCapitulo == 0) { // TUTORIAL
		// Paredes internas de prueba
		for(int i = 5; i < 15; i++) { mapa[i][10] = PARED; }
		
		// --- ¡ESTA LÍNEA ES LA CLAVE PARA GANAR! ---
		mapa[18][28] = SALIDA_NIVEL; // Esquina inferior derecha
		// ------------------------------------------
		
		// Héroe
		if (referenciaHeroe) delete referenciaHeroe;
		referenciaHeroe = new SanMartin(2, 2);
		entidades.push_back(referenciaHeroe);
		
		// Enemigos
		entidades.push_back(new Enemigo(15, 5, referenciaHeroe));
		entidades.push_back(new Enemigo(15, 15, referenciaHeroe));
	}
}
void Nivel::actualizar() {
	for (size_t i = 0; i < entidades.size(); i++) {
		Entidad* e = entidades[i];
		
		if (e->estaVivo()) {
			e->actualizar();
		} 
		else if (e->getTipo() != "PROCER") { 
			// Si está muerto y NO es San Martín, lo borramos
			delete e; 
			entidades.erase(entidades.begin() + i);
			i--; // <--- ESTO ES CRÍTICO. Si falta, el juego puede fallar.
		}
	}
}
bool Nivel::hayEnemigosVivos() {
	for (Entidad* e : entidades) {
		// Si encontramos al menos un realista vivo, devolvemos TRUE
		if (e->getTipo() == "REALISTA" && e->estaVivo()) {
			return true;
		}
	}
	// Si terminamos el bucle y no encontramos a nadie
	return false;
}

int Nivel::getContenidoCelda(int x, int y) {
	// Validación de límites para no crashear
	if(x < 0 || x >= 30 || y < 0 || y >= 20) return PARED;
	return mapa[y][x];
}
