#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

class Menu {
private:
	std::vector<std::string> opciones;
	int indiceSeleccionado;
	
public:
	// MODIFICADO: Ahora el constructor pide la lista de opciones
	Menu(std::vector<std::string> listaOpciones) {
		opciones = listaOpciones;
		indiceSeleccionado = 0;
	}
	
	void setOpciones(std::vector<std::string> nuevasOpciones) {
		opciones = nuevasOpciones;
	}
	
	void moverArriba() {
		indiceSeleccionado--;
		if (indiceSeleccionado < 0) indiceSeleccionado = opciones.size() - 1;
	}
	
	void moverAbajo() {
		indiceSeleccionado++;
		if (indiceSeleccionado >= (int)opciones.size()) indiceSeleccionado = 0;
	}
	
	// Función para resetear el cursor al abrir el menú
	void reiniciarCursor() { indiceSeleccionado = 0; }
	
	int getOpcionActual() const { return indiceSeleccionado; }
	int getCantidadOpciones() const { return opciones.size(); }
	std::string getTextoOpcion(int i) const { return opciones[i]; }
};

#endif
