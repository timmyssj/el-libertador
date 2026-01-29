#include "Nivel.h"

Nivel::Nivel() {
	referenciaHeroe = nullptr;
	// Ya no cargamos nada en el constructor.
	// Esperamos a que el hijo llame a 'cargarContenido()'
}

Nivel::~Nivel() {
	for (Entidad* e : entidades) {
		delete e;
	}
	entidades.clear();
	// referenciaHeroe se borra dentro del vector, no hace falta borrarlo aparte
}

// Función de ayuda para no repetir código en cada nivel
void Nivel::inicializarMapaVacio() {
	// 1. Todo Suelo
	for(int y = 0; y < 20; y++) {
		for(int x = 0; x < 30; x++) {
			mapa[y][x] = SUELO;
		}
	}
	// 2. Bordes
	for(int i = 0; i < 20; i++) { mapa[i][0] = PARED; mapa[i][29] = PARED; }
	for(int j = 0; j < 30; j++) { mapa[0][j] = PARED; mapa[19][j] = PARED; }
}

void Nivel::actualizar() {
	for (size_t i = 0; i < entidades.size(); i++) {
		Entidad* e = entidades[i];
		if (e->estaVivo()) {
			e->actualizar();
		} else if (e->getTipo() != "PROCER") { 
			delete e; 
			entidades.erase(entidades.begin() + i);
			i--; 
		}
	}
}

// --- GETTERS Y HELPERS ---

int Nivel::getContenidoCelda(int x, int y) {
	if (x < 0 || x >= 30 || y < 0 || y >= 20) return PARED; 
	return mapa[y][x];
}

SanMartin* Nivel::getHeroe() {
	return referenciaHeroe;
}

const std::vector<Entidad*>& Nivel::getEntidades() {
	return entidades;
}

bool Nivel::hayEnemigosVivos() {
	for (Entidad* e : entidades) {
		if (e->getTipo() == "REALISTA" && e->estaVivo()) return true;
	}
	return false;
}
