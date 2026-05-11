#include "../include/Button.h"

Button::Button(sf::Vector2f position, sf::Vector2f size,
               const std::string &label, sf::Font &font, int fontSize)
{
    this->currentState = ButtonState::Normal;
    this->text.setFont(font);
    this->text.setString(label);
    this->text.setCharacterSize(fontSize);
    this->shape.setSize(size);
    this->shape.setPosition(position);
    normalColor = sf::Color(70, 70, 70);
    hoverColor = sf::Color(100, 100, 100);
    selectedColor = sf::Color(50, 100, 200);
    this->shape.setFillColor(normalColor);
    centerText();
}
void Button::draw(sf::RenderWindow &window)
{
    switch (getState())
    {
    case (ButtonState::Hover):
    {
        this->shape.setFillColor(hoverColor);
        break;
    }
    case (ButtonState::Selected):
    {
        this->shape.setFillColor(selectedColor);
        break;
    }
    default:
    {
        this->shape.setFillColor(normalColor);
        break;
    }
    }
    window.draw(this->shape);
    window.draw(this->text);
}

void Button::update(sf::RenderWindow &window) {
    if (currentState == ButtonState::Selected)
        return;
    
    if (isMouseOver(window))
        currentState = ButtonState::Hover;
    else
        currentState = ButtonState::Normal;
}

bool Button::isClicked(sf::RenderWindow &window, sf::Event &event) {
    return isMouseOver(window)
        && event.type == sf::Event::MouseButtonPressed
        && event.mouseButton.button == sf::Mouse::Left;
}

void Button::setSelected(bool selected)
{
    if (selected)
        this->currentState = ButtonState::Selected;
    else
        this->currentState = ButtonState::Normal;
}

bool Button::isSelected() const {
    return currentState == ButtonState::Selected;
}

ButtonState Button::getState() const
{
    return this->currentState;
}

void Button::setNormalColor(sf::Color color)
{
    this->normalColor = color;
}
void Button::setHoverColor(sf::Color color)
{
    this->hoverColor = color;
}
void Button::setSelectedColor(sf::Color color)
{
    this->selectedColor = color;
}
void Button::setTextColor(sf::Color color)
{
    this->text.setFillColor(color);
}
void Button::setBorderColor(sf::Color color)
{
    this->shape.setOutlineColor(color);
}
void Button::setBorderThickness(float thickness)
{
    this->shape.setOutlineThickness(thickness);
}

void Button::setPosition(sf::Vector2f position)
{
    this->shape.setPosition(position);
}
sf::FloatRect Button::getBounds() const
{
    return this->shape.getGlobalBounds();
}

void Button::centerText()
{
    sf::FloatRect textBounds = text.getLocalBounds();
    sf::Vector2f shapePos = shape.getPosition();
    sf::Vector2f shapeSize = shape.getSize();

    text.setPosition(
        shapePos.x + (shapeSize.x - textBounds.width) / 2.f - textBounds.left,
        shapePos.y + (shapeSize.y - textBounds.height) / 2.f - textBounds.top);
}

bool Button::isMouseOver(sf::RenderWindow &window) const
{
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePos = window.mapPixelToCoords(mousePixelPos);

    return shape.getGlobalBounds().contains(mousePos);
}