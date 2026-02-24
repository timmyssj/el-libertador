#include "juego.h"
#include "NivelTutorial.h"
#include "Frances.h"
#include "NivelEspana.h"
#include <iostream>
#include <cmath>
#include <fstream> 

Juego::Juego() {
	estadoActual = EN_MENU;
	estadoAnterior = EN_MENU;
	
	volumenMusica = 50;
	volumenSonidos = 50;
	
	menuPrincipal = new Menu({"NUEVA PARTIDA", "CONTINUAR", "CONFIGURACION", "SALIR"});
	menuPausa = new Menu({"CONTINUAR", "CONFIGURACION", "SALIR AL MENU"});
	
	nivelMaximoDesbloqueado = 0; 
	nivelJugandoId = 0; 
	
	menuNiveles = new Menu({"0. TUTORIAL", "1. BATALLA EN ESPANA", "VOLVER"});
	
	menuConfig = new Menu({"Musica", "Sonidos", "Volver"}); 
	actualizarTextosConfig(); 
	
	nivelActual = nullptr;
}

Juego::~Juego() {
	delete menuPrincipal;
	delete menuPausa;
	delete menuConfig; 
	delete menuNiveles; 
	if (nivelActual) delete nivelActual;
}

// --- ARCHIVOS BINARIOS ---

void Juego::guardarProgreso() {
	std::ofstream archivo("progreso.dat", std::ios::binary);
	if (archivo.is_open()) {
		archivo.write(reinterpret_cast<char*>(&nivelMaximoDesbloqueado), sizeof(nivelMaximoDesbloqueado));
		archivo.close();
		std::cout << "[INFO] Partida guardada (Binario): Nivel " << nivelMaximoDesbloqueado << std::endl;
	} else {
		std::cout << "[ERROR] No se pudo crear progreso.dat" << std::endl;
	}
}

void Juego::cargarProgreso() {
	std::ifstream archivo("progreso.dat", std::ios::binary);
	if (archivo.is_open()) {
		archivo.read(reinterpret_cast<char*>(&nivelMaximoDesbloqueado), sizeof(nivelMaximoDesbloqueado));
		archivo.close();
		std::cout << "[INFO] Partida cargada (Binario): Nivel " << nivelMaximoDesbloqueado << std::endl;
	} else {
		std::cout << "[INFO] No hay archivo .dat. Empezando nueva partida." << std::endl;
		nivelMaximoDesbloqueado = 0;
	}
}

// -------------------------

void Juego::procesarTeclaArriba() {
	if (estadoActual == EN_MENU) menuPrincipal->moverArriba();
	else if (estadoActual == SELECCION_NIVEL) menuNiveles->moverArriba();
	else if (estadoActual == PAUSA) menuPausa->moverArriba();
	else if (estadoActual == CONFIGURACION) menuConfig->moverArriba();
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		if (nivelActual->estaCompletado()) return; 
		
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) heroe->moverse(0, -1);
	}
}

void Juego::procesarTeclaAbajo() {
	if (estadoActual == EN_MENU) menuPrincipal->moverAbajo();
	else if (estadoActual == SELECCION_NIVEL) menuNiveles->moverAbajo();
	else if (estadoActual == PAUSA) menuPausa->moverAbajo();
	else if (estadoActual == CONFIGURACION) menuConfig->moverAbajo();
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		if (nivelActual->estaCompletado()) return;
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) heroe->moverse(0, 1);
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
		if (nivelActual->estaCompletado()) return;
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) heroe->moverse(-1, 0);
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
		if (nivelActual->estaCompletado()) return;
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) heroe->moverse(1, 0);
	}
}

void Juego::procesarTeclaEnter() {
	if (estadoActual == EN_MENU) {
		int op = menuPrincipal->getOpcionActual();
		if (op == 0) { 
			nivelMaximoDesbloqueado = 0; 
			guardarProgreso(); 
			estadoActual = SELECCION_NIVEL;
			menuNiveles->reiniciarCursor();
		}
		else if (op == 1) { 
			cargarProgreso(); 
			estadoActual = SELECCION_NIVEL;
			menuNiveles->reiniciarCursor();
		}
		else if (op == 2) { 
			estadoActual = CONFIGURACION;
			menuConfig->reiniciarCursor();
		}
		else if (op == 3) {
			estadoActual = SALIR;
		}
	}
	else if (estadoActual == SELECCION_NIVEL) {
		int op = menuNiveles->getOpcionActual();
		
		if (op == menuNiveles->getCantidadOpciones() - 1) {
			estadoActual = EN_MENU;
			return;
		}
		
		if (op <= nivelMaximoDesbloqueado) {
			if (nivelActual) delete nivelActual;
			
			if (op == 0) nivelActual = new NivelTutorial();
			else if (op == 1) nivelActual = new NivelEspana(); 
			
			nivelJugandoId = op; 
			
			prepararNivel(nivelActual);
			
		} else {
			std::cout << "Nivel Bloqueado!" << std::endl;
		}
	}
	else if (estadoActual == INTRO_HISTORIA) {
		paginaHistoriaActual++;
		if (paginaHistoriaActual >= (int)lineasHistoria.size()) {
			estadoActual = JUGANDO;
		}
	}
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
	else if (estadoActual == CONFIGURACION) {
		int op = menuConfig->getOpcionActual();
		if (op == 2) { 
			estadoActual = estadoAnterior; 
		}
	}
	else if (estadoActual == JUGANDO && nivelActual != nullptr) {
		if (nivelActual->estaCompletado()) {
			if (nivelJugandoId >= nivelMaximoDesbloqueado) { 
				nivelMaximoDesbloqueado = nivelJugandoId + 1;
				guardarProgreso(); 
			}
			delete nivelActual; 
			nivelActual = nullptr;
			estadoActual = SELECCION_NIVEL; 
			menuNiveles->reiniciarCursor();
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
		if (nivelActual->estaCompletado()) return; 
		
		SanMartin* heroe = nivelActual->getHeroe();
		if (heroe) heroe->resetearMovimiento();
		
		nivelActual->actualizar();
		
		if (heroe && !heroe->estaVivo()) {
			estadoActual = GAME_OVER;
		}
	}
}

void Juego::atacarConSanMartin() {
	if (estadoActual != JUGANDO || nivelActual == nullptr) return;
	if (nivelActual->estaCompletado()) return; 
	
	SanMartin* heroe = nivelActual->getHeroe();
	if (!heroe) return;
	
	heroe->atacar(nivelActual->getEntidades());
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
