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
	
	void setArchivoFondo(const std::string& arch) { archivoFondo = arch; }
	std::string getArchivoFondo() const { return archivoFondo; }
	
	void agregarEntidad(Entidad* e) { entidades.push_back(e); }
	void setHeroe(SanMartin* h) { referenciaHeroe = h; }
	
	bool estaCompletado() const { return completado; }
	void setCompletado(bool c) { completado = c; }
	
	// --- NUEVO: Verifica si la celda es caminable para el Héroe ---
	// ¡Ahora sí está en el Nivel, donde puede ver el mapa y las entidades!
	bool esCeldaLibreParaHeroe(int x, int y) {
		int celda = getContenidoCelda(x, y);
		
		// 1. Si hay un muro, árbol o roca, no puede pasar
		if (celda == 1 || celda == 2 || celda == 3) return false; 
		
		// 2. Si hay un enemigo, bloquea el paso (Los enemigos son sólidos)
		for (Entidad* e : entidades) {
			if (e->estaVivo() && (int)e->getX() == x && (int)e->getY() == y) {
				if (e->getTipo() == "FRANCES" || e->getTipo() == "REALISTA") {
					return false; 
				}
			}
		}
		
		// 3. Si está vacía o hay un Aliado/Granadero, ¡vía libre!
		return true; 
	}
	// --- NUEVO: SISTEMA DE CINEMÁTICAS IN-GAME ---
	std::string mensajeCinematica = "";
	void setMensajeCinematica(std::string m) { mensajeCinematica = m; }
	std::string getMensajeCinematica() { return mensajeCinematica; }
	void limpiarCinematica() { mensajeCinematica = ""; }
	bool enCinematica() { return mensajeCinematica != ""; }
	
	// --- NUEVO: RELOJ DEL HUD ---
	// Devuelve -1 si el nivel es normal. Los niveles con tiempo (como Andes) la sobrescribirán.
	virtual int getTiempoRestante() { return -1; }
};

#endif
