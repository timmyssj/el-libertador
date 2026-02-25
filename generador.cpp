#include <iostream>
#include <fstream>

int main() {
	int mapa[20][30] = {0}; 
	
	// Paredes del edificio (Impenetrables)
	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 30; x++) {
			if (y == 2 || y == 19 || x == 5 || x == 24) {
				mapa[y][x] = 1; // 1 = PARED 
			}
		}
	}
	
	// La puerta por la que entramos (abierta atrás)
	mapa[19][14] = 0; mapa[19][15] = 0; mapa[19][16] = 0;
	
	// Algunos pilares internos (usamos el número 3 = ROCA para reciclar la colisión)
	mapa[8][10] = 3; mapa[8][20] = 3;
	mapa[14][10] = 3; mapa[14][20] = 3;
	
	// La salida final (El altar o la puerta trasera)
	mapa[4][15] = 4;
	
	std::ofstream archivo("nivel3.dat", std::ios::binary);
	if (archivo.is_open()) {
		archivo.write(reinterpret_cast<char*>(mapa), sizeof(mapa));
		archivo.close();
		std::cout << "¡Exito! El Interior del Convento (nivel3.dat) ha sido forjado." << std::endl;
	}
	
	return 0;
}
