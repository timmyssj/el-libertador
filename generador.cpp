#include <iostream>
#include <fstream>

// DICCIONARIO DEL MAPA:
// 0 = Suelo transitable (Pasto/Tierra)
// 1 = Pared Invisible / Borde del mundo (Para que no se salga de la pantalla)
// 2 = Entidad Árbol (Obstáculo 3D)
// 3 = Entidad Roca (Obstáculo 3D)
// 4 = Salida del Nivel (Zona de victoria)

void generarMapaBinario() {
	int mapaVisual[20][30] = {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, // Norte (Salida)
	{1,2,2,2,0,0,0,0,2,2,2,2,0,0,0,0,0,0,2,2,2,0,0,0,0,2,2,2,2,1},
	{1,2,2,0,0,3,0,0,0,2,2,0,0,0,0,0,0,0,0,2,2,0,0,3,0,0,0,2,2,1},
	{1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,3,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,3,0,0,1}, // Coberturas
	{1,0,0,0,0,2,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,1},
	{1,0,0,3,0,0,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,1},
	{1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,1}, // Zona central
	{1,3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,1},
	{1,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,2,1},
	{1,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,1}, // Más coberturas
	{1,0,0,0,3,3,0,0,0,0,0,0,2,2,3,2,2,0,0,0,0,3,3,3,0,0,0,0,0,1},
	{1,0,0,3,3,0,0,0,0,0,0,2,3,3,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,2,0,0,0,0,0,0,0,0,2,2,2,2,0,2,2,2,2,0,0,0,0,0,0,0,0,0,3,1},
	{1,2,2,0,0,0,0,0,0,0,2,2,2,0,0,0,2,2,2,0,0,0,0,0,0,0,0,3,3,1},
	{1,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,1},
	{1,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,1},
	{1,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,1}, // Sur (Inicio)
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};
	
	std::ofstream archivoSalida("nivel1.dat", std::ios::binary);
	if (archivoSalida.is_open()) {
		archivoSalida.write(reinterpret_cast<char*>(mapaVisual), sizeof(mapaVisual));
		archivoSalida.close();
		std::cout << "[EXITO] Archivo nivel1.dat generado con Arboles (2) y Rocas (3)." << std::endl;
	} else {
		std::cout << "[ERROR] No se pudo crear el archivo." << std::endl;
	}
}

int main() {
	// Ejecutar solo una vez para generar el archivo
	generarMapaBinario();
	
	// Todo lo demás de tu main normal coméntalo por ahora
	return 0;
}
