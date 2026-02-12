#include "juego.h"
#include "NivelTutorial.h"
#include "enemigo.h"
#include "NivelEspana.h" // Asegúrate de tener este archivo o comenta esta línea si aún no lo creaste
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string> 

// 1. CONSTRUCTOR
Juego::Juego() {
	estadoActual = EN_MENU;
	estadoAnterior = EN_MENU;
	
	volumenMusica = 50;
	volumenSonidos = 50;
	
	// Opciones iniciales
	menuPrincipal = new Menu({"NUEVA PARTIDA", "CONTINUAR", "CONFIGURACION", "SALIR"});
	menuPausa = new Menu({"CONTINUAR", "CONFIGURACION", "SALIR AL MENU"});
	
	nivelMaximoDesbloqueado = 0; 
	
	// Menú de Niveles
	menuNiveles = new Menu({"0. TUTORIAL", "1. BATALLA EN ESPANA", "VOLVER"});
	
	// Menú Configuración
	menuConfig = new Menu({"Musica", "Sonidos", "Volver"}); 
	actualizarTextosConfig(); 
	
	nivelActual = nullptr;
}

// 2. DESTRUCTOR
Juego::~Juego() {
	delete menuPrincipal;
	delete menuPausa;
	delete menuConfig; 
	delete menuNiveles; 
	if (nivelActual) delete nivelActual;
}

// --- PROCESAMIENTO DE TECLAS ---

void Juego::procesarTeclaArriba() {
	if (estadoActual == EN_MENU) menuPrincipal->moverArriba();
	else if (estadoActual == SELECCION_NIVEL) menuNiveles->moverArriba();
	else if (estadoActual == PAUSA) menuPausa->moverArriba();
	else if (estadoActual == CONFIGURACION) menuConfig->moverArriba();
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX();
			int ty = (int)heroe->getY() - 1; 
			
			if (nivelActual->getContenidoCelda(tx, ty) != PARED 
				&& !nivelActual->esCeldaOcupada(tx, ty)) { 
				heroe->moverse(0, -1);
			}
		}
	}
}

void Juego::procesarTeclaAbajo() {
	if (estadoActual == EN_MENU) menuPrincipal->moverAbajo();
	else if (estadoActual == SELECCION_NIVEL) menuNiveles->moverAbajo();
	else if (estadoActual == PAUSA) menuPausa->moverAbajo();
	else if (estadoActual == CONFIGURACION) menuConfig->moverAbajo();
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX();
			int ty = (int)heroe->getY() + 1; 
			
			if (nivelActual->getContenidoCelda(tx, ty) != PARED 
				&& !nivelActual->esCeldaOcupada(tx, ty)) { 
				heroe->moverse(0, 1);
			}
		}
	}
}

void Juego::procesarTeclaIzquierda() {
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
		actualizarTextosConfig(); 
	}
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX() - 1;
			int ty = (int)heroe->getY(); 
			
			if (nivelActual->getContenidoCelda(tx, ty) != PARED 
				&& !nivelActual->esCeldaOcupada(tx, ty)) { 
				heroe->moverse(-1, 0);
			}
		}
	}
}

void Juego::procesarTeclaDerecha() {
	if (estadoActual == CONFIGURACION) {
		int op = menuConfig->getOpcionActual();
		if (op == 0) { 
			volumenMusica += 10;
			if (volumenMusica > 100) volumenMusica = 100;
		}
		else if (op == 1) { 
			volumenSonidos += 10;
			if (volumenSonidos > 100) volumenSonidos = 100;
		}
		actualizarTextosConfig();
	}
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) {
			int tx = (int)heroe->getX() + 1;
			int ty = (int)heroe->getY();
			
			if (nivelActual->getContenidoCelda(tx, ty) != PARED 
				&& !nivelActual->esCeldaOcupada(tx, ty)) { 
				heroe->moverse(1, 0);
			}
		}
	}
}

void Juego::procesarTeclaEnter() {
	// --- CASO 1: MENU PRINCIPAL ---
	if (estadoActual == EN_MENU) {
		int op = menuPrincipal->getOpcionActual();
		
		if (op == 0) { // NUEVA PARTIDA
			nivelMaximoDesbloqueado = 0; // Reseteamos progreso
			guardarProgreso(); // Sobrescribimos el archivo
			
			estadoActual = SELECCION_NIVEL;
			menuNiveles->reiniciarCursor();
		}
		else if (op == 1) { // Opción: CONTINUAR CAMPAÑA
			cargarProgreso(); // <--- IMPORTANTE
			
			estadoActual = SELECCION_NIVEL;
			menuNiveles->reiniciarCursor();
		}
		else if (op == 2) { // CONFIGURACION
			estadoActual = CONFIGURACION;
			menuConfig->reiniciarCursor();
		}
		else if (op == 3) { // SALIR
			estadoActual = SALIR;
		}
	}
	// --- CASO 2: SELECCION DE NIVEL ---
	else if (estadoActual == SELECCION_NIVEL) {
		int op = menuNiveles->getOpcionActual();
		
		if (op == 2) { // VOLVER
			estadoActual = EN_MENU;
			return;
		}
		
		if (op <= nivelMaximoDesbloqueado) {
			if (nivelActual) delete nivelActual;
			
			if (op == 0) nivelActual = new NivelTutorial();
			else if (op == 1) nivelActual = new NivelEspana(); 
			
			prepararNivel(nivelActual);
			
		} else {
			std::cout << "Nivel Bloqueado!" << std::endl;
		}
	}
	// --- CASO 3: HISTORIA ---
	else if (estadoActual == INTRO_HISTORIA) {
		paginaHistoriaActual++;
		if (paginaHistoriaActual >= (int)lineasHistoria.size()) {
			estadoActual = JUGANDO;
		}
	}
	// --- CASO 4: PAUSA ---
	else if (estadoActual == PAUSA) {
		int op = menuPausa->getOpcionActual();
		if (op == 0) { estadoActual = JUGANDO; }
		else if (op == 1) { 
			estadoAnterior = PAUSA;
			estadoActual = CONFIGURACION;
			menuConfig->reiniciarCursor();
		}
		else if (op == 2) { 
			if (nivelActual) { delete nivelActual; nivelActual = nullptr; }
			estadoActual = EN_MENU;
			menuPrincipal->reiniciarCursor();
		}
	}
	// --- CASO 5: CONFIGURACION ---
	else if (estadoActual == CONFIGURACION) {
		int op = menuConfig->getOpcionActual();
		if (op == 2) { // VOLVER
			estadoActual = estadoAnterior; 
		}
	}
}

void Juego::teclaEscape() {
	if (estadoActual == JUGANDO) {
		estadoActual = PAUSA;
		menuPausa->reiniciarCursor();
	}
	else if (estadoActual == PAUSA) {
		estadoActual = JUGANDO; 
	}
	else if (estadoActual == GAME_OVER || estadoActual == VICTORIA) {
		if (nivelActual) {
			delete nivelActual;
			nivelActual = nullptr;
		}
		estadoActual = EN_MENU;
	}
	else if (estadoActual == CONFIGURACION) {
		estadoActual = estadoAnterior;
	}
}

void Juego::actualizar() {
	if (estadoActual == JUGANDO && nivelActual != nullptr) {
		nivelActual->actualizar();
		SanMartin* heroe = nivelActual->getHeroe();
		
		// Derrota
		if (!heroe->estaVivo()) {
			estadoActual = GAME_OVER;
		}
		
		// Combate Enemigo -> Heroe
		const std::vector<Entidad*>& entidades = nivelActual->getEntidades();
		for (Entidad* e : entidades) {
			if (e->getTipo() == "REALISTA" && e->estaVivo()) {
				float dx = e->getX() - heroe->getX();
				float dy = e->getY() - heroe->getY();
				float dist = std::sqrt(dx*dx + dy*dy);
				
				if (dist < 1.0f) {
					Enemigo* realista = static_cast<Enemigo*>(e);
					if (realista->intentarAtacar()) {
						heroe->recibirDanio(10); 
					}
				}
			}
		}
		
		// Victoria
		int x = (int)heroe->getX();
		int y = (int)heroe->getY();
		
		if (nivelActual->getContenidoCelda(x, y) == SALIDA_NIVEL) {
			
			if (!nivelActual->hayEnemigosVivos()) {
				estadoActual = VICTORIA;
				
				// LÓGICA DE DESBLOQUEO SIMPLIFICADA
				// 1. Obtenemos qué nivel estamos jugando (Truco sucio pero efectivo: por título)
				std::string titulo = nivelActual->getTituloIntro();
				
				int nivelRecienCompletado = -1;
				
				// Identificamos el ID del nivel actual
				if (titulo == "ENTRENAMIENTO BASICO") nivelRecienCompletado = 0;
				else if (titulo == "NIVEL 1: BATALLA EN ESPANA") nivelRecienCompletado = 1;
				// else if (titulo == "SAN LORENZO") nivelRecienCompletado = 2;
				
				// 2. Si ganamos un nivel que es igual al máximo que teníamos, desbloqueamos el siguiente
				if (nivelRecienCompletado >= nivelMaximoDesbloqueado) {
					nivelMaximoDesbloqueado = nivelRecienCompletado + 1;
					
					// --- AQUÍ GUARDAMOS ---
					guardarProgreso(); 
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
		
		if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA") && e->estaVivo()) {
			float dx = e->getX() - heroe->getX();
			float dy = e->getY() - heroe->getY();
			float dist = std::sqrt(dx*dx + dy*dy);
			
			if (dist < 1.5f) {
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
	menuConfig->setOpciones(opciones);
}

void Juego::prepararNivel(Nivel* nuevoNivel) {
	nivelActual = nuevoNivel;
	lineasHistoria = nivelActual->getTextoIntro();
	paginaHistoriaActual = 0;
	tituloActual = nivelActual->getTituloIntro(); 
	
	if (!lineasHistoria.empty()) {
		estadoActual = INTRO_HISTORIA;
	} else {
		estadoActual = JUGANDO;
	}
}
void Juego::guardarProgreso() {
	std::ofstream archivo("progreso.txt"); // Usamos .txt para que puedas verlo fácil
	if (archivo.is_open()) {
		archivo << nivelMaximoDesbloqueado;
		archivo.close();
		std::cout << "--- PROGRESO GUARDADO: Nivel " << nivelMaximoDesbloqueado << " ---" << std::endl;
	} else {
		std::cout << "ERROR: No se pudo crear el archivo de guardado." << std::endl;
	}
}

void Juego::cargarProgreso() {
	std::ifstream archivo("progreso.txt");
	if (archivo.is_open()) {
		archivo >> nivelMaximoDesbloqueado;
		archivo.close();
		std::cout << "--- PROGRESO CARGADO: Nivel " << nivelMaximoDesbloqueado << " ---" << std::endl;
	} else {
		std::cout << "ARCHIVO NO ENCONTRADO. Empezando de 0." << std::endl;
		nivelMaximoDesbloqueado = 0;
	}
}
