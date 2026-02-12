#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

class Menu {
private:
	std::vector<std::string> opciones;
	int indiceSeleccionado;
	
public:
	// Constructor
	Menu(std::vector<std::string> listaOpciones) {
		opciones = listaOpciones;
		indiceSeleccionado = 0;
	}
	
	// --- CORRECCIÓN 1: SETTER ---
	void setOpciones(const std::vector<std::string>& nuevasOpciones) {
		opciones = nuevasOpciones;
		indiceSeleccionado = 0; // Reiniciamos el cursor usando la variable correcta
	}
	
	// --- CORRECCIÓN 2: GETTER (Faltaba esto para que la Ventana funcione) ---
	std::vector<std::string> getOpciones() const {
		return opciones;
	}
	
	void moverArriba() {
		indiceSeleccionado--;
		if (indiceSeleccionado < 0) indiceSeleccionado = opciones.size() - 1;
	}
	
	void moverAbajo() {
		indiceSeleccionado++;
		if (indiceSeleccionado >= (int)opciones.size()) indiceSeleccionado = 0;
	}
	
	void reiniciarCursor() { indiceSeleccionado = 0; }
	
	int getOpcionActual() const { return indiceSeleccionado; }
	int getCantidadOpciones() const { return opciones.size(); }
	std::string getTextoOpcion(int i) const { return opciones[i]; }
};

#endif
