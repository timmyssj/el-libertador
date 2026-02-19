#ifndef NIVEL_ESPANA_H
#define NIVEL_ESPANA_H

#include "Nivel.h"
#include <fstream>  // <--- IMPORTANTE: Para leer el archivo binario
#include <iostream> // <--- IMPORTANTE: Para mostrar mensajes en consola

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
		// --- 1. LECTURA DEL MAPA BINARIO BLINDADO ---
		std::ifstream archivoEntrada("nivel1.dat", std::ios::binary);
		
		if (archivoEntrada.is_open()) {
			// Si el archivo existe, leemos la matriz 'mapa' completa de un solo golpe
			archivoEntrada.read(reinterpret_cast<char*>(mapa), sizeof(mapa));
			archivoEntrada.close();
			std::cout << "[NIVEL] Mapa Espana cargado desde nivel1.dat" << std::endl;
		} 
		else {
			std::cerr << "[ERROR] No se encontro nivel1.dat. Cargando mapa por defecto." << std::endl;
			
			// PLAN B: Si no generaste el archivo aún o se borró, usamos tu código original
			inicializarMapaVacio();
			
			for(int y = 0; y < 20; y++) {
				if (y % 3 == 0) { mapa[y][2] = PARED; mapa[y][27] = PARED; }
				if (y % 4 == 0) { mapa[y][5] = PARED; mapa[y][24] = PARED; }
			}
			mapa[10][14] = PARED; mapa[10][15] = PARED;
			mapa[11][14] = PARED; mapa[11][15] = PARED;
			mapa[0][15] = SALIDA_NIVEL;
		}
		// --------------------------------------------
		
		// --- 2. POSICIONES EN EL NUEVO CAMPO DE BATALLA ---
		
		// San Martín entra por el sur (abajo, en el camino de tierra)
		referenciaHeroe = new SanMartin(15, 17);
		entidades.push_back(referenciaHeroe);
		
		// Enemigos (Patrulla Francesa emboscada en el claro central)
		entidades.push_back(new Enemigo(10, 10, referenciaHeroe));
		entidades.push_back(new Enemigo(19, 9, referenciaHeroe));
		entidades.push_back(new Enemigo(18, 12, referenciaHeroe));
		
		// El capitán de la guardia bloqueando el paso norte
		entidades.push_back(new Enemigo(15, 2, referenciaHeroe));
	}
};
#endif
