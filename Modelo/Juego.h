#ifndef JUEGO_H
#define JUEGO_H

#include "menu.h"  // <--- Necesitamos esto para que funcione Menu*
#include "nivel.h"

enum EstadoJuego {
	EN_MENU,
	INTRO_HISTORIA,
	JUGANDO,
	SALIR,
	PAUSA,
	CONFIGURACION,
	GAME_OVER,
	VICTORIA
};

class Juego {
private:
	EstadoJuego estadoActual;
	EstadoJuego estadoAnterior;
	
	// Módulos: Juego posee un Menú y un Nivel
	Menu* menuPrincipal;   
	Menu* menuPausa;
	Menu* menuConfig;
	Nivel* nivelActual;     
	
	//Sonidos
	int volumenMusica, volumenSonidos;
	
	std::vector<std::string> lineasHistoria;
	int paginaHistoriaActual;
	std::string tituloActual;
	
	
public:
	Juego();
	~Juego();
	
	// --- INPUT DISTRIBUIDO ---
	void procesarTeclaArriba();
	void procesarTeclaAbajo();
	void procesarTeclaEnter();
	
	//Ataque de SanMartin
	void atacarConSanMartin();
	
	// --- NUEVO: MOVIMIENTO LATERAL ---
	void procesarTeclaIzquierda();
	void procesarTeclaDerecha();
	
	// Función de ESCAPE (Pausa o Volver)
	void teclaEscape();
	
	// --- LÓGICA ---
	void actualizar(); 
	
	// --- GETTERS ---
	// La Vista llamará a estos para saber qué dibujar
	EstadoJuego getEstado() const { return estadoActual; }
	
	// Si estamos en menú, la vista pedirá este puntero
	Menu* getMenu() const { return menuPrincipal; }
	
	// Si estamos jugando, la vista pedirá este puntero
	Nivel* getNivelActual() { return nivelActual; }
	
	// Getter para que la vista sepa cuál dibujar
	Menu* getMenuPausa() const { return menuPausa; }
	
	// Getter para la vista
	Menu* getMenuConfig() const { return menuConfig; }
	
	// Helper para actualizar los textos cuando subes/bajas volumen
	void actualizarTextosConfig();
	
	// Método auxiliar para iniciar cualquier nivel con historia
	void prepararNivel(Nivel* nuevoNivel);
	
	//getter simple
	std::string getTextoHistoria() const {
		if (paginaHistoriaActual >= 0 && paginaHistoriaActual < (int)lineasHistoria.size()) {
			return lineasHistoria[paginaHistoriaActual];
		}
		return "";
	}
	
	//sabemos si es la última página de texto
	bool esUltimaPaginaIntro() const {
		return paginaHistoriaActual >= (int)lineasHistoria.size() - 1;
	}
	//titulo 
	std::string getTituloActual() const { return tituloActual; }
};

#endif
