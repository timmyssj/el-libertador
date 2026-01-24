#include "juego.h"
#include <cmath> 
#include <vector>

// 1. En el Constructor, inicializamos el estado
Juego::Juego() {
	nivelActual = nullptr; // Aún no hay nivel cargado
	juegoPausado = false;
	capitulo = 0;
	
	// ARRANCAMOS EN EL MENÚ
	estadoActual = MENU_PRINCIPAL;
	opcionSeleccionada = 0; // Primera opción marcada por defecto
	
	// Configuración por defecto
	volumenMusica = 50;
	volumenSonidos = 50;
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
	// Si no estamos jugando o no hay nivel cargado, ¡NO HACER NADA!
	if (estadoActual != JUGANDO || nivelActual == nullptr) return;
	
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
	// Si estamos en el MENÚ, no actualizamos el nivel (evitamos crash)
		if (estadoActual == MENU_PRINCIPAL || estadoActual == PAUSA || estadoActual == CONFIGURACION) {
		return; 
		}
		
		// Solo actualizamos el nivel si estamos JUGANDO y existe el nivel
			if (estadoActual == JUGANDO && nivelActual != nullptr) {
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

// --- LÓGICA DE NAVEGACIÓN ---

void Juego::navegarMenu(int direccion) {
	// Solo navegamos si estamos en un estado de menú
	if (estadoActual == MENU_PRINCIPAL) {
		opcionSeleccionada += direccion;
		
		// Límites del menú principal (Supongamos 3 opciones: Jugar, Config, Salir)
		if (opcionSeleccionada < 0) opcionSeleccionada = 2;
		if (opcionSeleccionada > 2) opcionSeleccionada = 0;
	}
	else if (estadoActual == CONFIGURACION) {
		// Aquí podrías navegar entre opciones de volumen
		opcionSeleccionada += direccion;
		if (opcionSeleccionada < 0) opcionSeleccionada = 1;
		if (opcionSeleccionada > 1) opcionSeleccionada = 0;
	}
	// Añadir lógica para SELECCION_NIVEL si quieres
}

void Juego::confirmarSeleccion() {
	if (estadoActual == MENU_PRINCIPAL) {
		if (opcionSeleccionada == 0) { // Opción "JUGAR"
			estadoActual = SELECCION_NIVEL; // O ir directo a JUGANDO
			opcionSeleccionada = 0; // Reset para el siguiente menú
		}
		else if (opcionSeleccionada == 1) { // Opción "CONFIGURACION"
			estadoActual = CONFIGURACION;
			opcionSeleccionada = 0;
		}
		else if (opcionSeleccionada == 2) { // Opción "SALIR"
			estadoActual = SALIR;
		}
	}
	else if (estadoActual == SELECCION_NIVEL) {
		// Supongamos que eligió el Tutorial
		cargarNivel(0); // Carga el mapa
		estadoActual = JUGANDO; // ¡Arranca el juego!
	}
	else if (estadoActual == CONFIGURACION) {
		// Lógica para cambiar volumen (podríamos usar flechas izq/der)
	}
}

void Juego::teclaEscape() {
	if (estadoActual == JUGANDO) {
		estadoActual = PAUSA; // Pausar juego
		juegoPausado = true;
	}
	else if (estadoActual == PAUSA) {
		estadoActual = JUGANDO; // Reanudar
		juegoPausado = false;
	}
	else if (estadoActual == CONFIGURACION || estadoActual == SELECCION_NIVEL) {
		estadoActual = MENU_PRINCIPAL; // Volver atrás
		opcionSeleccionada = 0;
	}
}
// 5. Getter del mapa
int Juego::getContenidoCelda(int x, int y) {
	// Delegamos la pregunta al nivel usando la flecha
	return nivelActual->getContenidoCelda(x, y);
}
