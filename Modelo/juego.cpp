#include "juego.h"

// 1. Constructor
Juego::Juego() {
	nivelActual = new Nivel(); // Creamos el puntero
	juegoPausado = false;
	capitulo = 0;
	
	cargarNivel(capitulo);
}

// Destructor
Juego::~Juego() {
	delete nivelActual;
}

// 2. Cargar Nivel
void Juego::cargarNivel(int numCap) {
	// CORRECCIÓN: Usamos '->' porque nivelActual es un puntero.
	// Y llamamos a 'cargarMapa' (que es el nombre real en la clase Nivel)
	nivelActual->cargarMapa(numCap);
}

// 3. Movimiento delegando al Héroe
void Juego::intentarMoverSanMartin(int dx, int dy) {
	if (juegoPausado) return; 
	
	// Verificamos que el héroe exista para evitar crasheos
	if (nivelActual->getHeroe() != nullptr) {
		nivelActual->getHeroe()->moverse(dx, dy);
	}
}

// 4. Funciones de Control
void Juego::pausarOReanudar() {
	juegoPausado = !juegoPausado;
}

bool Juego::estaEnPausa() const {
	return juegoPausado;
}

void Juego::actualizar() {
	// Lógica futura para actualizar enemigos
}

void Juego::atacarConSanMartin() {
	// Pendiente
}

// 5. Getter del mapa
int Juego::getContenidoCelda(int x, int y) {
	// Delegamos la pregunta al nivel usando la flecha
	return nivelActual->getContenidoCelda(x, y);
}
