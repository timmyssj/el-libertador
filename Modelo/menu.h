#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>

class Menu {
private:
	std::vector<std::string> opciones;
	int indiceSeleccionado; 
	
public:
	Menu(std::vector<std::string> listaOpciones) {
		opciones = listaOpciones;
		indiceSeleccionado = 0;
	}
	
	// Esta función faltaba y causaba error en VentanaPrincipal
	void setOpciones(const std::vector<std::string>& nuevasOpciones) {
		opciones = nuevasOpciones;
		indiceSeleccionado = 0; 
	}
	
	std::vector<std::string> getOpciones() const { return opciones; }
	
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
