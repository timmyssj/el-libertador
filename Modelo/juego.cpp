#include "juego.h"
#include <cmath> 
#include <vector>

// 1. En el Constructor, inicializamos el estado
Juego::Juego() {
	nivelActual = new Nivel(); 
	juegoPausado = false;
	capitulo = 0;
	
	estadoActual = JUGANDO; // <-- Arrancamos jugando
	
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

// 2. Modificamos actualizar() para verificar victoria/derrota
void Juego::actualizar() {
	// Si ya terminó el juego o está en pausa, no hacemos nada
	if (juegoPausado || estadoActual != JUGANDO) return;
	
	// A. Actualizamos el mundo (Movimiento IA, etc.)
	nivelActual->actualizar();
	
	// B. VERIFICAR DERROTA
	// Si San Martín murió (vida <= 0)
	SanMartin* heroe = nivelActual->getHeroe();
	if (heroe == nullptr || !heroe->estaVivo()) {
		estadoActual = PERDIDO;
		return; // Fin del frame
	}
	
	// C. VERIFICAR VICTORIA
	// Obtenemos la posición actual del héroe
	// Nota: Casteamos a int para usarlo en la matriz
	int x = (int)heroe->getX();
	int y = (int)heroe->getY();
	
	// Preguntamos al nivel qué hay en el suelo donde está parado el héroe
	int terrenoBajoLosPies = nivelActual->getContenidoCelda(x, y);
	
	if (terrenoBajoLosPies == SALIDA_NIVEL) {
		estadoActual = GANADO;
	}
}
void Juego::atacarConSanMartin() {
	if (juegoPausado) return;
	
	// 1. Obtenemos al héroe y la lista de todos
	SanMartin* heroe = nivelActual->getHeroe();
	const std::vector<Entidad*>& entidades = nivelActual->getEntidades();
	
	if (!heroe) return;
	
	// 2. Buscamos víctimas cercanas
	for (Entidad* e : entidades) {
		// Ignoramos a San Martín (no se ataca a sí mismo)
		if (e == heroe) continue;
		
		// Si la entidad es un REALISTA (usamos getTipo para saberlo)
		if (e->getTipo() == "REALISTA" && e->estaVivo()) {
			
			// Calculamos distancia
			float dx = e->getX() - heroe->getX();
			float dy = e->getY() - heroe->getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			// RANGO DE LA ESPADA: 1.5 casillas
			if (dist < 1.5f) {
				// Convertimos Entidad* a Personaje* para poder dañarlo
				// (Dynamic cast es lo seguro, pero static_cast es más rápido aquí)
				Personaje* p = static_cast<Personaje*>(e);
				
				p->recibirDanio(30.0f); // ¡Golpe fuerte!
				
				// Opcional: Sumar moral por cada golpe
				heroe->arengarTropa(); 
			}
		}
	}
}

// 5. Getter del mapa
int Juego::getContenidoCelda(int x, int y) {
	// Delegamos la pregunta al nivel usando la flecha
	return nivelActual->getContenidoCelda(x, y);
}
