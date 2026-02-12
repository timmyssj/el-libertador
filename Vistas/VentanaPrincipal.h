#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../Modelo/Juego.h" 

class VentanaPrincipal {
private:
	sf::RenderWindow ventana;
	Juego* modelo; 
	sf::Font fuente; // Necesaria para escribir texto en el menú
	
public:
	VentanaPrincipal(Juego* j) : modelo(j) {
		ventana.create(sf::VideoMode(1100, 600), "San Martin: El Libertador");
		ventana.setFramerateLimit(60);
		
		// CARGA DE FUENTE (Vital para el menú)
		// Asegúrate de tener un archivo .ttf junto a tu ejecutable
		if (!fuente.loadFromFile("PressStart2P.ttf")) {
			// Si no tienes fuente, el menú no se verá. 
			// Poned un archivo .ttf en la carpeta del proyecto.
		}
	}
	
	void ejecutar() {
		while (ventana.isOpen()) {
			procesarEventos();
			actualizar();
			renderizar();
		}
	}
	
private:
		// --- 1. PROCESAR INPUT (Ahora delegamos todo al Modelo) ---
		void procesarEventos() {
			sf::Event evento;
			while (ventana.pollEvent(evento)) {
				if (evento.type == sf::Event::Closed)
					ventana.close();
				
				if (evento.type == sf::Event::KeyPressed) {
					switch (evento.key.code) {
						// --- MOVIMIENTO (WASD + Flechas) ---
					case sf::Keyboard::W:     // Arriba (WASD)
					case sf::Keyboard::Up:    // Arriba (Flechas)
						modelo->procesarTeclaArriba(); 
					break;
					
					case sf::Keyboard::S:     // Abajo (WASD)
					case sf::Keyboard::Down:  // Abajo (Flechas)
						modelo->procesarTeclaAbajo(); 
					break;
					
					case sf::Keyboard::A:     // Izquierda (WASD)
					case sf::Keyboard::Left:  // Izquierda (Flechas)
						modelo->procesarTeclaIzquierda(); 
					break;
					
					case sf::Keyboard::D:     // Derecha (WASD)
					case sf::Keyboard::Right: // Derecha (Flechas)
						modelo->procesarTeclaDerecha(); 
					break;
					
					// --- ACCIONES ---
					case sf::Keyboard::Space: // Atacar
						modelo->atacarConSanMartin(); 
					break;
					
					case sf::Keyboard::Return:
						modelo->procesarTeclaEnter(); 
					break;
					
					case sf::Keyboard::Escape: // Pausa / Salir
						modelo->teclaEscape(); 
					break;
					
					default: break;
					}
				}
			}
		}
		
		// --- 2. ACTUALIZAR ---
		void actualizar() {
			modelo->actualizar();
			
			// Verificar si el usuario eligió "SALIR" en el menú
			if (modelo->getEstado() == SALIR) {
				ventana.close();
			}
		}
		
		// --- 3. RENDERIZAR (El Pintor) ---
		void renderizar() {
			ventana.clear(sf::Color::Black); 
			EstadoJuego estado = modelo->getEstado();
			
			if (estado == EN_MENU) {
				// Título Grande
				dibujarTexto("EL LIBERTADOR", 550, 100, sf::Color::Cyan, 30);
				// Dibujamos el menú principal
				dibujarMenuGenerico(modelo->getMenu(), 550, 250);
			} 
			else if (estado == INTRO_HISTORIA) {
				// 1. Título
				// --- LOGICA DEL TITULO CAMBIANTE ---
				if (modelo->esUltimaPaginaIntro()) {
					// Si es la última página (Instrucciones), mostramos el nombre del Nivel
					dibujarTexto(modelo->getTituloActual(), 550, 100, sf::Color::Cyan, 30);
				} 
				else {
					// Si estamos leyendo historia, mostramos el título fijo
					dibujarTexto("CONTEXTO HISTÓRICO", 550, 100, sf::Color::Yellow, 30);
				}
				std::string texto = modelo->getTextoHistoria();
				// Truco: restamos un poco a X según el largo del texto para centrarlo a ojo
				dibujarTexto(texto, 550, 300, sf::Color::White, 17);
				
				// 3. Instrucción
				dibujarTexto("Presiona ENTER para continuar...", 750, 500, sf::Color::Cyan, 15);
			}
			else if (estado == JUGANDO) {
				dibujarJuego();
				
				// --- UI: BARRA DE VIDA ---
				if (modelo->getNivelActual() && modelo->getNivelActual()->getHeroe()) {
					int vida = modelo->getNivelActual()->getHeroe()->getVida();
					int vidaMax = 100; // Asumiendo 100 como base
					
					// 1. Fondo de la barra (Gris o Rojo oscuro)
					sf::RectangleShape fondoBarra(sf::Vector2f(200, 20)); // 200px de ancho
					fondoBarra.setPosition(50, 30);
					fondoBarra.setFillColor(sf::Color(50, 0, 0));
					fondoBarra.setOutlineThickness(2);
					fondoBarra.setOutlineColor(sf::Color::White);
					ventana.draw(fondoBarra);
					
					// 2. Barra de vida actual (Verde a Rojo)
					float porcentaje = (float)vida / vidaMax;
					if (porcentaje < 0) porcentaje = 0;
					
					sf::RectangleShape barraActual(sf::Vector2f(200 * porcentaje, 20));
					barraActual.setPosition(50, 30);
					
					if (vida > 50) barraActual.setFillColor(sf::Color::Green);
					else if (vida > 25) barraActual.setFillColor(sf::Color::Yellow);
					else barraActual.setFillColor(sf::Color::Red);
					
					ventana.draw(barraActual);
					
					// 3. Texto encima de la barra
					dibujarTexto("SAN MARTIN", 150, 40, sf::Color::White, 12);
					
					// --- PANEL DE INSTRUCCIONES ---
					// Un fondo negro semitransparente abajo
					sf::RectangleShape panelInstrucciones(sf::Vector2f(800, 50));
					panelInstrucciones.setPosition(0, 550); // Abajo (asumiendo alto 600)
					panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 150));
					ventana.draw(panelInstrucciones);
					
					// --- TEXTO DINÁMICO ---
					std::string mensaje;
					sf::Color colorTexto;
					
					if (modelo->getNivelActual()->hayEnemigosVivos()) {
						// FASE 1: COMBATE
						mensaje = "OBJETIVO: Destruye los maniquies (ESPACIO)";
						colorTexto = sf::Color::White;
					} else {
						// FASE 2: ESCAPE (Solo aparece cuando rompes todo)
						mensaje = "¡OBJETIVO CUMPLIDO! Ve a la zona Dorada";
						colorTexto = sf::Color::Green; // Verde para indicar éxito
					}
					
					// Dibujamos el mensaje centrado
					dibujarTexto(mensaje, 400, 565, colorTexto, 18);
				}
			}
			else if (estado == PAUSA) {
				dibujarJuego(); // Fondo del juego congelado
				
				// Un fondo semitransparente negro para que se lea bien el texto (Opcional pero recomendado)
				sf::RectangleShape fondoOscuro(sf::Vector2f(800, 600));
				fondoOscuro.setFillColor(sf::Color(0, 0, 0, 150)); // Negro con transparencia
				ventana.draw(fondoOscuro);
				
				dibujarTexto("JUEGO PAUSADO", 550, 100, sf::Color::Yellow, 30);
				
				// Dibujamos el menú de pausa
				dibujarMenuGenerico(modelo->getMenuPausa(), 550, 300);
			}
			// --- NUEVO ---
			else if (estado == CONFIGURACION) {
				// Si venimos de pausa, dibujamos el juego de fondo
				// Si venimos del menú, fondo negro (ya está limpio)
				
				dibujarTexto("CONFIGURACIÓN", 550, 100, sf::Color::Cyan, 30);
				dibujarMenuGenerico(modelo->getMenuConfig(), 550, 200);
				
				dibujarTexto("Usa IZQ/DER para cambiar valor", 250, 500, sf::Color::White, 15);
			}
			// --- NUEVO: PANTALLAS FINALES ---
			else if (estado == GAME_OVER) {
				// Fondo rojo oscuro (opcional, o dejar negro)
				dibujarTexto("¡DERROTA!", 550, 200, sf::Color::Red, 60);
				dibujarTexto("Presiona ESC para volver al Menú", 750, 400, sf::Color::White, 20);
			}
			else if (estado == VICTORIA) {
				dibujarTexto("¡VICTORIA!", 550, 200, sf::Color::Green, 60);
				dibujarTexto("Has liberado esta zona.", 550, 300, sf::Color::White, 20);
				dibujarTexto("Presiona ESC para volver al Menú", 750, 400, sf::Color::White, 20);
			}
			
			ventana.display(); 
		}
		
		// --- AYUDANTES DE DIBUJO ---
		
		void dibujarMenu() {
			Menu* m = modelo->getMenu();
			if (!m) return;
			
			dibujarTexto("EL LIBERTADOR", 200, 100, sf::Color::Cyan, 30);
			
			for (int i = 0; i < m->getCantidadOpciones(); i++) {
				sf::Color color = sf::Color::White;
				// Si es la opción seleccionada, la pintamos ROJO
				if (i == m->getOpcionActual()) {
					color = sf::Color::Red;
				}
				
				// Dibujamos la opción centrada
				dibujarTexto(m->getTextoOpcion(i), 300, 250 + (i * 50), color, 20);
			}
		}
		
		void dibujarJuego() {
			// 1. CALCULO DINÁMICO DE TAMAÑO
			// Obtenemos el tamaño real de la ventana y lo dividimos por las celdas del mapa
			float anchoVentana = (float)ventana.getSize().x;
			float altoVentana = (float)ventana.getSize().y;
			
			float bloqueX = anchoVentana / 30.0f; // Ancho de cada celda
			float bloqueY = altoVentana / 20.0f;  // Alto de cada celda
			
			// 2. DIBUJAR EL MAPA (GRID)
			for (int i = 0; i < 20; i++) {      // Filas (Y)
				for (int j = 0; j < 30; j++) {  // Columnas (X)
					
					// Creamos la celda con el tamaño calculado
					sf::RectangleShape celda(sf::Vector2f(bloqueX, bloqueY));
					celda.setPosition(j * bloqueX, i * bloqueY);
					
					// Obtenemos contenido
					int contenido = 0;
					if (modelo->getNivelActual()) {
						contenido = modelo->getNivelActual()->getContenidoCelda(j, i);
					}
					
					// --- COLORES ---
					if (contenido == 1) { // PARED
						celda.setFillColor(sf::Color(100, 100, 100)); 
						celda.setOutlineThickness(-1);
						celda.setOutlineColor(sf::Color::Black);
					} 
					else if (contenido == 4) { // SALIDA
						celda.setFillColor(sf::Color(255, 215, 0, 150)); 
						celda.setOutlineThickness(-2);
						celda.setOutlineColor(sf::Color::Yellow);
					}
					else { // SUELO
						celda.setFillColor(sf::Color(50, 50, 50)); 
						celda.setOutlineThickness(1); 
						celda.setOutlineColor(sf::Color(60, 60, 60)); 
					}
					
					ventana.draw(celda);
				}
			}
			
			// 3. DIBUJAR ENTIDADES
			if (modelo->getNivelActual()) {
				const std::vector<Entidad*>& entidades = modelo->getNivelActual()->getEntidades();
				
				// Calculamos un radio que entre bien en la celda (usamos el menor lado para que sea redondo)
				float radio = (std::min(bloqueX, bloqueY) / 2.5f);
				
				for (Entidad* e : entidades) {
					if (!e->estaVivo()) continue;
					
					sf::CircleShape forma(radio); 
					
					// Centramos el círculo en la celda rectangular
					float posX = e->getX() * bloqueX + (bloqueX / 2) - radio;
					float posY = e->getY() * bloqueY + (bloqueY / 2) - radio;
					
					forma.setPosition(posX, posY);
					
					// Colores
					if (e->getTipo() == "PROCER") forma.setFillColor(sf::Color::White);
					else if (e->getTipo() == "REALISTA") forma.setFillColor(sf::Color::Red);
					else if (e->getTipo() == "ALIADO") forma.setFillColor(sf::Color::Blue);
					
					// --- NUEVO: MUÑECO DE PRÁCTICA (Color Madera/Marrón) ---
					else if (e->getTipo() == "PRACTICA") {
						forma.setFillColor(sf::Color(139, 69, 19)); // Marrón
					}
					ventana.draw(forma);
				}
			}
		}
		
		// Función auxiliar para no repetir código de texto
		void dibujarTexto(std::string mensaje, float x, float y, sf::Color color, int tam) {
			sf::Text texto;
			texto.setFont(fuente);
			texto.setString(mensaje);
			texto.setCharacterSize(tam);
			texto.setFillColor(color);
			
			// TRUCO: CENTRAR ORIGEN
			// Calculamos el ancho y alto del texto y ponemos el "punto de anclaje" en el medio
			sf::FloatRect bounds = texto.getLocalBounds();
			texto.setOrigin(bounds.width / 2, bounds.height / 2);
			
			// Ahora 'x' e 'y' serán el CENTRO exacto del texto
			texto.setPosition(x, y);
			
			ventana.draw(texto);
		}
		
		// FUNCIÓN REUTILIZABLE PARA DIBUJAR CUALQUIER MENÚ
		void dibujarMenuGenerico(Menu* menu, float xBase, float yBase) {
			if (!menu) return;
			
			for (int i = 0; i < menu->getCantidadOpciones(); i++) {
				sf::Color color = sf::Color::White;
				if (i == menu->getOpcionActual()) color = sf::Color::Red; // El seleccionado
				
				dibujarTexto(menu->getTextoOpcion(i), xBase, yBase + (i * 50), color, 20);
			}
		}
};

#endif
