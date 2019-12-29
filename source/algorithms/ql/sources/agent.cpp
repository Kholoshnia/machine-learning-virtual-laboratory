#include <QL/Header Files/QL_Agent.h>

ql::Agent::Agent()
{
	rect.setFillColor(sf::Color::Black);
	rect.setSize(sf::Vector2f(static_cast<float>(height / map_size.x), static_cast<float>(width / map_size.y)));
}

void ql::Agent::update(const int n)
{
	pos = sf::Vector2f(static_cast<float>((n - map_size.x * (n / map_size.x)) * (width / map_size.y)), static_cast<float>((n / map_size.x) * (height / map_size.x)));
}

void ql::Agent::show(sf::RenderWindow& window)
{
	rect.setPosition(pos);
	window.draw(rect);
}
