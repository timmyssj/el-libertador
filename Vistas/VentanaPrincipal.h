#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <SFML/Graphics.hpp>
#include "../Modelo/Juego.h" // La Vista conoce al Modelo

class VentanaPrincipal {
private:
	sf::RenderWindow ventana;
	Juego* modelo; // Referencia a la lógica del juego
	
public:
	VentanaPrincipal(Juego* j) : modelo(j) {
		// Crear la ventana de 800x600 con título
		ventana.create(sf::VideoMode(800, 600), "San Martin: El Libertador");
		ventana.setFramerateLimit(60); // Limitar a 60 FPS
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
				
				// Ejemplo: Capturar tecla P para pausar
				if (evento.type == sf::Event::KeyPressed) {
					if (evento.key.code == sf::Keyboard::P) {
						modelo->pausarOReanudar(); // Ordenamos al modelo
					}
				}
			}
		}
		
		void actualizar() {
			// La vista NO calcula nada, solo le dice al modelo que avance el tiempo
			modelo->actualizar();
		}
		
		void renderizar() {
			ventana.clear(sf::Color::Black); // Limpiar pantalla
			
			// DIBUJAR TODO AQUÍ
			// Ejemplo (temporal): Un cuadrado rojo si está en pausa
			if (modelo->estaEnPausa()) {
				sf::RectangleShape rect(sf::Vector2f(100, 100));
				rect.setFillColor(sf::Color::Red);
				ventana.draw(rect);
			}
			
			ventana.display(); // Mostrar frame
		}
};

#endif
