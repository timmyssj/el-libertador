#ifndef JUEGO_H
#define JUEGO_H

// Tipos de terreno
enum TipoTerreno {
	SUELO,
	PARED,
	AGUA,
	EVENTO_NARRATIVO,
	SALIDA_NIVEL
};

class Juego {
private:
	// --- ESTADO DEL MUNDO ---
	int mapa[20][30]; 
	int capituloActual; 
	bool juegoPausado;
	
	// --- ESTADO DE SAN MARTÍN ---
	int smX, smY;       
	int salud;          
	int moral;          
	int suministros;    
	
public:
	Juego(); 
	
	// --- MÉTODOS DE CONTROL ---
	void cargarNivel(int capitulo); 
	void intentarMover(int dx, int dy); 
	
	// Métodos de Pausa y Loop (Necesarios para que no de error la ventana)
	void pausarOReanudar();
	bool estaEnPausa();
	void actualizar(); 
	
	// --- GETTERS ---
	int getSanMartinX() const { return smX; }
	int getSanMartinY() const { return smY; }
	int getSalud() const { return salud; }
	int getMoral() const { return moral; }
	int getSuministros() const { return suministros; }
	
	// Auxiliar para saber qué dibujar
	int getContenidoCelda(int x, int y);
};

#endif // JUEGO_H
