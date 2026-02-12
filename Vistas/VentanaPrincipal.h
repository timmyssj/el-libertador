#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm> // Necesario para std::max o std::min
#include "../Modelo/juego.h" // Cuidado con may�sculas/min�sculas en el include

class VentanaPrincipal {
private:
	sf::RenderWindow ventana;
	Juego* modelo; 
	sf::Font fuente; 
	
public:
	VentanaPrincipal(Juego* j) : modelo(j) {
		ventana.create(sf::VideoMode(1100, 600), "San Martin: El Libertador");
		ventana.setFramerateLimit(60);
		
		if (!fuente.loadFromFile("PressStart2P.ttf")) {
			// Manejo de error si no hay fuente
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
				dibujarTexto("EL LIBERTADOR", 550, 100, sf::Color::Cyan, 30);
				dibujarMenuGenerico(modelo->getMenu(), 550, 250);
			} 
			else if (estado == SELECCION_NIVEL) {
				dibujarTexto("SELECCIONAR NIVEL", 550, 100, sf::Color::Yellow, 30);
				
				Menu* menu = modelo->getMenuNiveles();
				// --- CORRECCI�N CR�TICA AQU�: getOpciones() ---
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
						mensaje = "�OBJETIVO CUMPLIDO! Ve a la zona Dorada";
						colorTexto = sf::Color::Green; 
					}
					dibujarTexto(mensaje, 400, 565, colorTexto, 18);
				}
			}
			else if (estado == PAUSA) {
				dibujarJuego(); 
				sf::RectangleShape fondoOscuro(sf::Vector2f(1100, 600)); // Ajustado a tama�o ventana
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
				dibujarTexto("�DERROTA!", 550, 200, sf::Color::Red, 60);
				dibujarTexto("Presiona ESC para volver al Men�", 550, 400, sf::Color::White, 20);
			}
			else if (estado == VICTORIA) {
				dibujarTexto("�VICTORIA!", 550, 200, sf::Color::Green, 60);
				dibujarTexto("Has liberado esta zona.", 550, 300, sf::Color::White, 20);
				dibujarTexto("Presiona ESC para volver al Men�", 550, 400, sf::Color::White, 20);
			}
			
			ventana.display(); 
		}
		
		// --- FUNCIONES AYUDANTES ---
		
		void dibujarJuego() {
			float anchoVentana = (float)ventana.getSize().x;
			float altoVentana = (float)ventana.getSize().y;
			
			float bloqueX = anchoVentana / 30.0f; 
			float bloqueY = altoVentana / 20.0f;  
			
			for (int i = 0; i < 20; i++) {       
				for (int j = 0; j < 30; j++) {   
					sf::RectangleShape celda(sf::Vector2f(bloqueX, bloqueY));
					celda.setPosition(j * bloqueX, i * bloqueY);
					
					int contenido = 0;
					if (modelo->getNivelActual()) {
						contenido = modelo->getNivelActual()->getContenidoCelda(j, i);
					}
					
					if (contenido == 1) { // PARED
						celda.setFillColor(sf::Color(100, 100, 100)); 
						celda.setOutlineThickness(-1); celda.setOutlineColor(sf::Color::Black);
					} 
					else if (contenido == 4) { // SALIDA
						celda.setFillColor(sf::Color(255, 215, 0, 150)); 
					}
					else { // SUELO
						celda.setFillColor(sf::Color(50, 50, 50)); 
						celda.setOutlineThickness(1); celda.setOutlineColor(sf::Color(60, 60, 60)); 
					}
					ventana.draw(celda);
				}
			}
			
			if (modelo->getNivelActual()) {
				const std::vector<Entidad*>& entidades = modelo->getNivelActual()->getEntidades();
				float radio = (std::min(bloqueX, bloqueY) / 2.5f);
				
				for (Entidad* e : entidades) {
					if (!e->estaVivo()) continue;
					
					sf::CircleShape forma(radio); 
					float posX = e->getX() * bloqueX + (bloqueX / 2) - radio;
					float posY = e->getY() * bloqueY + (bloqueY / 2) - radio;
					forma.setPosition(posX, posY);
					
					if (e->getTipo() == "PROCER") forma.setFillColor(sf::Color::White);
					else if (e->getTipo() == "REALISTA") forma.setFillColor(sf::Color::Red);
					else if (e->getTipo() == "ALIADO") forma.setFillColor(sf::Color::Blue);
					else if (e->getTipo() == "PRACTICA") forma.setFillColor(sf::Color(139, 69, 19)); 
					
					ventana.draw(forma);
				}
			}
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
