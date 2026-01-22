#ifndef JUEGO_H
#define JUEGO_H

// Tipos de terreno para matriz
enum TipoTerreno {
	SUELO,
	PARED,
	AGUA,
	EVENTO_NARRATIVO,
	SALIDA_NIVEL
};
struct Enemigo {
	int x, y;
	int salud;
	// Si quieres que sean más lentos, usamos un contador
	int contadorPasos; 
};
class Juego {
private:
	// --- ESTADO DEL MUNDO ---
	int mapa[20][30]; 
	int capituloActual; 
	bool juegoPausado; // Variable para la pausa
	
	// --- ESTADO DE SAN MARTÍN ---
	int smX, smY;       
	int salud;          
	int moral;          
	int suministros;    
	//lista de enemigos
	std::vector<Enemigo> realistas;
public:
	Juego(); // Constructor
	
	// --- MÉTODOS DE CONTROL ---
	void cargarNivel(int capitulo); 
	void intentarMover(int dx, int dy); 
	
	// --- MÉTODOS QUE PEDÍA LA VENTANA (Corrección de errores) ---
	void pausarOReanudar();
	bool estaEnPausa();
	void actualizar(); // <--- Faltaba esta función vital
	
	// --- GETTERS (Para dibujar) ---
	int getSanMartinX() const { return smX; }
	int getSanMartinY() const { return smY; }
	int getSalud() const { return salud; }
	int getMoral() const { return moral; }
	int getSuministros() const { return suministros; }
	
	// Auxiliar para saber qué dibujar en el mapa
	int getContenidoCelda(int x, int y) {
		if(x >= 0 && x < 30 && y >= 0 && y < 20) {
			return mapa[y][x];
		}
		return PARED; // Si se va del rango, devolvemos pared por seguridad
	}
	//getter
	const std::vector<Enemigo>& getEnemigos() const { return realistas; }
};

#endif // JUEGO_H
