#ifndef JUEGO_H
#define JUEGO_H

// Fíjate que NO incluimos <SFML/Graphics.hpp> aquí.
// El Modelo no sabe que existen los gráficos.

class Juego {
private:
	bool enPausa;
	// Aquí irán: Nivel* nivelActual;
	// Aquí irán: SanMartin* jugador;
	
public:
	Juego() : enPausa(false) {
		// Inicializar variables de lógica (vidas = 3, score = 0)
	}
	
	// Este método se llamará 60 veces por segundo
	void actualizar() {
		if (enPausa) return;
		
		// Aquí luego pondrás: jugador->mover();
		// nivel->actualizarEnemigos();
	}
	
	// Métodos para que la Vista pregunte cosas (Getters)
	bool estaEnPausa() const { return enPausa; }
	
	// Métodos para que la Vista ordene cosas (Inputs)
	void pausarOReanudar() { enPausa = !enPausa; }
	void inputMoverJugador(int dx, int dy) {
		// Aquí validaremos con la matriz del Nivel si se puede mover
	}
};

#endif
