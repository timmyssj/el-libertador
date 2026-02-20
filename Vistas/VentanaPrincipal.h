#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm> // Necesario para std::max o std::min
#include "../Modelo/juego.h" // Cuidado con mayï¿½sculas/minï¿½sculas en el include

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
	
	//camara
	sf::View camara;
	
public:
	VentanaPrincipal(Juego* j) : modelo(j) {
		ventana.create(sf::VideoMode(1100, 600), "San Martin: El Libertador");
		ventana.setFramerateLimit(60);
		
		if (!fuente.loadFromFile("PressStart2P.ttf")) {// Manejo de error si no hay fuente
			
		}
		// REEMPLAZA 'tu_imagen_de_fondo.png' POR EL NOMBRE REAL DE TU ARCHIVO
		if (!texturaFondoMenu.loadFromFile("fondos/pantalla1.png")) {
			std::cout << "Error: No se pudo cargar la imagen de fondo del menú." << std::endl;
			// Si falla, no pasa nada grave, simplemente se verá negro.
		} else {
			// Si cargó bien, se la asignamos al sprite
			spriteFondoMenu.setTexture(texturaFondoMenu);
			
			// OPCIONAL: Si tu imagen no es del mismo tamaño que la ventana, 
			// puedes descomentar estas líneas para que se estire y la cubra toda.
			
			float escalaX = (float)ventana.getSize().x / texturaFondoMenu.getSize().x;
			float escalaY = (float)ventana.getSize().y / texturaFondoMenu.getSize().y;
			spriteFondoMenu.setScale(escalaX, escalaY);
			
		}
		
		// Le damos el tamaño de la ventana y hacemos un zoom (0.6 = más cerca)
		camara.setSize((float)ventana.getSize().x, (float)ventana.getSize().y);
		camara.zoom(0.6f);
		// ---------------------------------------------------
		
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
					case sf::Keyboard::W:     
					case sf::Keyboard::Up:    
						modelo->procesarTeclaArriba(); 
					break;
					case sf::Keyboard::S:     
					case sf::Keyboard::Down:  
						modelo->procesarTeclaAbajo(); 
					break;
					case sf::Keyboard::A:     
					case sf::Keyboard::Left:  
						modelo->procesarTeclaIzquierda(); 
					break;
					case sf::Keyboard::D:     
					case sf::Keyboard::Right: 
						modelo->procesarTeclaDerecha(); 
					break;
					case sf::Keyboard::Space: 
						modelo->atacarConSanMartin(); 
					break;
					case sf::Keyboard::Return:
						modelo->procesarTeclaEnter(); 
					break;
					case sf::Keyboard::Escape: 
						modelo->teclaEscape(); 
					break;
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
				if (spriteFondoMenu.getTexture() != nullptr) {
					ventana.draw(spriteFondoMenu);
				}
				dibujarTexto("EL LIBERTADOR", 550, 100, sf::Color::Cyan, 30);
				dibujarMenuGenerico(modelo->getMenu(), 550, 250);
			} 
			else if (estado == SELECCION_NIVEL) {
				dibujarTexto("SELECCIONAR NIVEL", 550, 100, sf::Color::Yellow, 30);
				
				Menu* menu = modelo->getMenuNiveles();
				// --- CORRECCIï¿½N CRï¿½TICA AQUï¿½: getOpciones() ---
				std::vector<std::string> opciones = menu->getOpciones(); 
				// ----------------------------------------------
				
				int desbloqueados = modelo->getNivelMaximo();
				
				for (size_t i = 0; i < opciones.size(); i++) {
					sf::Color colorTexto;
					std::string texto = opciones[i];
					
					if (i == (size_t)menu->getOpcionActual()) {
						colorTexto = sf::Color::Cyan; 
					} 
					else if ((int)i > desbloqueados && i != opciones.size() - 1) { 
						colorTexto = sf::Color(100, 100, 100); 
						texto += " (BLOQUEADO)";
					} 
					else {
						colorTexto = sf::Color::White; 
					}
					
					dibujarTexto(texto, 550, 200 + (i * 50), colorTexto, 20);
				}
			}
			else if (estado == INTRO_HISTORIA) {
				if (modelo->esUltimaPaginaIntro()) {
					dibujarTexto(modelo->getTituloActual(), 550, 100, sf::Color::Cyan, 30);
				} 
				else {
					dibujarTexto("CONTEXTO HISTORICO", 550, 100, sf::Color::Yellow, 30);
				}
				std::string texto = modelo->getTextoHistoria();
				dibujarTexto(texto, 550, 300, sf::Color::White, 17);
				dibujarTexto("Presiona ENTER para continuar...", 750, 500, sf::Color::Cyan, 15);
			}
			else if (estado == JUGANDO) {
				dibujarJuego();
				
				if (modelo->getNivelActual() && modelo->getNivelActual()->getHeroe()) {
					int vida = modelo->getNivelActual()->getHeroe()->getVida();
					int vidaMax = 100; 
					
					sf::RectangleShape fondoBarra(sf::Vector2f(200, 20)); 
					fondoBarra.setPosition(50, 30);
					fondoBarra.setFillColor(sf::Color(50, 0, 0));
					fondoBarra.setOutlineThickness(2);
					fondoBarra.setOutlineColor(sf::Color::White);
					ventana.draw(fondoBarra);
					
					float porcentaje = (float)vida / vidaMax;
					if (porcentaje < 0) porcentaje = 0;
					
					sf::RectangleShape barraActual(sf::Vector2f(200 * porcentaje, 20));
					barraActual.setPosition(50, 30);
					
					if (vida > 50) barraActual.setFillColor(sf::Color::Green);
					else if (vida > 25) barraActual.setFillColor(sf::Color::Yellow);
					else barraActual.setFillColor(sf::Color::Red);
					
					ventana.draw(barraActual);
					dibujarTexto("SAN MARTIN", 150, 40, sf::Color::White, 12);
					
					sf::RectangleShape panelInstrucciones(sf::Vector2f(800, 50));
					panelInstrucciones.setPosition(0, 550); 
					panelInstrucciones.setFillColor(sf::Color(0, 0, 0, 150));
					ventana.draw(panelInstrucciones);
					
					std::string mensaje;
					sf::Color colorTexto;
					
					if (modelo->getNivelActual()->hayEnemigosVivos()) {
						mensaje = "OBJETIVO: Destruye los maniquies (ESPACIO)";
						colorTexto = sf::Color::White;
					} else {
						mensaje = "ï¿½OBJETIVO CUMPLIDO! Ve a la zona Dorada";
						colorTexto = sf::Color::Green; 
					}
					dibujarTexto(mensaje, 400, 565, colorTexto, 18);
				}
			}
			else if (estado == PAUSA) {
				dibujarJuego(); 
				sf::RectangleShape fondoOscuro(sf::Vector2f(1100, 600)); // Ajustado a tamaï¿½o ventana
				fondoOscuro.setFillColor(sf::Color(0, 0, 0, 150)); 
				ventana.draw(fondoOscuro);
				
				dibujarTexto("JUEGO PAUSADO", 550, 100, sf::Color::Yellow, 30);
				dibujarMenuGenerico(modelo->getMenuPausa(), 550, 300);
			}
			else if (estado == CONFIGURACION) {
				dibujarTexto("CONFIGURACION", 550, 100, sf::Color::Cyan, 30);
				dibujarMenuGenerico(modelo->getMenuConfig(), 550, 200);
				dibujarTexto("Usa IZQ/DER pa   ra cambiar valor", 250, 500, sf::Color::White, 15);
			}
			else if (estado == GAME_OVER) {
				dibujarTexto("ï¿½DERROTA!", 550, 200, sf::Color::Red, 60);
				dibujarTexto("Presiona ESC para volver al Menï¿½", 550, 400, sf::Color::White, 20);
			}
			else if (estado == VICTORIA) {
				dibujarTexto("ï¿½VICTORIA!", 550, 200, sf::Color::Green, 60);
				dibujarTexto("Has liberado esta zona.", 550, 300, sf::Color::White, 20);
				dibujarTexto("Presiona ESC para volver al Menï¿½", 550, 400, sf::Color::White, 20);
			}
			
			ventana.display(); 
		}
		
		// --- FUNCIONES AYUDANTES ---
		
		void dibujarJuego() {
			// Usamos el tamaño por defecto de la ventana para calcular los bloques
			// Esto asegura que el mapa mida lo mismo, pero la cámara lo vea de cerca
			float anchoVentana = (float)ventana.getDefaultView().getSize().x;
			float altoVentana = (float)ventana.getDefaultView().getSize().y;
			float bloqueX = anchoVentana / 30.0f; 
			float bloqueY = altoVentana / 20.0f;  
			
			// --- 1. LÓGICA DE LA CÁMARA ---
			if (modelo->getNivelActual() && modelo->getNivelActual()->getHeroe()) {
				SanMartin* heroe = modelo->getNivelActual()->getHeroe();
				
				// Calculamos el centro exacto de San Martín en píxeles
				float heroePixelX = (heroe->getX() * bloqueX) + (bloqueX / 2);
				float heroePixelY = (heroe->getY() * bloqueY) + (bloqueY / 2);
				
				// LIMITES DE LA CÁMARA (Para no salirse del mapa)
				float mitadCamaraX = camara.getSize().x / 2.0f;
				float mitadCamaraY = camara.getSize().y / 2.0f;
				
				float limiteDerecho = (30 * bloqueX) - mitadCamaraX;
				float limiteInferior = (20 * bloqueY) - mitadCamaraY;
				
				// Chocamos la cámara contra los bordes si intenta salir
				if (heroePixelX < mitadCamaraX) heroePixelX = mitadCamaraX;
				if (heroePixelX > limiteDerecho) heroePixelX = limiteDerecho;
				if (heroePixelY < mitadCamaraY) heroePixelY = mitadCamaraY;
				if (heroePixelY > limiteInferior) heroePixelY = limiteInferior;
				
				// Centramos la cámara y se la aplicamos a la ventana
				camara.setCenter(heroePixelX, heroePixelY);
				ventana.setView(camara);
			}
			// ------------------------------
			
			// 2. DIBUJAR MAPA
			for (int i = 0; i < 20; i++) {       
				for (int j = 0; j < 30; j++) {   
					sf::RectangleShape celda(sf::Vector2f(bloqueX, bloqueY));
					celda.setPosition(j * bloqueX, i * bloqueY);
					
					int contenido = 0;
					if (modelo->getNivelActual()) 
						contenido = modelo->getNivelActual()->getContenidoCelda(j, i);
					
					if (contenido == 1) celda.setFillColor(sf::Color(100, 100, 100)); 
					else if (contenido == 4) celda.setFillColor(sf::Color(255, 215, 0, 100)); 
					else celda.setFillColor(sf::Color(50, 50, 50)); 
					
					celda.setOutlineThickness(1);
					celda.setOutlineColor(sf::Color(40, 40, 40));
					ventana.draw(celda);
				}
			}
			
			// 3. DIBUJAR PERSONAJES (Tu algoritmo de Profundidad 2.5D se mantiene igual)
			if (modelo->getNivelActual()) {
				const std::vector<Entidad*>& entidades = modelo->getNivelActual()->getEntidades();
				
				for (int fila = 0; fila < 20; fila++) {
					for (Entidad* e : entidades) {
						if (!e->estaVivo()) continue;
						
						if ((int)e->getY() == fila) {
							Personaje* p = dynamic_cast<Personaje*>(e);
							
							if (p) {
								// SOMBRA
								sf::CircleShape sombra(bloqueX * 0.35f); 
								sombra.setFillColor(sf::Color(0, 0, 0, 120)); 
								sombra.setScale(1.0f, 0.4f); 
								
								float sombraX = (p->getX() * bloqueX) + (bloqueX / 2) - sombra.getGlobalBounds().width / 2;
								float sombraY = (p->getY() * bloqueY) + bloqueY - sombra.getGlobalBounds().height;
								sombra.setPosition(sombraX, sombraY);
								ventana.draw(sombra);
								
								// SPRITE
								sf::Sprite& sprite = p->getSprite(); 
								float factorEscala = 1.5f; 
								
								if (sprite.getTexture()) {
									float escalaX = (bloqueX / sprite.getTexture()->getSize().x) * factorEscala;
									float escalaY = (bloqueY / sprite.getTexture()->getSize().y) * factorEscala;
									sprite.setScale(escalaX, escalaY);
								}
								
								float anchoSprite = sprite.getGlobalBounds().width;
								float altoSprite = sprite.getGlobalBounds().height;
								
								float posX = (p->getX() * bloqueX) + (bloqueX / 2) - (anchoSprite / 2);
								float posY = (p->getY() * bloqueY) + bloqueY - altoSprite;
								
								// COMPENSACIÓN (El parche que pusimos antes)
								if (e->getTipo() == "PROCER") posX += 1.0f; 
								else posY += 8.0f;  
								
								sprite.setPosition(posX, posY);
								ventana.draw(sprite);
								
								// BARRAS DE VIDA ENEMIGAS
								if (e->getTipo() != "PROCER") {
									float anchoBarra = bloqueX * 0.8f; 
									float altoBarra = 6.0f;            
									float porcentajeVida = p->getVida() / p->getVidaMax();
									if (porcentajeVida < 0) porcentajeVida = 0;
									
									sf::RectangleShape fondoBarra(sf::Vector2f(anchoBarra, altoBarra));
									fondoBarra.setFillColor(sf::Color::Red);
									float barraX = (p->getX() * bloqueX) + (bloqueX / 2) - (anchoBarra / 2);
									float barraY = posY - 8.0f; 
									fondoBarra.setPosition(barraX, barraY);
									fondoBarra.setOutlineThickness(1);
									fondoBarra.setOutlineColor(sf::Color::Black);
									
									sf::RectangleShape barraVerde(sf::Vector2f(anchoBarra * porcentajeVida, altoBarra));
									barraVerde.setFillColor(sf::Color::Green);
									barraVerde.setPosition(barraX, barraY);
									
									ventana.draw(fondoBarra);
									ventana.draw(barraVerde);
								}
							}
						}
					}
				}
			}
			
			// --- 4. MUY IMPORTANTE: RESETEAR LA VISTA PARA LA UI ---
			// Al terminar de dibujar el mundo 2D, regresamos a la vista normal
			// para que textos como "OBJETIVO: ..." no queden gigantes ni se muevan.
			ventana.setView(ventana.getDefaultView());
			// -------------------------------------------------------
		}
		
		void dibujarTexto(std::string mensaje, float x, float y, sf::Color color, int tam) {
			sf::Text texto;
			texto.setFont(fuente);
			texto.setString(mensaje);
			texto.setCharacterSize(tam);
			texto.setFillColor(color);
			sf::FloatRect bounds = texto.getLocalBounds();
			texto.setOrigin(bounds.width / 2, bounds.height / 2);
			texto.setPosition(x, y);
			ventana.draw(texto);
		}
		
		void dibujarMenuGenerico(Menu* menu, float xBase, float yBase) {
			if (!menu) return;
			for (int i = 0; i < menu->getCantidadOpciones(); i++) {
				sf::Color color = sf::Color::White;
				if (i == menu->getOpcionActual()) color = sf::Color::Red; 
				dibujarTexto(menu->getTextoOpcion(i), xBase, yBase + (i * 50), color, 20);
			}
		}
};

#endif
