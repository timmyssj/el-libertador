#ifndef NIVEL_H
#define NIVEL_H

#include "Entidad.h"
#include "SanMartin.h"
#include <vector>
#include <string>

const int SUELO = 0;
const int PARED = 1;
const int SALIDA_NIVEL = 4;

class Nivel {
private: 
	int mapa[20][30];
	std::string tituloIntro;
	std::vector<std::string> textoIntro;
	std::vector<Entidad*> entidades;
	SanMartin* referenciaHeroe;
	bool completado; 
	
	// --- NUEVO: RUTA DEL FONDO ---
	std::string archivoFondo;
	
public:
	Nivel();
	virtual ~Nivel();
	void inicializarMapaVacio();
	int getContenidoCelda(int x, int y);
	SanMartin* getHeroe();
	const std::vector<Entidad*>& getEntidades();
	bool hayEnemigosVivos();
	bool esCeldaOcupada(int x, int y);
	
	virtual void cargarContenido() = 0;
	virtual void actualizar() = 0;
	virtual std::string getObjetivo() { return "Objetivo desconocido"; }
	
	void setCelda(int x, int y, int valor) { mapa[y][x] = valor; }
	void* getMapaPointer() { return mapa; } 
	int getMapaSize() const { return sizeof(mapa); }
	
	void setTituloIntro(const std::string& t) { tituloIntro = t; }
	std::string getTituloIntro() const { return tituloIntro; }
	
	void addTextoIntro(const std::string& t) { textoIntro.push_back(t); }
	std::vector<std::string> getTextoIntro() const { return textoIntro; }
	
	// --- NUEVOS SETTERS Y GETTERS PARA EL FONDO ---
	void setArchivoFondo(const std::string& arch) { archivoFondo = arch; }
	std::string getArchivoFondo() const { return archivoFondo; }
	
	void agregarEntidad(Entidad* e) { entidades.push_back(e); }
	void setHeroe(SanMartin* h) { referenciaHeroe = h; }
	
	bool estaCompletado() const { return completado; }
	void setCompletado(bool c) { completado = c; }
};

#endif
