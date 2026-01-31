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
					
					// Color dinámico: Verde si está sano, Rojo si está muriendo
					sf::Color colorVida = sf::Color::Green;
					if (vida < 30) colorVida = sf::Color::Red;
					
					// Dibujamos el texto arriba a la izquierda
					dibujarTexto("SALUD: " + std::to_string(vida) + "%", 100, 30, colorVida, 20);
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
			Nivel* nivel = modelo->getNivelActual();
			if (!nivel) return; // Seguridad
			
			// A. DIBUJAR MAPA (Celdas)
			float tamCelda = 25.0f; // Tamaño de cada cuadradito
			sf::RectangleShape celda(sf::Vector2f(tamCelda, tamCelda));
			
			for (int y = 0; y < 24; y++) {
				for (int x = 0; x < 44; x++) {
					int tipo = nivel->getContenidoCelda(x, y);
					
					// Colores simples por ahora (Placeholder)
					if (tipo == PARED) celda.setFillColor(sf::Color(100, 50, 0)); // Marrón
					else if (tipo == SUELO) celda.setFillColor(sf::Color(0, 100, 0)); // Verde
					else if (tipo == AGUA) celda.setFillColor(sf::Color::Blue);
					else if (tipo == SALIDA_NIVEL) celda.setFillColor(sf::Color::Yellow);
					
					celda.setPosition(x * tamCelda, y * tamCelda);
					ventana.draw(celda);
				}
			}
			
			// B. DIBUJAR ENTIDADES (San Martín y Enemigos)
			const std::vector<Entidad*>& lista = nivel->getEntidades();
			for (Entidad* e : lista) {
				sf::CircleShape forma(10.0f); // Radio 10
				forma.setPosition(e->getX() * tamCelda, e->getY() * tamCelda);
				
				if (e->getTipo() == "PROCER") {
					forma.setFillColor(sf::Color::White); // San Martín es Blanco (Patria)
				} else if (e->getTipo() == "REALISTA") {
					forma.setFillColor(sf::Color::Red);   // Realistas Rojos
				} else if (e->getTipo() == "ALIADO") {
					forma.setFillColor(sf::Color::Blue); // Granaderos azules
				}
				
				ventana.draw(forma);
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
