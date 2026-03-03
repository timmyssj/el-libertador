#include "Nivel.h"

// CONSTRUCTOR (Solo una vez)
Nivel::Nivel() {
	referenciaHeroe = nullptr;
	tituloIntro = "CONTEXTO HISTORICO"; 
}

Nivel::~Nivel() {
	for (Entidad* e : entidades) {
		delete e;
	}
	entidades.clear();
}

void Nivel::inicializarMapaVacio() {
	for(int y = 0; y < 20; y++) {
		for(int x = 0; x < 30; x++) {
			mapa[y][x] = SUELO;
		}
	}
	for(int i = 0; i < 20; i++) { mapa[i][0] = PARED; mapa[i][29] = PARED; }
	for(int j = 0; j < 30; j++) { mapa[0][j] = PARED; mapa[19][j] = PARED; }
}

void Nivel::actualizar() {
	for (size_t i = 0; i < entidades.size(); i++) {
		Entidad* e = entidades[i];
		
		if (e->estaVivo()) {
			e->actualizar();
		} 
		else if (e->getTipo() != "PROCER") { 
			delete e; 
			entidades.erase(entidades.begin() + i);
			i--; 
		}
	}
}

int Nivel::getContenidoCelda(int x, int y) {
	if (x < 0 || x >= 30 || y < 0 || y >= 20) return PARED; 
	return mapa[y][x];
}

SanMartin* Nivel::getHeroe() { return referenciaHeroe; }

const std::vector<Entidad*>& Nivel::getEntidades() { return entidades; }

bool Nivel::hayEnemigosVivos() {
	for (Entidad* e : entidades) {
		// Si hay un Realista O un Muñeco de Práctica vivo, devuelve true
		if ((e->getTipo() == "REALISTA" || e->getTipo() == "PRACTICA") && e->estaVivo()) {
			return true;
		}
	}
	return false; // Solo devuelve false si limpiaste todo
}
bool Nivel::esCeldaOcupada(int x, int y) {
	// ... (Acá arriba seguramente tenés los chequeos de los bordes del mapa o celdas de pared) ...
	
	for (Entidad* e : entidades) {
		if (e->estaVivo() && (int)e->getX() == x && (int)e->getY() == y) {
			
			// --- ¡LA MAGIA QUE FALTA ESTÁ ACÁ! ---
			// Le decimos al motor que si la entidad empieza con la palabra "ITEM", sea un fantasma atravesable
			if (e->getTipo() == "ITEM_SABLE" || e->getTipo() == "ITEM_CURACION") {
				continue; // Ignora esta entidad y deja que San Martín la pise
			}
			// ------------------------------------
			
			return false; // Si llegamos acá, chocaste con un árbol o enemigo
		}
	}
	return true;
}

