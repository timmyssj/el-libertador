#ifndef NIVEL_H
#define NIVEL_H

#include "Entidad.h"
#include "SanMartin.h"
#include <vector>
#include <string>

// --- CONSTANTES GLOBALES (Como las tenías originalmente) ---
const int SUELO = 0;
const int PARED = 1;
const int SALIDA_NIVEL = 4;

class Nivel {
private: // --- ¡ESTRICTAMENTE PRIVADO! Reto aceptado ---
	int mapa[20][30];
	std::string tituloIntro;
	std::vector<std::string> textoIntro;
	std::vector<Entidad*> entidades;
	SanMartin* referenciaHeroe;
	bool completado; 
	
public:
	// --- DECLARACIONES (Se implementan en tu nivel.cpp) ---
	Nivel();
	virtual ~Nivel();
	void inicializarMapaVacio();
	int getContenidoCelda(int x, int y);
	SanMartin* getHeroe();
	const std::vector<Entidad*>& getEntidades();
	bool hayEnemigosVivos();
	bool esCeldaOcupada(int x, int y);
	
	// Métodos virtuales puros para los hijos
	virtual void cargarContenido() = 0;
	virtual void actualizar() = 0;
	virtual std::string getObjetivo() { return "Objetivo desconocido"; }
	
	// --- NUEVOS SETTERS / GETTERS (Para que los hijos no rompan nada) ---
	
	// Para construir el Mapa
	void setCelda(int x, int y, int valor) { mapa[y][x] = valor; }
	void* getMapaPointer() { return mapa; } // Necesario para leer el archivo binario
	int getMapaSize() const { return sizeof(mapa); }
	
	// Para los Textos
	void setTituloIntro(const std::string& t) { tituloIntro = t; }
	std::string getTituloIntro() const { return tituloIntro; }
	
	void addTextoIntro(const std::string& t) { textoIntro.push_back(t); }
	std::vector<std::string> getTextoIntro() const { return textoIntro; }
	
	// Para los Personajes
	void agregarEntidad(Entidad* e) { entidades.push_back(e); }
	void setHeroe(SanMartin* h) { referenciaHeroe = h; }
	
	// Para la Victoria
	bool estaCompletado() const { return completado; }
	void setCompletado(bool c) { completado = c; }
};

#endif
