#include "juego.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <string> // <--- IMPORTANTE: Necesario para std::to_string

// 1. CONSTRUCTOR
Juego::Juego() {
	estadoActual = EN_MENU;
	estadoAnterior = EN_MENU;
	
	volumenMusica = 50;
	volumenSonidos = 50;
	
	// Opciones iniciales
	menuPrincipal = new Menu({"COMENZAR CAMPAÑA", "CONFIGURACION", "SALIR"});
	menuPausa = new Menu({"CONTINUAR", "CONFIGURACION", "SALIR AL MENU"});
	
	// Creamos el menú config. La función actualizarTextosConfig pondrá los números.
	menuConfig = new Menu({"Musica", "Sonidos", "Volver"}); 
	actualizarTextosConfig(); 
	
	nivelActual = nullptr;
}

// 2. DESTRUCTOR
Juego::~Juego() {
	delete menuPrincipal;
	delete menuPausa;
	delete menuConfig; // No olvidar este
	if (nivelActual) delete nivelActual;
}

// --- PROCESAMIENTO DE TECLAS ---

void Juego::procesarTeclaArriba() {
	if (estadoActual == EN_MENU) menuPrincipal->moverArriba();
	else if (estadoActual == PAUSA) menuPausa->moverArriba();
	else if (estadoActual == CONFIGURACION) menuConfig->moverArriba();
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		// Mover San Martín al Norte
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX();
			int ty = (int)heroe->getY() - 1;
			if (nivelActual->getContenidoCelda(tx, ty) != PARED) {
				heroe->moverse(0, -1);
			}
		}
	}
}

void Juego::procesarTeclaAbajo() {
	if (estadoActual == EN_MENU) menuPrincipal->moverAbajo();
	else if (estadoActual == PAUSA) menuPausa->moverAbajo();
	else if (estadoActual == CONFIGURACION) menuConfig->moverAbajo();
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		// Mover San Martín al Sur
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX();
			int ty = (int)heroe->getY() + 1;
			if (nivelActual->getContenidoCelda(tx, ty) != PARED) {
				heroe->moverse(0, 1);
			}
		}
	}
}

void Juego::procesarTeclaIzquierda() {
	// A. Si estamos configurando: Bajar volumen
	if (estadoActual == CONFIGURACION) {
		int op = menuConfig->getOpcionActual();
		if (op == 0) { 
			volumenMusica -= 10;
			if (volumenMusica < 0) volumenMusica = 0;
		}
		else if (op == 1) {
			volumenSonidos -= 10;
			if (volumenSonidos < 0) volumenSonidos = 0;
		}
		actualizarTextosConfig(); // Refrescar texto
	}
	// B. Si estamos jugando: Mover izquierda
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX() - 1;
			int ty = (int)heroe->getY();
			if (nivelActual->getContenidoCelda(tx, ty) != PARED) {
				heroe->moverse(-1, 0);
			}
		}
	}
}

void Juego::procesarTeclaDerecha() {
	// A. Si estamos configurando: Subir volumen
	if (estadoActual == CONFIGURACION) {
		int op = menuConfig->getOpcionActual();
		if (op == 0) { // Musica
			volumenMusica += 10;
			if (volumenMusica > 100) volumenMusica = 100;
		}
		else if (op == 1) { // Sonido
			volumenSonidos += 10;
			if (volumenSonidos > 100) volumenSonidos = 100;
		}
		actualizarTextosConfig();
	}
	// B. Si estamos jugando: Mover derecha
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX() + 1;
			int ty = (int)heroe->getY();
			if (nivelActual->getContenidoCelda(tx, ty) != PARED) {
				heroe->moverse(1, 0);
			}
		}
	}
}

void Juego::procesarTeclaEnter() {
	// --- CASO 1: ESTAMOS EN MENU PRINCIPAL ---
	if (estadoActual == EN_MENU) {
		int op = menuPrincipal->getOpcionActual();
		
		if (op == 0) { // OPCIÓN: COMENZAR CAMPAÑA
			estadoActual = JUGANDO;
			if (nivelActual) delete nivelActual; // Seguridad
			nivelActual = new Nivel();
			nivelActual->cargarMapa(0); // Cargar Tutorial
		}
		else if (op == 1) { // OPCIÓN: CONFIGURACION
			estadoAnterior = EN_MENU;
			estadoActual = CONFIGURACION;
			menuConfig->reiniciarCursor();
		}
		else if (op == 2) { // OPCIÓN: SALIR
			estadoActual = SALIR;
		}
	}
	// --- CASO 2: ESTAMOS EN PAUSA ---
	else if (estadoActual == PAUSA) {
		int op = menuPausa->getOpcionActual();
		
		if (op == 0) { // OPCIÓN: CONTINUAR
			estadoActual = JUGANDO; 
		}
		else if (op == 1) { // OPCIÓN: CONFIGURACION
			estadoAnterior = PAUSA;
			estadoActual = CONFIGURACION;
			menuConfig->reiniciarCursor();
		}
		else if (op == 2) { // OPCIÓN: SALIR AL MENU
			// Destruimos la partida actual
			delete nivelActual;
			nivelActual = nullptr;
			// Volvemos a la portada
			estadoActual = EN_MENU;
			menuPrincipal->reiniciarCursor();
		}
	}
	// --- CASO 3: ESTAMOS CONFIGURANDO ---
	else if (estadoActual == CONFIGURACION) {
		int op = menuConfig->getOpcionActual();
		if (op == 2) { // Opción "VOLVER"
			estadoActual = estadoAnterior; // Regresa a donde estaba
		}
	}
}

void Juego::teclaEscape() {
	if (estadoActual == JUGANDO) {
		estadoActual = PAUSA;
		menuPausa->reiniciarCursor();
	}
	else if (estadoActual == PAUSA) {
		estadoActual = JUGANDO; // Reanudar
	}
	// Salir de pantallas finales
	else if (estadoActual == GAME_OVER || estadoActual == VICTORIA) {
		if (nivelActual) {
			delete nivelActual;
			nivelActual = nullptr;
		}
		estadoActual = EN_MENU;
	}
	// Salir de Configuración hacia atrás
	else if (estadoActual == CONFIGURACION) {
		estadoActual = estadoAnterior;
	}
}

// --- ACTUALIZACIÓN ---

void Juego::actualizar() {
	if (estadoActual == JUGANDO && nivelActual != nullptr) {
		
		nivelActual->actualizar();
		
		// Verificar Derrota
		if (!nivelActual->getHeroe()->estaVivo()) {
			estadoActual = GAME_OVER;
		}
		
		// --- VERIFICAR VICTORIA ---
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int x = (int)heroe->getX();
			int y = (int)heroe->getY();
			
			// SI PISA LA SALIDA...
			if (nivelActual->getContenidoCelda(x, y) == SALIDA_NIVEL) {
				
				// ... Y NO QUEDAN ENEMIGOS VIVOS
				if (!nivelActual->hayEnemigosVivos()) {
					estadoActual = VICTORIA;
				}
			}
		}
	}
}
void Juego::atacarConSanMartin() {
	if (estadoActual != JUGANDO || nivelActual == nullptr) return;
	
	SanMartin* heroe = nivelActual->getHeroe();
	if (!heroe) return;
	
	const std::vector<Entidad*>& entidades = nivelActual->getEntidades();
	
	for (Entidad* e : entidades) {
		if (e == heroe) continue;
		
		if (e->getTipo() == "REALISTA" && e->estaVivo()) {
			float dx = e->getX() - heroe->getX();
			float dy = e->getY() - heroe->getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist < 1.5f) {
				// Casteo y daño
				Personaje* p = static_cast<Personaje*>(e);
				p->recibirDanio(100.0f);
			}
		}
	}
}

void Juego::actualizarTextosConfig() {
	std::vector<std::string> opciones;
	opciones.push_back("Musica: " + std::to_string(volumenMusica));
	opciones.push_back("Sonidos: " + std::to_string(volumenSonidos));
	opciones.push_back("VOLVER");
	
	// Asignamos las nuevas opciones al menú
	menuConfig->setOpciones(opciones);
}

