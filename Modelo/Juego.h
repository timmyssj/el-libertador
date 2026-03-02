#ifndef JUEGO_H
#define JUEGO_H

#include "menu.h"
#include "Nivel.h"
#include <vector>
#include <string>
#include <SFML/Audio.hpp>

enum EstadoJuego {
	EN_MENU,
	SELECCION_NIVEL,
	INTRO_HISTORIA,
	JUGANDO,
	PAUSA,
	CONFIGURACION,
	GAME_OVER,
	VICTORIA,
	SALIR
};

class Juego {
private:
	EstadoJuego estadoActual;
	EstadoJuego estadoAnterior;
	
	int volumenMusica;
	int volumenSonidos;
	
	Menu* menuPrincipal;
	Menu* menuPausa;
	Menu* menuConfig;
	Menu* menuNiveles;
	
	Nivel* nivelActual;
	
	int nivelMaximoDesbloqueado;
	int nivelJugandoId; 
	
	std::vector<std::string> lineasHistoria;
	int paginaHistoriaActual;
	std::string tituloActual;
	
	sf::SoundBuffer bufferAtaque;
	sf::Sound sonidoAtaque;
	
	sf::SoundBuffer bufferArenga;
	sf::Sound sonidoArenga;
	
public:
	Juego();
	~Juego();
	
	void procesarTeclaArriba();
	void procesarTeclaAbajo();
	void procesarTeclaIzquierda();
	void procesarTeclaDerecha();
	void procesarTeclaEnter();
	void teclaEscape();
	void atacarConSanMartin();
	
	void actualizar();
	void prepararNivel(Nivel* nuevoNivel);
	void actualizarTextosConfig();
	
	void guardarProgreso();
	void cargarProgreso();
	void desbloquearSiguienteNivel() { nivelMaximoDesbloqueado++; }
	
	EstadoJuego getEstado() { return estadoActual; }
	Menu* getMenu() { return menuPrincipal; }
	Menu* getMenuPausa() { return menuPausa; }
	Menu* getMenuConfig() { return menuConfig; }
	Menu* getMenuNiveles() { return menuNiveles; }
	Nivel* getNivelActual() { return nivelActual; }
	int getNivelMaximo() { return nivelMaximoDesbloqueado; }
	
	std::string getTextoHistoria() { return lineasHistoria[paginaHistoriaActual]; }
	bool esUltimaPaginaIntro() { return paginaHistoriaActual == (int)lineasHistoria.size() - 1; }
	std::string getTituloActual() { return tituloActual; }
};

#endif
