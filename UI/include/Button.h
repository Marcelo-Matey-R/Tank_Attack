#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class ButtonState
{
    Normal,
    Hover,
    Selected
};

class Button
{
public:
    Button(sf::Vector2f position, sf::Vector2f size,
           const std::string &label, sf::Font &font, int fontSize);

    // dibuja el botón en la ventana
    void draw(sf::RenderWindow &window);

    // actualiza el estado según la posición del mouse
    void update(sf::RenderWindow &window);

    // devuelve true si fue clickeado en este frame
    bool isClicked(sf::RenderWindow &window, sf::Event &event);

    /* 
        manejo de estado, lo que hace es ver si se selecciono ese boton 
        y tambien otorga el estado actual del boton
    */
    void setSelected(bool selected);
    bool isSelected() const;
    ButtonState getState() const;

    // personalización de colores
    void setNormalColor(sf::Color color);
    void setHoverColor(sf::Color color);
    void setSelectedColor(sf::Color color);
    void setTextColor(sf::Color color);
    void setBorderColor(sf::Color color);
    void setBorderThickness(float thickness);

    // posición y tamaño
    void setPosition(sf::Vector2f position);
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    sf::Text text;

    ButtonState currentState;

    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color selectedColor;

    // centra el texto dentro del botón
    void centerText();
    // verifica si el mouse está sobre el botón
    bool isMouseOver(sf::RenderWindow &window) const;
};