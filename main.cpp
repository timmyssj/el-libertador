#include "Modelo/Juego.h"
#include "Vistas/VentanaPrincipal.h"

int main() {
	// 1. Crear el Modelo (Lógica)
	Juego juegoLogica;
	
	// 2. Crear la Vista e inyectarle el Modelo
	VentanaPrincipal ventana(&juegoLogica);
	
	// 3. Iniciar el bucle
	ventana.ejecutar();
	
	return 0;
}
