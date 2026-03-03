#ifndef NIVEL_SAN_LORENZO_H
#define NIVEL_SAN_LORENZO_H

#include "Nivel.h"
#include "Obstaculo.h"
#include "enemigo.h"   // <--- Tu clase Enemigo (Realista)
#include "Granadero.h" // <--- Tu clase Granadero
#include "Cabral.h"
#include <fstream>  
#include <iostream> 

class NivelSanLorenzo : public Nivel {
private:
	
	Cabral* sargentoCabral;
	bool eventoSacrificioOcurrido;
	
public:
	NivelSanLorenzo() {
		setCompletado(false); 
		eventoSacrificioOcurrido = false;
		
		// El pasto a las afueras del Convento de San Carlos
		setArchivoFondo("fondos/suelo_sanlorenzo.png"); 
		
		setTituloIntro("NIVEL 2: SAN LORENZO");
		addTextoIntro("3 de Febrero de 1813. Provincia de Santa Fe.");
		addTextoIntro("Una expedición realista desembarca cerca\n\ndel Convento de San Carlos.");
		addTextoIntro("El Coronel San Martín los espera oculto\n\ncon su nuevo regimiento:");
		addTextoIntro("Los Granaderos a Caballo.");
		addTextoIntro("Es su bautismo de fuego en territorio americano.");
		addTextoIntro("Misión: Rechazar el avance realista hacia el convento.");
		
		cargarContenido();
	}
	
	std::string getObjetivo() override { 
		return "OBJETIVO: Vence a los realistas y asegura la zona"; 
	}
	
	void cargarContenido() override {
		std::ifstream archivoEntrada("nivel2.dat", std::ios::binary);
		
		if (archivoEntrada.is_open()) {
			archivoEntrada.read(reinterpret_cast<char*>(getMapaPointer()), getMapaSize());
			archivoEntrada.close();
			std::cout << "[NIVEL] Mapa San Lorenzo cargado desde nivel2.dat" << std::endl;
		} else {
			std::cerr << "[ERROR] No se encontro nivel2.dat. Cargando mapa por defecto." << std::endl;
			inicializarMapaVacio();
			// Borde perimetral de rocas (3) para evitar crasheos
			for (int y = 0; y < 20; y++) {
				for (int x = 0; x < 30; x++) {
					if (y == 0 || y == 19 || x == 0 || x == 29) setCelda(x, y, 3);
				}
			}
			setCelda(15, 0, SALIDA_NIVEL);
		}
		
		// Sembrar obstáculos 3D
		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 30; x++) {
				if (getContenidoCelda(x, y) == 2) agregarEntidad(new Obstaculo(x, y, "ARBOL"));
				else if (getContenidoCelda(x, y) == 3) agregarEntidad(new Obstaculo(x, y, "ROCA"));
			}
		}
		
		// --- FORMACIÓN ALIADA (DESPLAZADA HACIA ARRIBA) ---
		// Movimos a San Martín del Y=15 al Y=10
		setHeroe(new SanMartin(15, 10, 2)); 
		agregarEntidad(getHeroe());
		
		// Cabral y el resto del escuadrón también suben 5 bloques
		sargentoCabral = new Cabral(15, 11, getHeroe());
		agregarEntidad(sargentoCabral);
		
		agregarEntidad(new Granadero(14, 11, getHeroe(), -1)); 
		agregarEntidad(new Granadero(16, 11, getHeroe(),  1)); 
		
		agregarEntidad(new Granadero(12, 12, getHeroe(), -2)); 
		agregarEntidad(new Granadero(18, 12, getHeroe(),  2)); 
		
		agregarEntidad(new Granadero(10, 13, getHeroe(), -3)); 
		agregarEntidad(new Granadero(20, 13, getHeroe(),  3)); 
		
		// Ahora los últimos nacen en Y=14, muy lejos de la barricada de abajo
		agregarEntidad(new Granadero(8,  14, getHeroe(), -4)); 
		agregarEntidad(new Granadero(22, 14, getHeroe(),  4));
		
		// --- FORMACIÓN ENEMIGA: LÍNEA DE DEFENSA REALISTA ---
		// Una barrera horizontal bloqueando el acceso al convento (Fila 7)
		for (int x = 8; x <= 22; x += 2) { // Un realista cada 2 bloques
			agregarEntidad(new Enemigo(x, 7, getHeroe()));
		}
		// Dos capitanes realistas custodiando las puertas del convento (Fila 5)
		agregarEntidad(new Enemigo(14, 5, getHeroe()));
		agregarEntidad(new Enemigo(16, 5, getHeroe()));
		
		// Nace el Convento (x=15 es el centro, y=4 es arriba)
		agregarEntidad(new Obstaculo(15, 4, "CONVENTO"));
		
		// --- Activar las colisiones para todos ---
		for (Entidad* e : getEntidades()) {
			Personaje* p = dynamic_cast<Personaje*>(e);
			if (p) p->setMapaEntidades(&getEntidades());
		}
	}
	
	void actualizar() override {
		// --- 1. SI ESTAMOS EN CINEMÁTICA, EL JUEGO SE CONGELA ---
		if (enCinematica()) return; 
		
		// 2. Actualizar tropas y héroe normalmente
		for (Entidad* e : getEntidades()) {
			if (e->estaVivo()) e->actualizar();
		}
		
		if (getHeroe() && getHeroe()->estaVivo()) {
			
			// --- 3. DETECTAR EL MOMENTO CRÍTICO ---
			if (!eventoSacrificioOcurrido && sargentoCabral && sargentoCabral->estaVivo()) {
				
				if (getHeroe()->getVida() <= getHeroe()->getVidaMax() * 0.40f) {
					// Disparamos la pantalla cinematica
					// Disparamos la pantalla cinematica con renglones más cortos
					setMensajeCinematica(
										 "¡El caballo de San Martin cae en combate!\n\n"
										 "El sargento Juan Bautista Cabral se arroja\n"
										 "sobre las bayonetas enemigas para\n"
										 "salvar a su comandante.\n\n"
										 "Sus ultimas palabras resuenan:\n\n"
										 "'¡Muero contento, hemos batido al enemigo!'"
										 );
					
					// Cabral se mueve sobre San Martín y da su vida
					sargentoCabral->setX(getHeroe()->getX());
					sargentoCabral->setY(getHeroe()->getY());
					sargentoCabral->darLaVida(); 
					getHeroe()->curarCompleto(); 
					
					eventoSacrificioOcurrido = true;
				}
			}
			
			int x = (int)getHeroe()->getX();
			int y = (int)getHeroe()->getY();
			if (x >= 0 && x < 30 && y >= 0 && y < 20) {
				if (getContenidoCelda(x, y) == SALIDA_NIVEL) setCompletado(true);
			}
		}
	}
};

#endif
