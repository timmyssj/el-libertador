#ifndef NIVEL_H
#define NIVEL_H

#include <vector>
#include "Entidad.h"
#include "SanMartin.h"
#include "Enemigo.h"

// Definiciones de tipos de celda
const int SUELO = 0;
const int PARED = 1;
const int AGUA = 2;
const int SALIDA_NIVEL = 4;

class Nivel {
protected: // <--- CAMBIO CLAVE: 'protected' permite que los hijos accedan
	int mapa[20][30];
	std::vector<Entidad*> entidades;
	SanMartin* referenciaHeroe; 
	std::vector<std::string> textoIntro;
	
public:
	Nivel();
	virtual ~Nivel(); // Destructor virtual es OBLIGATORIO en herencia
	
	// Método Virtual Puro: Obligamos a los hijos a definir cómo son
	virtual void cargarContenido() = 0; 
	
	// Métodos comunes (La física no cambia entre niveles)
	void actualizar();
	int getContenidoCelda(int x, int y);
	SanMartin* getHeroe();
	const std::vector<Entidad*>& getEntidades();
	bool hayEnemigosVivos();
	
	// Helper para los hijos: limpia el mapa a "todo suelo" con bordes
	void inicializarMapaVacio(); 
	
	// Método para que Juego pueda leer la historia
	std::vector<std::string> getTextoIntro() { return textoIntro; }
};

#endif
