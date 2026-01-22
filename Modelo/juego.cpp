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
	
	// 1. Obtenemos al Héroe
	SanMartin* heroe = nivelActual->getHeroe();
	
	if (heroe != nullptr) {
		// 2. Calculamos A DÓNDE quiere ir (Futuro)
		// Nota: Convertimos a int porque el mapa funciona con enteros
		int proximoX = (int)(heroe->getX() + dx * 1.0f); // Asumiendo velocidad 1
		int proximoY = (int)(heroe->getY() + dy * 1.0f);
		
		// 3. PREGUNTAMOS AL NIVEL: ¿Qué hay ahí?
		int contenido = nivelActual->getContenidoCelda(proximoX, proximoY);
		
		// 4. SI NO ES PARED, nos movemos
		if (contenido != PARED && contenido != AGUA) {
			heroe->moverse(dx, dy);
		}
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
	if (juegoPausado) return;
	
	// Delegamos la actualización al nivel actual
	nivelActual->actualizar();
}
void Juego::atacarConSanMartin() {
	// Pendiente
}

// 5. Getter del mapa
int Juego::getContenidoCelda(int x, int y) {
	// Delegamos la pregunta al nivel usando la flecha
	return nivelActual->getContenidoCelda(x, y);
}
