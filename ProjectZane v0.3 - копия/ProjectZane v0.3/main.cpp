#include <SFML/Graphics.hpp>
#include <ltbl/lighting/LightSystem.h>
#include <SFML/Audio.hpp>
//#include "Zane.h"

using namespace sf;

float xOffset = 0, yOffset = 0, os = 0.1;
const int lobbyheight = 23, lobbywidth = 100;
const int xCentre = 640, yCentre = 360;
float xPlayerPos = xCentre, yPlayerPos = yCentre;
String LobbyLvl[lobbyheight] = {
	"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH", //+(7,12)
	"H                                                BB                                B               H",
	"H                                                                                                  H",
	"H                                                                                                  H",
	"H                                                BB                                B               H",
	"H                                  BBBBBBBBBBB  BBBBBBBBBBBBB                      B               H",
	"H         BBBBBBBBBBBBBBBBBBBBBB   BBBBBBBBBBB  BBBBBBBBBBBBB                      B               H",
	"H         B                    B   BB                      BBBBBBBBBBBBBBB   BBBBBBBBBBBBBBBBBBBBBBH",
	"H         B                    B   BB                      BB            B   B                     H",
	"H         B                    B   BB                      BB                B                     H",
	"H         B       H  H         B   BB                      BB                B                     H",
	"H         B       H  H         B   BB                      BB            B   B                     H",
	"H         B       H  H         B   BB                      BB            B   B                     H",
	"H         B       HHHH         B   BB                      BBBBBBBBBBBBBBB   B                     H",
	"H         B                    B   BB                      BB            B   B                     H",
	"H         B                    B   BB                      BB                B                     H",
	"H         BBBBBBBBBBB  BBBBBBBBB   BB                      BB                B                     H",
	"HBB  BBBBBBBHHHHHHHBB  BBHHHHHBBBBBBB                      BB            B   B                     H",
	"H       BBBBBBBBBBBBB  BBBBBBBBBBBBBB                      BBBBBBBBBBBBBBB   BBB                   H",
	"H       BB                                                 BB            B                         H",
	"H       BB                                                 BB                                      H",
	"H       BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB                BBB                   H",
	"HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH",
};

class Zane
{
public:
	float xSpeed, ySpeed;
	char SightDirection;
	FloatRect rect;
	FloatRect SpellRect;
	Sprite sprite, SpellSprite;
	float CurrentFrame;
	bool IsxCollided, IsyCollided;
	float Health = 100;
	float Stamina = 100;
	bool isCasted = true;

	Zane(Texture &image)
	{
		sprite.setTexture(image);
		rect = FloatRect(xCentre-32, yCentre-32, 62, 62); //положение и размеры геро€

		SightDirection = 'S';

		sprite.setTextureRect(IntRect(0, 0, 64, 64));
		sprite.setPosition(xCentre, yCentre);

		xSpeed = ySpeed = 0.0;
		CurrentFrame = 0;

		IsxCollided = false;
		IsyCollided = false;
	}

	void update(float time)
	{
		IsxCollided = false;
		IsyCollided = false;

		rect.left += xSpeed * time;
		Collision(1, time);

		rect.top += ySpeed * time;
		Collision(0, time);

		CurrentFrame += time * 0.004;
		if (CurrentFrame > 4)
			CurrentFrame = 0;

		if (xSpeed != 0 || ySpeed != 0)
		{
			if (SightDirection == 'S' && ySpeed > 0)
				sprite.setTextureRect(IntRect(0, (int)CurrentFrame * 64, 64, 64));
			if (SightDirection == 'N' && ySpeed < 0)
				sprite.setTextureRect(IntRect(64, (int)CurrentFrame * 64, 64, 64));
			if (SightDirection == 'E' && xSpeed > 0)
				sprite.setTextureRect(IntRect(128, (int)CurrentFrame * 64, 64, 64));
			if (SightDirection == 'W' && xSpeed < 0)
				sprite.setTextureRect(IntRect(192, (int)CurrentFrame * 64, 64, 64));
		}
		if (xSpeed == 0 && ySpeed == 0)
		{
			if (SightDirection == 'N') sprite.setTextureRect(IntRect(64, 0, 64, 64));
			if (SightDirection == 'E') sprite.setTextureRect(IntRect(128, 0, 64, 64));
			if (SightDirection == 'W') sprite.setTextureRect(IntRect(192, 0, 64, 64));
			if (SightDirection == 'S') sprite.setTextureRect(IntRect(0, 0, 64, 64));
		}

		sprite.setPosition(rect.left, rect.top);
	}

	void Collision(bool flag, float time)
	{
		for (int i = (rect.top) / 32; i < (rect.top + rect.height) / 32; i++)
			for (int j = (rect.left) / 32; j < (rect.left + rect.width) / 32; j++)
			{
				if (LobbyLvl[i][j] == 'B' || LobbyLvl[i][j] == 'H')
				{
					if (xSpeed > 0 && flag == 1)
					{
						rect.left = j * 32 - rect.width;
						xSpeed = 0;
						IsxCollided = true;
					}

					if (xSpeed < 0 && flag == 1)
					{
						rect.left = j * 32 + 32;
						xSpeed = 0;
						IsxCollided = true;
					}

					if (ySpeed > 0 && flag == 0)
					{
						rect.top = i * 32 - rect.height;
						ySpeed = 0;
						IsyCollided = true;
					}

					if (ySpeed < 0 && flag == 0)
					{
						rect.top = i * 32 + 32;
						ySpeed = 0;
						IsyCollided = true;
					}
				}
			}
	}

	void SpellAttack(float time, Texture &image)
	{
		SpellSprite.setTexture(image);
		SpellRect = FloatRect(xPlayerPos, yPlayerPos, 16, 16);
		SpellSprite.setTextureRect(IntRect(48, 1184, 16, 16));
		SpellSprite.setPosition(xPlayerPos,yPlayerPos);
		if (SightDirection == 'N')
		{
			SpellSprite.setTextureRect(IntRect(48, 1184, 16, 16));
			SpellRect.top += -0.1*time;
		}
		if (SightDirection == 'W')
		{
			SpellSprite.setTextureRect(IntRect(32, 1184, 16, 16));
			SpellRect.left += -0.1*time;
		}
		if (SightDirection == 'E')
		{
			SpellSprite.setTextureRect(IntRect(16, 1184, 16, 16));
			SpellRect.left += 0.1*time;
		}
		if (SightDirection == 'S')
		{
			SpellSprite.setTextureRect(IntRect(0, 1184, 16, 16));
			SpellRect.top += 0.1*time;
		}
		SpellSprite.setPosition(SpellRect.left, SpellRect.top);
		isCasted = true;
	}
};

class Enemy
{
public:
	float xSpeed, ySpeed;
	FloatRect EnemyRectangle;
	Sprite EnemySprite;
	bool IsxCollided, IsyCollided;
	char SightDirection;
	float Distance;
	float Health = 20;
	bool isDead = false;

	Enemy(Texture &image, float x, float y) //¬ качестве положени€ необходимо указывать именно квадрат, на котором будет сто€ть противник
	{										//например, 28*32,0*32
		EnemySprite.setTexture(image);
		EnemyRectangle = FloatRect(x * 32, y * 32, 30, 30);
		EnemySprite.setTextureRect(IntRect(1472, 0, 32, 32));

		xSpeed = ySpeed = 0;
		IsxCollided = IsyCollided = false;
		SightDirection = 'S';
	}
	void move()
	{
		Distance = pow(pow(EnemyRectangle.left + 16 - xPlayerPos, 2) + pow(EnemyRectangle.top + 16 - yPlayerPos, 2), 0.5);
		if ((Distance < 300) && (yPlayerPos >= EnemyRectangle.top + 16)) //рассто€ние до середины экрана/геро€ ^
		{
			ySpeed = 0.05;
			//SightDirection = 'S';
		}
		if ((Distance < 300) && (yPlayerPos <= EnemyRectangle.top + 16))
		{
			ySpeed = -0.05;
			//SightDirection = 'N';
		}
		if ((Distance < 300) && (xPlayerPos <= EnemyRectangle.left + 16))
		{
			xSpeed = -0.05;
			//SightDirection = 'W';
		}
		if ((Distance < 300) && (xPlayerPos >= EnemyRectangle.left + 16))
		{
			xSpeed = 0.05;
			//SightDirection = 'E';
		}

	}

	void update(float time)
	{
		IsxCollided = false;
		IsyCollided = false;

		if (!isDead)
		{
			move();

			EnemyRectangle.left += xSpeed * time;
			Collision(1, time);

			EnemyRectangle.top += ySpeed * time;
			Collision(0, time);

			if (EnemyRectangle.top + 16 >= yPlayerPos) //если враг ниже геро€
			{
				if (xPlayerPos - EnemyRectangle.left + 16 <= EnemyRectangle.top + 16 - yPlayerPos)
					SightDirection = 'N';
				else
					SightDirection = 'E';
			}
			if ((int)EnemyRectangle.top + 16 == (int)yPlayerPos) //если на одной горизонтальной пр€мой
			{
				if (xPlayerPos < EnemyRectangle.left + 16)
					SightDirection = 'W';
				else
					SightDirection = 'E';
			}
			else //если выше
			{
				if (EnemyRectangle.left + 16 - xPlayerPos >= yPlayerPos - EnemyRectangle.top + 16)
					SightDirection = 'W';
				else
					SightDirection = 'S';
			}

			if (/*ySpeed > 0*/SightDirection == 'S')
			{
				EnemySprite.setTextureRect(IntRect(1472, 0, 32, 32));
			}
			if (/*ySpeed < 0*/SightDirection == 'N')
			{
				EnemySprite.setTextureRect(IntRect(1568, 0, 32, 32));
			}
			if (/*xSpeed > 0*/SightDirection == 'E')
			{
				EnemySprite.setTextureRect(IntRect(1504, 0, 32, 32));
			}
			if (/*xSpeed < 0*/SightDirection == 'W')
			{
				EnemySprite.setTextureRect(IntRect(1536, 0, 32, 32));
			}
		}

		EnemySprite.setPosition(EnemyRectangle.left, EnemyRectangle.top);

		xSpeed = 0;
		ySpeed = 0;
	}


	void Collision(bool flag, float time) //1 = x; 0 = y
	{
		for (int i = EnemyRectangle.top / 32; i < (EnemyRectangle.top + EnemyRectangle.height) / 32; i++)
			for (int j = EnemyRectangle.left / 32; j < (EnemyRectangle.left + EnemyRectangle.width) / 32; j++)
			{
				if (LobbyLvl[i][j] == 'B' || LobbyLvl[i][j] == 'H')
				{
					if (xSpeed > 0 && flag == 1)
					{
						EnemyRectangle.left = j * 32 - EnemyRectangle.width;
						xSpeed = 0;
						IsxCollided = true;
					}

					if (xSpeed < 0 && flag == 1)
					{
						EnemyRectangle.left = j * 32 + 32;
						xSpeed = 0;
						IsxCollided = true;
					}

					if (ySpeed > 0 && flag == 0)
					{
						EnemyRectangle.top = i * 32 - EnemyRectangle.height;
						ySpeed = 0;
						IsyCollided = true;
					}

					if (ySpeed < 0 && flag == 0)
					{
						EnemyRectangle.top = i * 32 + 32;
						ySpeed = 0;
						IsyCollided = true;
					}
				}
			}
	}
};

int main()
{
	RenderWindow Game(VideoMode(xCentre * 2, yCentre * 2), "ProjectZane v0.3"); //40x23

	Clock clock, timer, staminarestore, attacktimer;
	RectangleShape HealthBar;
	RectangleShape HealthBarBackground;
	RectangleShape MagickaBar;
	RectangleShape MagickaBarBackground;
	RectangleShape StaminaBar;
	RectangleShape StaminaBarBackground;
	Texture ZaneTexture;
	Texture penumbraTexture;
	Texture pointLightTexture;
	Texture ConeLightTexture;
	sf::Sprite Lsprite;//—прайт света.
	Sprite DungeonTest;
	Shader unshadowShader, lightOverShapeShader;
	sf::RenderStates lightRenderStates;
	ltbl::LightSystem ls;

	ZaneTexture.loadFromFile("SpriteZane.png");

	penumbraTexture.loadFromFile("data/penumbraTexture.png");
	penumbraTexture.setSmooth(true);

	pointLightTexture.loadFromFile("data/pointLightTexture.png");
	pointLightTexture.setSmooth(true);

	ConeLightTexture.loadFromFile("data/spotLightTexture.png");
	ConeLightTexture.setSmooth(true);

	unshadowShader.loadFromFile("data/unshadowShader.vert", "data/unshadowShader.frag");
	lightOverShapeShader.loadFromFile("data/lightOverShapeShader.vert", "data/lightOverShapeShader.frag");

	ls.create(ltbl::rectFromBounds(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(10000.0f, 10000.0f)), Game.getSize(), penumbraTexture, unshadowShader, lightOverShapeShader);

	std::shared_ptr<ltbl::LightPointEmission> light = std::make_shared<ltbl::LightPointEmission>();
	light->_emissionSprite.setOrigin(sf::Vector2f(pointLightTexture.getSize().x * 0.5f, pointLightTexture.getSize().y * 0.5f));
	light->_emissionSprite.setTexture(pointLightTexture);
	light->_emissionSprite.setScale(sf::Vector2f(10, 10));
	light->_emissionSprite.setColor(sf::Color(255, 255, 200));
	light->_emissionSprite.setPosition(sf::Vector2f(xCentre, yCentre));
	light->_sourceRadius = 10;
	light->_shadowOverExtendMultiplier = 1;
	ls.addLight(light);

	std::shared_ptr<ltbl::LightShape> lightShape = std::make_shared<ltbl::LightShape>();
	lightShape->_shape.setPointCount(4);
	lightShape->_shape.setPoint(0, Vector2f(0, 0));
	lightShape->_shape.setPoint(1, Vector2f(32, 0));
	lightShape->_shape.setPoint(2, Vector2f(32, 32*4));
	lightShape->_shape.setPoint(3, Vector2f(0, 32*4));
	lightShape->_shape.setPosition(xCentre - 2 * 32, yCentre - 8 - 1 * 32);
	ls.addShape(lightShape);

	std::shared_ptr<ltbl::LightShape> lightShape1 = std::make_shared<ltbl::LightShape>();
	lightShape1->_shape.setPointCount(4);
	lightShape1->_shape.setPoint(0, Vector2f(0, 0));
	lightShape1->_shape.setPoint(1, Vector2f(32*4, 0));
	lightShape1->_shape.setPoint(2, Vector2f(32*4, 32));
	lightShape1->_shape.setPoint(3, Vector2f(0, 32));
	lightShape1->_shape.setPosition(Vector2f(xCentre - 2 * 32, yCentre - 8 + 2 * 32));
	ls.addShape(lightShape1);

	std::shared_ptr<ltbl::LightShape> lightShape2 = std::make_shared<ltbl::LightShape>();
	lightShape2->_shape.setPointCount(4);
	lightShape2->_shape.setPoint(0, Vector2f(0, 0));
	lightShape2->_shape.setPoint(1, Vector2f(32, 0));
	lightShape2->_shape.setPoint(2, Vector2f(32, 32 * 4));
	lightShape2->_shape.setPoint(3, Vector2f(0, 32 * 4));
	lightShape2->_shape.setPosition(xCentre + 1 * 32, yCentre - 8 - 1 * 32);
	ls.addShape(lightShape2);

	Zane Hero(ZaneTexture);
	Enemy Buddy(ZaneTexture, 46.5, 12);

	DungeonTest.setTexture(ZaneTexture);

	sf::View view = Game.getDefaultView();
	
	//================ ѕолоски здоровь€, стамины и магии ====================
	HealthBar.setFillColor(Color(170, 0, 0));
	HealthBar.setPosition(50, 50);
	HealthBar.setSize(Vector2f(200, 15));
	HealthBarBackground.setFillColor(Color(60,0,0));
	HealthBarBackground.setPosition(47, 47);
	HealthBarBackground.setSize(Vector2f(206, 21));

	MagickaBar.setFillColor(Color(0, 0, 200));
	MagickaBar.setPosition(50, 72);
	MagickaBar.setSize(Vector2f(200,15));
	MagickaBarBackground.setFillColor(Color(0, 0, 60));
	MagickaBarBackground.setPosition(47,69);
	MagickaBarBackground.setSize(Vector2f(206,21));

	StaminaBar.setFillColor(Color(0, 120, 0));
	StaminaBar.setPosition(50,94);
	StaminaBar.setSize(Vector2f(200, 15));
	StaminaBarBackground.setFillColor(Color(0,60,0));
	StaminaBarBackground.setPosition(47,91);
	StaminaBarBackground.setSize(Vector2f(206,21));
	//=======================================================================

	while (Game.isOpen()) //ќсновной цикл
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 400;
		if (time > 20)
			time = 20;

		float DamageCoolDown = timer.getElapsedTime().asSeconds();
		float AttackCoolDown = attacktimer.getElapsedTime().asSeconds();
		if (DamageCoolDown > 5)
			DamageCoolDown = 2;

		float StaminaCoolDown = staminarestore.getElapsedTime().asSeconds();

		Event GameEvent;
		while (Game.pollEvent(GameEvent))
		{
			if (GameEvent.type == Event::Closed || Hero.Health < 1) //ѕри закрытии
				Game.close(); //окно закрываетс€...
		}

		//======================= ѕроверка нажати€ клавиш ======================================================
		if (Keyboard::isKeyPressed(Keyboard::Escape))                                                          
			Game.close();

		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			Hero.xSpeed = -0.1;
			Hero.SightDirection = 'W';
		}

		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			Hero.xSpeed = 0.1;
			Hero.SightDirection = 'E';
		}

		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			Hero.ySpeed = -0.1;
			Hero.SightDirection = 'N';
		}

		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			Hero.ySpeed = 0.1;
			Hero.SightDirection = 'S';
		}

		if (Keyboard::isKeyPressed(Keyboard::LShift) && Hero.Stamina > 0 && !(Hero.IsxCollided || Hero.IsyCollided) && (Hero.xSpeed || Hero.ySpeed))
		{
			Hero.xSpeed *= 2;
			Hero.ySpeed *= 2;
			Hero.Stamina -= 0.03*time;
			staminarestore.restart();
		}
		if (StaminaCoolDown > 1 && Hero.Stamina <= 100)
			Hero.Stamina += 0.02*time;

		if (Keyboard::isKeyPressed(Keyboard::Up))
			Hero.SpellAttack(time,ZaneTexture);
		//======================================================================================================

		if (Hero.rect.intersects(Buddy.EnemyRectangle) && DamageCoolDown > 1)
		{
			Hero.Health -= 5;
			timer.restart();
		}
		if (Hero.SpellRect.intersects(Buddy.EnemyRectangle) && AttackCoolDown > 1)
		{
			Buddy.Health -= 10;
			Buddy.EnemyRectangle.left -= 1;
			attacktimer.restart();
		}

		if (Buddy.Health < 1)
		{
			Buddy.EnemySprite.setTextureRect(IntRect(1440, 0, 32, 32));
			Buddy.isDead = true;
		}

		Buddy.update(time);
		Hero.update(time);

		light->_emissionSprite.move(Hero.xSpeed*time, Hero.ySpeed*time);
		view.move(Hero.xSpeed*time,Hero.ySpeed*time);

		xPlayerPos += Hero.xSpeed*time;
		yPlayerPos += Hero.ySpeed*time;

		//================ ќтрисовка полосок состо€ни€ ===============================
		HealthBar.move(Hero.xSpeed*time, Hero.ySpeed*time);
		HealthBar.setScale(Vector2f(Hero.Health/100, 1));
		HealthBarBackground.move(Hero.xSpeed*time, Hero.ySpeed*time);
		StaminaBar.move(Hero.xSpeed*time, Hero.ySpeed*time);
		StaminaBar.setScale(Vector2f(Hero.Stamina / 100, 1));
		StaminaBarBackground.move(Hero.xSpeed*time, Hero.ySpeed*time);
		MagickaBar.move(Hero.xSpeed*time, Hero.ySpeed*time);
		MagickaBarBackground.move(Hero.xSpeed*time, Hero.ySpeed*time);
		//============================================================================
		Hero.xSpeed = Hero.ySpeed = 0;

		printf("%f\n", Buddy.Health);

		Game.clear(Color(10, 0, 0)); //«десь очищаем экран от всего, что было нарисовано ранее...

		for (int i = 0; i < lobbyheight; i++)
			for (int j = 0; j < lobbywidth; j++)
			{
				if (LobbyLvl[i][j] == 'B')
				{
					DungeonTest.setTextureRect(IntRect(1568, 1168, 32, 32));
					DungeonTest.setPosition(j * 32, i * 32);
					
				}

				if (LobbyLvl[i][j] == 'H')
				{
					DungeonTest.setTextureRect(IntRect(1568, 1168 - 32, 32, 32));
					DungeonTest.setPosition(j * 32, i * 32);
				}

				if (LobbyLvl[i][j] == ' ')
				{
					DungeonTest.setTextureRect(IntRect(1568 - 32, 1168, 32, 32));
					DungeonTest.setPosition(j * 32, i * 32);
				}

				Game.draw(DungeonTest);
			}

		Game.draw(Buddy.EnemySprite);
		Game.draw(Hero.sprite);
		Game.draw(Hero.SpellSprite);

		ls.render(view, unshadowShader, lightOverShapeShader);
		Lsprite.setTexture(ls.getLightingTexture());
		lightRenderStates.blendMode = sf::BlendMultiply;
		Game.setView(Game.getDefaultView());
		Game.draw(Lsprite, lightRenderStates);
		Game.setView(view);
		Game.draw(HealthBarBackground);
		Game.draw(HealthBar);
		Game.draw(MagickaBarBackground);
		Game.draw(MagickaBar);
		Game.draw(StaminaBarBackground);
		Game.draw(StaminaBar);

		Game.display();
	}

	return 0;
}