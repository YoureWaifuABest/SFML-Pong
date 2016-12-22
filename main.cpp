#include <SFML/Graphics.hpp>
#include <iostream>

struct velocity {
	double speedx;
	double speedy;
	int directionx;
	int directiony;
};

struct scores {
	unsigned int p1;
	unsigned int p2;
};

class object 
{
public:
	sf::RectangleShape rect;
	double bot, left, right, top;

	object(sf::Vector2f position, sf::Vector2f size, sf::Color color)
	{
		rect.setPosition(position);
		rect.setSize(size);
		rect.setFillColor(color);
	}

	void update()
	{
		bot   = rect.getPosition().y + rect.getSize().y;
		left  = rect.getPosition().x;
		right = rect.getPosition().x + rect.getSize().x;
		top   = rect.getPosition().y;
	}

	bool collision(object p)
	{
		if (right < p.left || left > p.right || top > p.bot || bot < p.top)
			return false;
		return true;
	}
};

int main(void)
{
	sf::RenderWindow window(sf::VideoMode(200, 240), "Pong");
	sf::View view(sf::FloatRect(0, 0, 200, 240));

	struct velocity ball_velocity;
	struct scores   scores;
	object p1(object(sf::Vector2f(10,  75),    sf::Vector2f(5, 50), sf::Color::White)),
	       p2(object(sf::Vector2f(185, 75),    sf::Vector2f(5, 50), sf::Color::White)),
	       ball(object(sf::Vector2f(100, 100), sf::Vector2f(2, 2),  sf::Color::White));

	ball_velocity.directionx = -1;
	ball_velocity.speedx     = 1;
	ball_velocity.directiony = 0;
	ball_velocity.speedy     = 0;

	scores.p1 = 0;
	scores.p2 = 0;

	window.setVerticalSyncEnabled(true);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;

				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape)
						window.close();
					break;

				default:
					break;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && p1.bot < 200)
			p1.rect.move(0,  5);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)   && p1.top > 0)
			p1.rect.move(0, -5);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)    && p2.bot < 200)
			p2.rect.move(0,  5);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)    && p2.top > 0)
			p2.rect.move(0, -5);

		ball.rect.move(ball_velocity.directionx * ball_velocity.speedx, ball_velocity.directiony * ball_velocity.speedy);
		if (p1.collision(ball)) {
			ball_velocity.speedx    *= 1.08;
			ball_velocity.directionx = 1;
			if (ball.top < ((p1.bot + p1.top) / 2)) {
				if (!ball_velocity.speedy)
					ball_velocity.speedy   = .25;
				ball_velocity.directiony = -1;
				ball_velocity.speedy    *= 1.02;
			}
			else {
				if (!ball_velocity.speedy)
					ball_velocity.speedy   = .25;
				ball_velocity.directiony = 1;
				ball_velocity.speedy    *= 1.02;
			}
		}
		if (p2.collision(ball)) {
			ball_velocity.directionx = -1;
			ball_velocity.speedx    *= 1.08;
			if (ball.top < ((p2.bot + p2.top) / 2)) {
				if (!ball_velocity.speedy)
					ball_velocity.speedy   = .25;
				ball_velocity.directiony = -1;
				ball_velocity.speedy    *= 1.02;
			}
			else {
				if (!ball_velocity.speedy)
					ball_velocity.speedy   = .25;
				ball_velocity.directiony = 1;
				ball_velocity.speedy    *= 1.02;
			}
		}

		if (ball_velocity.speedx > 3)
			ball_velocity.speedx = 3;
		if (ball_velocity.speedy > 3)
			ball_velocity.speedy = 3;

		if (ball.bot >= 200)
			ball_velocity.directiony = -1;
		if (ball.top <= 0)
			ball_velocity.directiony =  1;

		if (ball.left  <= 0) {
			scores.p2++;
			std::cout << "Player 2 points: " << scores.p2 << std::endl;
			ball.rect.setPosition(100, 100);	
			ball_velocity.directiony = 0;
			ball_velocity.speedy     = 0;
			ball_velocity.directionx = 1;
			ball_velocity.speedx     = 1;
		}
		if (ball.right >= 200) {
			scores.p1++;
			std::cout << "Player 1 points: " << scores.p1 << std::endl;
			ball.rect.setPosition(100, 100);
			ball_velocity.directiony = 0;
			ball_velocity.speedy     = 0;
			ball_velocity.directionx = -1;
			ball_velocity.speedx     =  1;
		}

		p1.update();
		p2.update();
		ball.update();

		window.clear(sf::Color::Black);
		window.setView(view);
		window.draw(p1.rect);
		window.draw(p2.rect);
		window.draw(ball.rect);
		window.display();
	}
}
