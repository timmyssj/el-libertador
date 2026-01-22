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
	// 1. Limpiar lista antigua si recargamos nivel
	for (Entidad* e : entidades) { delete e; }
	entidades.clear();
	
	// 2. Limpiar Mapa (Todo Suelo)
	for(int i = 0; i < 20; i++) {
		for(int j = 0; j < 30; j++) {
			mapa[i][j] = SUELO;
		}
	}
	// Bordes
	for(int i = 0; i < 20; i++) { mapa[i][0] = PARED; mapa[i][29] = PARED; }
	for(int j = 0; j < 30; j++) { mapa[0][j] = PARED; mapa[19][j] = PARED; }
	
	// 3. Configuración según capítulo
	if (numeroCapitulo == 0) { // Tutorial
		// Muros de prueba
		for(int i = 5; i < 15; i++) { mapa[i][10] = PARED; }
		
		// --- CREACIÓN DE ENTIDADES (Lo que pide el diagrama) ---
		
		// a) Creamos a San Martín
		referenciaHeroe = new SanMartin(2, 2); // Nace en (2,2)
		entidades.push_back(referenciaHeroe);  // Lo agregamos a la lista general
		
		// b) Creamos Enemigos
		Enemigo* realista1 = new Enemigo(15, 5);
		entidades.push_back(realista1);
		
		Enemigo* realista2 = new Enemigo(15, 15);
		entidades.push_back(realista2);
	}
}
void Nivel::actualizar() {
	// Recorremos todas las entidades (Héroe y Enemigos)
	for (Entidad* e : entidades) {
		if (e->estaVivo()) {
			e->actualizar(); // Polimorfismo: Cada uno hará lo suyo (IA o nada)
		}
	}
}

int Nivel::getContenidoCelda(int x, int y) {
	// Validación de límites para no crashear
	if(x < 0 || x >= 30 || y < 0 || y >= 20) return PARED;
	return mapa[y][x];
}
