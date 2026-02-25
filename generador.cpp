#include <iostream>
#include <fstream>

int main() {
	// 1. Mapa vacío (0 = SUELO)
	int mapa[20][30] = {0}; 
	
	// 2. BORDES DE ROCA (3)
	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 30; x++) {
			if (y == 0 || y == 19 || x == 0 || x == 29) {
				mapa[y][x] = 3; 
			}
		}
	}
	
	// 3. EL CONVENTO DE SAN CARLOS (1 = PARED)
	// Hacemos una estructura en la parte superior central
	for (int x = 8; x <= 22; x++) {
		mapa[2][x] = 1; // Pared trasera
		mapa[6][x] = 1; // Pared frontal
	}
	for (int y = 2; y <= 6; y++) {
		mapa[y][8] = 1;  // Pared izquierda
		mapa[y][22] = 1; // Pared derecha
	}
	// Abrimos una "puerta" en el convento
	mapa[6][14] = 0;
	mapa[6][15] = 0;
	mapa[6][16] = 0;
	
	// 4. LA SALIDA (4 = SALIDA DORADA)
	// La colocamos a salvo adentro del patio del convento
	mapa[3][15] = 4;
	
	// 5. CAMPO DE BATALLA Y VEGETACIÓN (2 = ÁRBOL, 3 = ROCA)
	// Bosque a los costados
	for (int y = 8; y < 17; y++) {
		if (y % 2 == 0) {
			mapa[y][3] = 2; mapa[y][4] = 2; 
			mapa[y][25] = 2; mapa[y][26] = 2;
		}
	}
	// Rocas esparcidas como cobertura
	mapa[10][10] = 3; mapa[10][11] = 3;
	mapa[12][18] = 3; mapa[13][19] = 3;
	mapa[15][14] = 3;
	
	// 6. GUARDAR EN BINARIO
	std::ofstream archivo("nivel2.dat", std::ios::binary);
	if (archivo.is_open()) {
		archivo.write(reinterpret_cast<char*>(mapa), sizeof(mapa));
		archivo.close();
		std::cout << "¡Exito! El archivo 'nivel2.dat' ha sido forjado (San Lorenzo)." << std::endl;
	} else {
		std::cout << "[ERROR] No se pudo crear el archivo." << std::endl;
	}
	
	return 0;
}
