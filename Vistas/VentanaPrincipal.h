#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm> 
#include "../Modelo/juego.h" 

class VentanaPrincipal {
private:
	sf::RenderWindow ventana;
	Juego* modelo; 
	sf::Font fuente; 
	
	sf::Texture texSanMartin;
	sf::Texture texEnemigo;
	sf::Texture texGranadero;
	sf::Texture texSuelo;
	sf::Texture texPared;
	
	sf::Texture texturaFondoMenu;
	sf::Sprite spriteFondoMenu;
	
	std::string fondoActualCargado;
	sf::Texture texturaFondoNivel;
	sf::Sprite spriteFondoNivel;
	
	sf::View camara;
	
public:
	VentanaPrincipal(Juego* j) : modelo(j) {
		ventana.create(sf::VideoMode(1100, 600), "San Martin: El Libertador");
		ventana.setFramerateLimit(60);
		
		fondoActualCargado = ""; 
		
		if (!fuente.loadFromFile("PressStart2P.ttf")) { }
		
		if (!texturaFondoMenu.loadFromFile("fondos/pantalla1.png")) {
			std::cout << "Error: No se pudo cargar la imagen de fondo del menu." << std::endl;
		} else {
			spriteFondoMenu.setTexture(texturaFondoMenu);
			float escalaX = (float)ventana.getSize().x / texturaFondoMenu.getSize().x;
			float escalaY = (float)ventana.getSize().y / texturaFondoMenu.getSize().y;
			spriteFondoMenu.setScale(escalaX, escalaY);
		}
		
		camara.setSize((float)ventana.getSize().x, (float)ventana.getSize().y);
		camara.zoom(0.6f);
	}
	
	void ejecutar() {
		while (ventana.isOpen()) {
			procesarEventos();
			actualizar();
			renderizar();
		}
	}
	
private:
		void procesarEventos() {
			sf::Event evento;
			while (ventana.pollEvent(evento)) {
				if (evento.type == sf::Event::Closed)
					ventana.close();
				
				if (evento.type == sf::Event::KeyPressed) {
					switch (evento.key.code) {
					case sf::Keyboard::W: case sf::Keyboard::Up:    modelo->procesarTeclaArriba(); break;
					case sf::Keyboard::S: case sf::Keyboard::Down:  modelo->procesarTeclaAbajo(); break;
					case sf::Keyboard::A: case sf::Keyboard::Left:  modelo->procesarTeclaIzquierda(); break;
					case sf::Keyboard::D: case sf::Keyboard::Right: modelo->procesarTeclaDerecha(); break;
					case sf::Keyboard::Space: modelo->atacarConSanMartin(); break;
					case sf::Keyboard::Return: modelo->procesarTeclaEnter(); break;
					case sf::Keyboard::Escape: modelo->teclaEscape(); break;
					default: break;
					}
				}
			}
		}
		
		void actualizar() {
			modelo->actualizar();
			if (modelo->getEstado() == SALIR) {
				ventana.close();
			}
		}
		
		void renderizar() {
			ventana.clear(sf::Color::Black); 
			EstadoJuego estado = modelo->getEstado();
			
			if (estado == EN_MENU) {
				if (spriteFondoMenu.getTexture() != nullptr) ventana.draw(spriteFondoMenu);
				dibujarTexto("EL LIBERTADOR", 550, 100, sf::Color::Cyan, 30);
				dibujarMenuGenerico(modelo->getMenu(), 550, 250);
			} 
			else if (estado == SELECCION_NIVEL) {
				dibujarTexto("SELECCIONAR NIVEL", 550, 100, sf::Color::Yellow, 30);
				Menu* menu = modelo->getMenuNiveles();
				std::vector<std::string> opciones = menu->getOpciones(); 
				int desbloqueados = modelo->getNivelMaximo();
				int opcionActual = menu->getOpcionActual(); 
				
				for (size_t i = 0; i < opciones.size(); i++) {
					sf::Color colorTexto = sf::Color::White;
					std::string texto = opciones[i];
					
					// --- CORRECCIÓN MATEMÁTICA: Usar >= en lugar de > ---
					bool estaBloqueado = ((int)i >= desbloqueados && i != opciones.size() - 1);
					
					if (estaBloqueado) {
						texto += " (BLOQUEADO)";
						colorTexto = sf::Color(100, 100, 100); 
					}
					
					if (i == (size_t)opcionActual) {
						if (estaBloqueado) colorTexto = sf::Color::Red; 
						else colorTexto = sf::Color::Cyan; 
					}
					
					dibujarTexto(texto, 550, 200 + (i * 50), colorTexto, 20);
				}
				
				// --- CORRECCIÓN EN EL MENSAJE INFERIOR ---
				if (opcionActual >= desbloqueados && opcionActual != (int)opciones.size() - 1) {
					dibujarTexto("¡Completa el nivel anterior para desbloquearlo!", 550, 500, sf::Color::Red, 15);
				} else if (opcionActual != (int)opciones.size() - 1) {
					dibujarTexto("Presiona ENTER para iniciar", 550, 500, sf::Color::Green, 15);
				}
			}
			else if (estado == INTRO_HISTORIA) {
				if (modelo->esUltimaPaginaIntro()) dibujarTexto(modelo->getTituloActual(), 550, 100, sf::Color::Cyan, 30);
				else dibujarTexto("CONTEXTO HISTORICO", 550, 100, sf::Color::Yellow, 30);
				dibujarTexto(modelo->getTextoHistoria(), 550, 300, sf::Color::White, 17);
				dibujarTexto("Presiona ENTER para continuar...", 750, 500, sf::Color::Cyan, 15);
			}
			else if (estado == JUGANDO) {
				dibujarJuego(); // 1. Dibuja el mapa y personajes (esto ya resetea el zoom dinámico)
				
				if (modelo->getNivelActual() != nullptr) {
					
					// 2A. ¿ESTAMOS EN UNA CINEMÁTICA?
					if (modelo->getNivelActual()->enCinematica()) {
						sf::RectangleShape fondoOscuro(sf::Vector2f(ventana.getSize().x, ventana.getSize().y));
						fondoOscuro.setFillColor(sf::Color(0, 0, 0, 220)); 
						ventana.draw(fondoOscuro);
						
						dibujarTexto("MOMENTO HISTORICO", 550, 150, sf::Color::Yellow, 24);
						// ACHICAMOS LA LETRA A 12 PARA QUE ENTRE PERFECTO
						dibujarTexto(modelo->getNivelActual()->getMensajeCinematica(), 550, 320, sf::Color::White, 12);
						dibujarTexto("Presiona ENTER para continuar", 550, 500, sf::Color::Cyan, 12);
					}
					// 2B. ¿GANAMOS EL NIVEL?
					else if (modelo->getNivelActual()->estaCompletado()) {
						sf::RectangleShape fondoOscuro(sf::Vector2f(ventana.getSize().x, ventana.getSize().y));
						fondoOscuro.setFillColor(sf::Color(0, 0, 0, 180)); 
						ventana.draw(fondoOscuro);
						
						// --- VICTORIA ÉPICA ---
						// Título gigante en amarillo (Tamaño 60 como la Derrota)
						dibujarTexto("¡VICTORIA!", 550, 200, sf::Color::Green, 60);
						
						// Subtítulo descriptivo en blanco
						dibujarTexto("Has asegurado la posicion.", 550, 320, sf::Color::White, 20);
						
						// Instrucción para continuar en verde vibrante
						dibujarTexto("Presiona ENTER para continuar", 550, 450, sf::Color::Cyan, 15);
					}
					// 2C. JUEGO NORMAL: DIBUJAR INTERFAZ (HUD)
					else {
						// Dibujar la Barra de Vida
						if (modelo->getNivelActual()->getHeroe()) {
							int vida = modelo->getNivelActual()->getHeroe()->getVida();
							sf::RectangleShape fondoBarra(sf::Vector2f(200, 20)); 
							fondoBarra.setPosition(50, 30); fondoBarra.setFillColor(sf::Color(50, 0, 0));
							fondoBarra.setOutlineThickness(2); fondoBarra.setOutlineColor(sf::Color::White);
							ventana.draw(fondoBarra);
							
							float porcentaje = (float)vida / 100.0f; if (porcentaje < 0) porcentaje = 0;
							sf::RectangleShape barraActual(sf::Vector2f(200 * porcentaje, 20)); barraActual.setPosition(50, 30);
							if (vida > 50) barraActual.setFillColor(sf::Color::Green); 
							else if (vida > 25) barraActual.setFillColor(sf::Color::Yellow); 
							else barraActual.setFillColor(sf::Color::Red);
							
							ventana.draw(barraActual); 
							dibujarTexto("SAN MARTIN", 150, 40, sf::Color::White, 12);
						}
						
						// --- NUEVO: DIBUJAR EL RELOJ DEL NIVEL (SI LO TIENE) ---
						int tiempo = modelo->getNivelActual()->getTiempoRestante();
						if (tiempo != -1) {
							// Elegimos el color: Blanco normal, Rojo si quedan 30s o menos
							sf::Color colorReloj = (tiempo <= 30) ? sf::Color::Red : sf::Color::White;
							
							std::string textoReloj = "TIEMPO: " + std::to_string(tiempo);
							
							// Lo dibujamos arriba al medio (X: 550, Y: 40) tamaño 20
							dibujarTexto(textoReloj, 550, 40, colorReloj, 20);
						}
						// --------------------------------------------------------
						
						// Dibujar la Barra de Objetivo Abajo
						sf::RectangleShape panelInstrucciones(sf::Vector2f(800, 50));
						panelInstrucciones.setPosition(0, 550); panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 150));
						ventana.draw(panelInstrucciones);
						
						std::string mensaje; sf::Color colorTexto;
						if (modelo->getNivelActual()->hayEnemigosVivos()) { 
							mensaje = modelo->getNivelActual()->getObjetivo(); 
							colorTexto = sf::Color::White; 
						} else { 
							mensaje = "¡OBJETIVO CUMPLIDO! Ve a la zona Dorada"; 
							colorTexto = sf::Color::Green; 
						}
						dibujarTexto(mensaje, 500, 565, colorTexto, 14);
					}
				}
			}
			else if (estado == PAUSA) {
				dibujarJuego(); 
				sf::RectangleShape fondoOscuro(sf::Vector2f(1100, 600)); fondoOscuro.setFillColor(sf::Color(0, 0, 0, 150)); ventana.draw(fondoOscuro);
				dibujarTexto("JUEGO PAUSADO", 550, 100, sf::Color::Yellow, 30); dibujarMenuGenerico(modelo->getMenuPausa(), 550, 300);
			}
			else if (estado == CONFIGURACION) {
				dibujarTexto("CONFIGURACION", 550, 100, sf::Color::Cyan, 30); dibujarMenuGenerico(modelo->getMenuConfig(), 550, 200);
				dibujarTexto("Usa IZQ/DER para cambiar valor", 550, 500, sf::Color::White, 15);
			}
			else if (estado == GAME_OVER) {
				dibujarTexto("¡DERROTA!", 550, 200, sf::Color::Red, 60); dibujarTexto("Presiona ESC para volver al Menu", 550, 400, sf::Color::White, 20);
			}
			ventana.display(); 
		}
		
		// --- FUNCIONES AYUDANTES ---
		
		void dibujarJuego() {
			float anchoVentana = (float)ventana.getDefaultView().getSize().x;
			float altoVentana = (float)ventana.getDefaultView().getSize().y;
			float bloqueX = anchoVentana / 30.0f; 
			float bloqueY = altoVentana / 20.0f;  
			
			if (modelo->getNivelActual() && modelo->getNivelActual()->getHeroe()) {
				SanMartin* heroe = modelo->getNivelActual()->getHeroe();
				float heroePixelX = (heroe->getX() * bloqueX) + (bloqueX / 2);
				float heroePixelY = (heroe->getY() * bloqueY) + (bloqueY / 2);
				float mitadCamaraX = camara.getSize().x / 2.0f;
				float mitadCamaraY = camara.getSize().y / 2.0f;
				float limiteDerecho = (30 * bloqueX) - mitadCamaraX;
				float limiteInferior = (20 * bloqueY) - mitadCamaraY;
				if (heroePixelX < mitadCamaraX) heroePixelX = mitadCamaraX;
				if (heroePixelX > limiteDerecho) heroePixelX = limiteDerecho;
				if (heroePixelY < mitadCamaraY) heroePixelY = mitadCamaraY;
				if (heroePixelY > limiteInferior) heroePixelY = limiteInferior;
				camara.setCenter(heroePixelX, heroePixelY);
				ventana.setView(camara);
			}
			
			if (modelo->getNivelActual()) {
				std::string rutaFondo = modelo->getNivelActual()->getArchivoFondo();
				if (rutaFondo != "" && rutaFondo != fondoActualCargado) {
					if (texturaFondoNivel.loadFromFile(rutaFondo)) { spriteFondoNivel.setTexture(texturaFondoNivel); fondoActualCargado = rutaFondo; } 
					else { fondoActualCargado = "ERROR"; }
				}
				if (fondoActualCargado != "" && fondoActualCargado != "ERROR") {
					float escalaX = (30.0f * bloqueX) / texturaFondoNivel.getSize().x;
					float escalaY = (20.0f * bloqueY) / texturaFondoNivel.getSize().y;
					spriteFondoNivel.setScale(escalaX, escalaY); spriteFondoNivel.setPosition(0, 0); ventana.draw(spriteFondoNivel);
				}
			}
			
			for (int i = 0; i < 20; i++) {       
				for (int j = 0; j < 30; j++) {   
					sf::RectangleShape celda(sf::Vector2f(bloqueX, bloqueY)); celda.setPosition(j * bloqueX, i * bloqueY);
					int contenido = 0; if (modelo->getNivelActual()) contenido = modelo->getNivelActual()->getContenidoCelda(j, i);
					if (contenido == 1) { celda.setFillColor(sf::Color(100, 100, 100)); celda.setOutlineColor(sf::Color(40, 40, 40)); } 
					else if (contenido == 4) { celda.setFillColor(sf::Color(255, 215, 0, 100)); celda.setOutlineColor(sf::Color(40, 40, 40)); } 
					else {
						if (fondoActualCargado != "" && fondoActualCargado != "ERROR") { celda.setFillColor(sf::Color::Transparent); celda.setOutlineColor(sf::Color(255, 255, 255, 20)); } 
						else { celda.setFillColor(sf::Color(50, 50, 50)); celda.setOutlineColor(sf::Color(40, 40, 40)); }
					}
					celda.setOutlineThickness(1); ventana.draw(celda);
				}
			}
			
			// --- 3. DIBUJAR PERSONAJES Y OBJETOS 3D (SISTEMA DE DOS PASADAS) ---
			if (modelo->getNivelActual()) {
				const std::vector<Entidad*>& entidades = modelo->getNivelActual()->getEntidades();
				
				for (int fila = 0; fila < 20; fila++) {
					
					// PASADA 1: Dibujar Personajes "planos" (San Martín, Enemigos)
					for (Entidad* e : entidades) {
						if (!e->estaVivo()) continue;
						if ((int)e->getY() == fila) {
							if (e->getTipo() != "ARBOL" && e->getTipo() != "ROCA") {
								dibujarEntidadUnica(e, bloqueX, bloqueY);
							}
						}
					}
					
					// PASADA 2: Dibujar Obstáculos altos ENCIMA de los personajes
					for (Entidad* e : entidades) {
						if (!e->estaVivo()) continue;
						if ((int)e->getY() == fila) {
							if (e->getTipo() == "ARBOL" || e->getTipo() == "ROCA" || e->getTipo() == "OBSTACULO_CONVENTO") {
								dibujarEntidadUnica(e, bloqueX, bloqueY);
							}
						}
					}
				}
			}
			// --- DIBUJAR LA CINEMÁTICA IN-GAME ---
			if (modelo->getNivelActual() != nullptr && modelo->getNivelActual()->enCinematica()) {
				sf::RectangleShape fondoOscuro(sf::Vector2f(ventana.getSize().x, ventana.getSize().y));
				fondoOscuro.setFillColor(sf::Color(0, 0, 0, 220)); // Pantalla casi negra
				ventana.draw(fondoOscuro);
				
				// Bajamos el tamaño del título de 30 a 24 y lo subimos un poco (Y=150)
				dibujarTexto("MOMENTO HISTÓRICO", 550, 150, sf::Color::Yellow, 24);
				
				// ¡EL CAMBIO CLAVE! Bajamos el texto principal de 20 a 14
				dibujarTexto(modelo->getNivelActual()->getMensajeCinematica(), 550, 320, sf::Color::White, 14);
				
				// El botón de continuar lo dejamos chiquito
				dibujarTexto("Presiona ENTER para continuar", 550, 500, sf::Color::Cyan, 12);
			}
			
			ventana.setView(ventana.getDefaultView());
		}
		
		// --- NUEVA FUNCIÓN PRIVADA PARA DIBUJAR UNA SOLA ENTIDAD ---
		void dibujarEntidadUnica(Entidad* e, float bloqueX, float bloqueY) {
			sf::Sprite* spritePtr = e->getSpriteRender();
			if (!spritePtr) return;
			
			sf::CircleShape sombra(bloqueX * 0.35f); 
			sombra.setFillColor(sf::Color(0, 0, 0, 120)); 
			sombra.setScale(1.0f, 0.4f); 
			float sombraX = (e->getX() * bloqueX) + (bloqueX / 2) - sombra.getGlobalBounds().width / 2;
			float sombraY = (e->getY() * bloqueY) + bloqueY - sombra.getGlobalBounds().height;
			sombra.setPosition(sombraX, sombraY);
			ventana.draw(sombra);
			
			// --- CORRECCIÓN INFALIBLE: ESCALA DINÁMICA ---
			Personaje* p = dynamic_cast<Personaje*>(e);
			float factorEscala = 1.5f; 
			
			if (p == nullptr) {
				factorEscala = 2.3f; // Árboles y rocas
				
				// --- NUEVO: Detectar el nombre correcto para hacerlo GIGANTE ---
				if (e->getTipo() == "OBSTACULO_CONVENTO") {
					factorEscala = 20.0f; // Escala del edificio (ajusta a gusto)
				}
				else if (e->getTipo() == "OBSTACULO_PILAR") {
					// Ajusta este número: 
					// 1.5f = tamaño de una persona
					// 2.3f = tamaño de un árbol grande
					factorEscala = 10.0f; 
				}else if (e->getTipo() == "ITEM_SABLE" || e->getTipo() == "ITEM_CURACION") {
					factorEscala = 1.0f; // Pequeños, tirados en el suelo
				}
			}
			
			if (spritePtr->getTexture()) {
				float escalaX = (bloqueX / spritePtr->getTexture()->getSize().x) * factorEscala;
				float escalaY = (bloqueY / spritePtr->getTexture()->getSize().y) * factorEscala;
				spritePtr->setScale(escalaX, escalaY);
			}
			
			float anchoSprite = spritePtr->getGlobalBounds().width;
			float altoSprite = spritePtr->getGlobalBounds().height;
			
			float posX = (e->getX() * bloqueX) + (bloqueX / 2) - (anchoSprite / 2);
			float posY = (e->getY() * bloqueY) + bloqueY - altoSprite;
			
			if (p) {
				posX += (p->getOffsetX() * bloqueX);
				posY += (p->getOffsetY() * bloqueY);
				if (e->getTipo() == "PROCER") posY += 0.0f; 
				else posY += 3.0f; 
			} else {
				posY += 10.0f; // Hunde un poco los árboles
				
				// --- CORRECCIÓN: El convento no se hunde, queda a ras del suelo ---
				if (e->getTipo() == "OBSTACULO_CONVENTO") {
					posY += 20.0f; 
				}else if (e->getTipo() == "OBSTACULO_PILAR"){
					posY += 10.0f;
				}
			}
			
			spritePtr->setPosition(posX, posY);
			ventana.draw(*spritePtr);
			
			// Barras de vida (solo si es personaje y no es San Martín)
			if (p && e->getTipo() != "PROCER") {
				float anchoBarra = bloqueX * 0.8f; float altoBarra = 6.0f;            
				float porcentajeVida = p->getVida() / p->getVidaMax();
				if (porcentajeVida < 0) porcentajeVida = 0;
				sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarra, altoBarra)); fondoBarra.setFillColor(sf::Color::Red);
				float barraX = (e->getX() * bloqueX) + (bloqueX / 2) - (anchoBarra / 2); float barraY = posY - 8.0f; 
				fondoBarra.setPosition(barraX, barraY); fondoBarra.setOutlineThickness(1); fondoBarra.setOutlineColor(sf::Color::Black);
				sf::RectangleShape barraVerde(sf::Vector2f(anchoBarra * porcentajeVida, altoBarra)); barraVerde.setFillColor(sf::Color::Green);
				barraVerde.setPosition(barraX, barraY); ventana.draw(fondoBarra); ventana.draw(barraVerde);
			}
		}
		
		void dibujarTexto(std::string mensaje, float x, float y, sf::Color color, int tam) {
			sf::Text texto; texto.setFont(fuente); texto.setString(mensaje); texto.setCharacterSize(tam); texto.setFillColor(color);
			sf::FloatRect bounds = texto.getLocalBounds(); texto.setOrigin(bounds.width / 2, bounds.height / 2);
			texto.setPosition(x, y); ventana.draw(texto);
		}
		
		void dibujarMenuGenerico(Menu* menu, float xBase, float yBase) {
			if (!menu) return;
			for (int i = 0; i < menu->getCantidadOpciones(); i++) {
				sf::Color color = sf::Color::White; if (i == menu->getOpcionActual()) color = sf::Color::Red; 
				dibujarTexto(menu->getTextoOpcion(i), xBase, yBase + (i * 50), color, 20);
			}
		}
};

#endif
