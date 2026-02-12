#ifndef NIVEL_ESPANA_H
#define NIVEL_ESPANA_H

#include "Nivel.h"

class NivelEspana : public Nivel {
public:
	NivelEspana() {
		tituloIntro = "NIVEL 1: BATALLA EN ESPANA";
		
		textoIntro.push_back("Ano 1808. Arjonilla, Espana.");
		textoIntro.push_back("Las tropas de Napoleon invaden la peninsula.");
		textoIntro.push_back("El joven Capitan San Martin lucha junto\nal ejercito espanol.");
		textoIntro.push_back("Mision: Emboscar a la patrulla francesa.");
		
		cargarContenido();
	}
	
	void cargarContenido() override {
		inicializarMapaVacio();
		
		// 1. AMBIENTACIÓN: CAMINO ÁRIDO
		// Hacemos un camino central ancho con rocas a los lados
		for(int y = 0; y < 20; y++) {
			// Rocas dispersas (simulando terreno difícil)
			if (y % 3 == 0) { mapa[y][2] = PARED; mapa[y][27] = PARED; }
			if (y % 4 == 0) { mapa[y][5] = PARED; mapa[y][24] = PARED; }
		}
		
		// 2. OBSTÁCULOS CENTRALES (Carruaje volcado o ruinas)
		mapa[10][14] = PARED; mapa[10][15] = PARED;
		mapa[11][14] = PARED; mapa[11][15] = PARED;
		
		// 3. SALIDA (Norte)
		mapa[0][15] = SALIDA_NIVEL;
		
		// 4. EL HÉROE (Sur)
		referenciaHeroe = new SanMartin(15, 18);
		entidades.push_back(referenciaHeroe);
		
		// 5. ENEMIGOS (Soldados Franceses)
		// Están patrullando el norte
		entidades.push_back(new Enemigo(5, 5, referenciaHeroe));
		entidades.push_back(new Enemigo(20, 5, referenciaHeroe));
		entidades.push_back(new Enemigo(12, 10, referenciaHeroe));
		
		// Un enemigo guardia cerca de la salida
		entidades.push_back(new Enemigo(15, 2, referenciaHeroe));
	}
};

#endif
