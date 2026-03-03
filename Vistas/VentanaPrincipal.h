#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm> 
#include <cstdlib> 
#include "../Modelo/juego.h" 

class VentanaPrincipal {
private:
	sf::RenderWindow ventana;
	Juego* modelo; 
	sf::Font fuente; 
	
	sf::Texture texRocaNieve;
	sf::Sprite spriteRocaNieve;
	sf::Texture texturaFondoMenu;
	sf::Sprite spriteFondoMenu;
	
	std::string fondoActualCargado;
	sf::Texture texturaFondoNivel;
	sf::Sprite spriteFondoNivel;
	sf::View camara;
	
	struct CopoNieve { float x, y, velY, velX; };
	std::vector<CopoNieve> copos;
	
public:
	VentanaPrincipal(Juego* j) : modelo(j) {
		ventana.create(sf::VideoMode(1100, 600), "San Martin: El Libertador");
		ventana.setFramerateLimit(60);
		
		fondoActualCargado = ""; 
		if (!fuente.loadFromFile("PressStart2P.ttf")) { }
		
		if (!texRocaNieve.loadFromFile("sprites/roca_nieve.png")) {
			std::cout << "Error: No se pudo cargar sprites/roca_nieve.png" << std::endl;
		} else {
			spriteRocaNieve.setTexture(texRocaNieve);
		}
		
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
		
		for (int i = 0; i < 300; i++) {
			copos.push_back({
				(float)(rand() % 1100), (float)(rand() % 600), 
										 (float)(rand() % 3 + 2), (float)((rand() % 3) - 1) 
			});
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
		void procesarEventos() {
			sf::Event evento;
			while (ventana.pollEvent(evento)) {
				if (evento.type == sf::Event::Closed) ventana.close();
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
			if (modelo->getEstado() == SALIR) ventana.close();
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
					bool estaBloqueado = ((int)i >= desbloqueados && i != opciones.size() - 1);
					
					if (estaBloqueado) { texto += " (BLOQUEADO)"; colorTexto = sf::Color(100, 100, 100); }
					if (i == (size_t)opcionActual) { colorTexto = estaBloqueado ? sf::Color::Red : sf::Color::Cyan; }
					
					dibujarTexto(texto, 550, 200 + (i * 50), colorTexto, 20);
				}
				
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
				dibujarJuego(); 
				
				if (modelo->getNivelActual() != nullptr) {
					if (modelo->getNivelActual()->enCinematica()) {
						static sf::RectangleShape fondoOscuro;
						if (fondoOscuro.getSize().x == 0) {
							fondoOscuro.setSize(sf::Vector2f(ventana.getSize().x, ventana.getSize().y));
							fondoOscuro.setFillColor(sf::Color(0, 0, 0, 220)); 
						}
						ventana.draw(fondoOscuro);
						dibujarTexto("MOMENTO HISTORICO", 550, 150, sf::Color::Yellow, 24);
						dibujarTexto(modelo->getNivelActual()->getMensajeCinematica(), 550, 320, sf::Color::White, 12);
						dibujarTexto("Presiona ENTER para continuar", 550, 500, sf::Color::Cyan, 12);
					}
					else if (modelo->getNivelActual()->estaCompletado()) {
						static sf::RectangleShape fondoOscuro;
						if (fondoOscuro.getSize().x == 0) {
							fondoOscuro.setSize(sf::Vector2f(ventana.getSize().x, ventana.getSize().y));
							fondoOscuro.setFillColor(sf::Color(0, 0, 0, 180)); 
						}
						ventana.draw(fondoOscuro);
						dibujarTexto("¡VICTORIA!", 550, 200, sf::Color::Green, 60);
						dibujarTexto("Has asegurado la posicion.", 550, 320, sf::Color::White, 20);
						dibujarTexto("Presiona ENTER para continuar", 550, 450, sf::Color::Cyan, 15);
					}
					else {
						if (modelo->getNivelActual()->getHeroe()) {
							int vida = modelo->getNivelActual()->getHeroe()->getVida();
							
							static sf::RectangleShape fondoBarra(sf::Vector2f(200, 20)); 
							static sf::RectangleShape barraActual;
							static bool initHud = false;
							if(!initHud){
								fondoBarra.setPosition(50, 30); fondoBarra.setFillColor(sf::Color(50, 0, 0));
								fondoBarra.setOutlineThickness(2); fondoBarra.setOutlineColor(sf::Color::White);
								barraActual.setPosition(50, 30);
								initHud = true;
							}
							
							ventana.draw(fondoBarra);
							
							float porcentaje = (float)vida / 100.0f; if (porcentaje < 0) porcentaje = 0;
							barraActual.setSize(sf::Vector2f(200 * porcentaje, 20));
							if (vida > 50) barraActual.setFillColor(sf::Color::Green); 
							else if (vida > 25) barraActual.setFillColor(sf::Color::Yellow); 
							else barraActual.setFillColor(sf::Color::Red);
							
							ventana.draw(barraActual); 
							dibujarTexto("SAN MARTIN", 150, 40, sf::Color::White, 12);
						}
						
						int tiempo = modelo->getNivelActual()->getTiempoRestante();
						if (tiempo != -1) {
							sf::Color colorReloj = (tiempo <= 30) ? sf::Color::Red : sf::Color::White;
							dibujarTexto("TIEMPO: " + std::to_string(tiempo), 550, 40, colorReloj, 20);
							int curas = modelo->getNivelActual()->getCurasRestantes();
							dibujarTexto("CURAS: " + std::to_string(curas) + "/4", 900, 40, sf::Color::Cyan, 16);
						}
						
						static sf::RectangleShape panelInstrucciones(sf::Vector2f(800, 50));
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
				static sf::RectangleShape fondoOscuro(sf::Vector2f(1100, 600)); 
				fondoOscuro.setFillColor(sf::Color(0, 0, 0, 150)); 
				ventana.draw(fondoOscuro);
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
			
			static sf::RectangleShape celdaOro;
			static bool initOro = false;
			if(!initOro){
				celdaOro.setFillColor(sf::Color(255, 215, 0, 100)); 
				celdaOro.setOutlineColor(sf::Color(40, 40, 40)); 
				celdaOro.setOutlineThickness(1); 
				initOro = true;
			}
			celdaOro.setSize(sf::Vector2f(bloqueX, bloqueY));
			
			for (int i = 0; i < 20; i++) {       
				for (int j = 0; j < 30; j++) {   
					int contenido = 0; if (modelo->getNivelActual()) contenido = modelo->getNivelActual()->getContenidoCelda(j, i);
					if (contenido == 4) { 
						celdaOro.setPosition(j * bloqueX, i * bloqueY);
						ventana.draw(celdaOro);
					} 
				}
			}
			
			if (modelo->getNivelActual()) {
				const std::vector<Entidad*>& entidades = modelo->getNivelActual()->getEntidades();
				
				// Sombra reciclable para las rocas
				static sf::CircleShape sombraRoca(10.f);
				static bool initSombraRoca = false;
				if(!initSombraRoca){
					sombraRoca.setFillColor(sf::Color(0, 0, 0, 120)); 
					sombraRoca.setScale(1.0f, 0.4f); 
					initSombraRoca = true;
				}
				sombraRoca.setRadius(bloqueX * 0.35f);
				
				for (int fila = 0; fila < 20; fila++) {
					for (Entidad* e : entidades) {
						if (!e->estaVivo()) continue;
						if ((int)e->getY() == fila && e->getTipo() != "ARBOL" && e->getTipo() != "OBSTACULO_CONVENTO") {
							dibujarEntidadUnica(e, bloqueX, bloqueY);
						}
					}
					for (Entidad* e : entidades) {
						if (!e->estaVivo()) continue;
						if ((int)e->getY() == fila && (e->getTipo() == "ARBOL" || e->getTipo() == "OBSTACULO_CONVENTO")) {
							dibujarEntidadUnica(e, bloqueX, bloqueY);
						}
					}
					for (int col = 0; col < 30; col++) {
						if (modelo->getNivelActual()->getContenidoCelda(col, fila) == 5) {
							if (texRocaNieve.getSize().x > 0) {
								float factorEscala = 2.3f; 
								float escalaX = (bloqueX / texRocaNieve.getSize().x) * factorEscala;
								float escalaY = (bloqueY / texRocaNieve.getSize().y) * factorEscala;
								spriteRocaNieve.setScale(escalaX, escalaY);
								float anchoSprite = spriteRocaNieve.getGlobalBounds().width;
								float altoSprite = spriteRocaNieve.getGlobalBounds().height;
								float posX = (col * bloqueX) + (bloqueX / 2) - (anchoSprite / 2);
								float posY = (fila * bloqueY) + bloqueY - altoSprite + 10.0f; 
								
								sombraRoca.setPosition((col * bloqueX) + (bloqueX / 2) - sombraRoca.getGlobalBounds().width / 2, (fila * bloqueY) + bloqueY - sombraRoca.getGlobalBounds().height);
								ventana.draw(sombraRoca);
								
								spriteRocaNieve.setPosition(posX, posY);
								ventana.draw(spriteRocaNieve);
							}
						}
					}
				}
			}
			
			ventana.setView(ventana.getDefaultView());
			
			// --- OPTIMIZACIÓN DEL EFECTO DE NIEVE ---
			if (modelo->getNivelActual() && modelo->getNivelActual()->getArchivoFondo() == "fondos/andes.jpg") {
				// Reemplazamos los círculos pesados por Cuadrados Ligeros (sf::RectangleShape)
				static sf::RectangleShape copoShape(sf::Vector2f(4.0f, 4.0f)); 
				static bool initNieve = false;
				if(!initNieve) {
					copoShape.setFillColor(sf::Color(255, 255, 255, 200));
					initNieve = true;
				}
				
				for (auto& c : copos) {
					c.y += c.velY;
					c.x += c.velX;
					if (c.y > 600) { c.y = -10; c.x = (float)(rand() % 1100); }
					if (c.x < 0) c.x = 1100;
					if (c.x > 1100) c.x = 0;
					
					copoShape.setPosition(c.x, c.y);
					ventana.draw(copoShape);
				}
			}
		}
		
		void dibujarEntidadUnica(Entidad* e, float bloqueX, float bloqueY) {
			sf::Sprite* spritePtr = e->getSpriteRender();
			if (!spritePtr) return;
			
			// --- OPTIMIZACIÓN DE SOMBRAS Y BARRAS ---
			static sf::CircleShape sombra(10.f);
			static sf::RectangleShape fondoBarra;
			static sf::RectangleShape barraVerde;
			static sf::RectangleShape sableHolo;
			static sf::RectangleShape curaHolo;
			static bool initShapes = false;
			
			if(!initShapes){
				sombra.setFillColor(sf::Color(0, 0, 0, 120)); sombra.setScale(1.0f, 0.4f); 
				fondoBarra.setFillColor(sf::Color::Red); fondoBarra.setOutlineThickness(1); fondoBarra.setOutlineColor(sf::Color::Black);
				barraVerde.setFillColor(sf::Color::Green);
				
				sableHolo.setFillColor(sf::Color::Yellow);
				curaHolo.setFillColor(sf::Color::White); curaHolo.setOutlineThickness(2.0f); curaHolo.setOutlineColor(sf::Color::Red);
				
				initShapes = true;
			}
			
			sombra.setRadius(bloqueX * 0.35f);
			float sombraX = (e->getX() * bloqueX) + (bloqueX / 2) - sombra.getGlobalBounds().width / 2;
			float sombraY = (e->getY() * bloqueY) + bloqueY - sombra.getGlobalBounds().height;
			sombra.setPosition(sombraX, sombraY);
			ventana.draw(sombra);
			
			if (spritePtr->getTexture() == nullptr) {
				if (e->getTipo() == "ITEM_SABLE") {
					sableHolo.setSize(sf::Vector2f(bloqueX * 0.8f, 6.0f));
					sableHolo.setPosition((e->getX() * bloqueX) + (bloqueX * 0.1f), (e->getY() * bloqueY) + bloqueY - 10.0f);
					ventana.draw(sableHolo);
				} 
				else if (e->getTipo() == "ITEM_CURACION") {
					curaHolo.setSize(sf::Vector2f(bloqueX * 0.5f, bloqueY * 0.5f));
					curaHolo.setPosition((e->getX() * bloqueX) + (bloqueX * 0.25f), (e->getY() * bloqueY) + (bloqueY * 0.5f));
					ventana.draw(curaHolo);
				}
				return; 
			}
			
			Personaje* p = dynamic_cast<Personaje*>(e);
			float factorEscala = 1.5f; 
			
			if (p == nullptr) {
				factorEscala = 2.3f; 
				if (e->getTipo() == "OBSTACULO_CONVENTO") factorEscala = 20.0f; 
				else if (e->getTipo() == "OBSTACULO_PILAR") factorEscala = 10.0f; 
				else if (e->getTipo() == "ITEM_SABLE") factorEscala = 1.0f; 
				else if (e->getTipo() == "ITEM_CURACION") factorEscala = 0.5f;
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
				posY += 10.0f; 
				if (e->getTipo() == "OBSTACULO_CONVENTO") posY += 20.0f; 
				else if (e->getTipo() == "OBSTACULO_PILAR") posY += 10.0f;
			}
			
			spritePtr->setPosition(posX, posY);
			ventana.draw(*spritePtr);
			
			if (p && e->getTipo() != "PROCER") {
				float anchoBarra = bloqueX * 0.8f; float altoBarra = 6.0f;            
				float porcentajeVida = p->getVida() / p->getVidaMax();
				if (porcentajeVida < 0) porcentajeVida = 0;
				
				fondoBarra.setSize(sf::Vector2f(anchoBarra, altoBarra));
				barraVerde.setSize(sf::Vector2f(anchoBarra * porcentajeVida, altoBarra));
				
				float barraX = (e->getX() * bloqueX) + (bloqueX / 2) - (anchoBarra / 2); float barraY = posY - 8.0f; 
				fondoBarra.setPosition(barraX, barraY); 
				barraVerde.setPosition(barraX, barraY); 
				
				ventana.draw(fondoBarra); ventana.draw(barraVerde);
			}
		}
		
		void dibujarTexto(std::string mensaje, float x, float y, sf::Color color, int tam) {
			static sf::Text texto; // Reutilizamos el objeto de texto
			texto.setFont(fuente); texto.setString(mensaje); texto.setCharacterSize(tam); texto.setFillColor(color);
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
