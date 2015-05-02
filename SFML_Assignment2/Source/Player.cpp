#include <Book/Player.hpp>
#include <Book/CommandQueue.hpp>
#include <Book/Aircraft.hpp>
#include <Book/Foreach.hpp>
//#include <Mouse.hpp>
#include <map>
#include <string>
#include <algorithm>
#include <Book/Application.hpp>

using namespace std::placeholders;


struct AircraftMover
{
	AircraftMover(float vx, float vy)
	: velocity(vx, vy)
	{
	}

	void operator() (Aircraft& aircraft, sf::Time) const
	{
		aircraft.accelerate(velocity * aircraft.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

Player::Player()
: mCurrentMissionStatus(MissionRunning)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::A] = MoveLeft;
	mKeyBinding[sf::Keyboard::D] = MoveRight;
	mKeyBinding[sf::Keyboard::W] = MoveUp;
	mKeyBinding[sf::Keyboard::S] = MoveDown;
	mKeyBinding[sf::Keyboard::Space] = Fire;
	mKeyBinding[sf::Keyboard::M] = LaunchMissile;
	mKeyBinding[sf::Keyboard::N] = LaunchMissile1;
	mMouse[sf::Mouse::Left] = Left;
	mMouse[sf::Mouse::Right] = Right;
 
	// Set initial action bindings
	initializeActions();	
	mousemove();

	// Assign all categories to player's aircraft
	FOREACH(auto& pair, mActionBinding)
		pair.second.category = Category::PlayerAircraft;
	// Assign all categories to player's aircraft
	FOREACH(auto& pair, mActionBinding1)
		pair.second.category = Category::PlayerAircraft;
	FOREACH(auto& pair1, mActionBinding1)
		pair1.second .category = Category::PlayerAircraft;

	
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);
		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding[found->second]);
	}
	if (event.type == sf::Event::MouseMoved)
	{
		FOREACH(auto pair, mMouse)
		/*	auto found = mMouse.find(event.mouseMove.y);
			if (found != mMouse.end() && !isRealtimeAction(found->second))
			commands.push(mActionBinding1[found->second]);*/
		commands.push(mActionBinding1[pair.second]);

	}
}

void Player::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	FOREACH(auto pair, mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding[pair.second]);
	}
	FOREACH(auto pair, mMouse)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Mouse::isButtonPressed(pair.first) && isRealtimeAction(pair.second))
			commands.push(mActionBinding1[pair.second]);

	}

}

void Player::assignKey(Action action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Action action) const
{
	FOREACH(auto pair, mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}
void Player::assignKey1(Buttons action, sf::Mouse::Button key1)
{
	// Remove all keys that already map to action
	for (auto itr = mMouse.begin(); itr != mMouse.end();)
	{
		if (itr->second == action)
			mMouse.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mMouse[key1] = action;
}



sf::Mouse::Button Player::getAssignedKey1(Buttons action) const
{
	FOREACH(auto pair, mMouse)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Mouse::ButtonCount;
}
void Player::setMissionStatus(MissionStatus status)
{
	mCurrentMissionStatus = status;
}

Player::MissionStatus Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

void Player::initializeActions()
{
	
	mActionBinding[MoveLeft].action      = derivedAction<Aircraft>(AircraftMover(-1,  0));
	mActionBinding[MoveRight].action     = derivedAction<Aircraft>(AircraftMover(+1,  0));
	mActionBinding[MoveUp].action        = derivedAction<Aircraft>(AircraftMover( 0, -1));
	mActionBinding[MoveDown].action      = derivedAction<Aircraft>(AircraftMover( 0, +1));
	mActionBinding[Fire].action          = derivedAction<Aircraft>([] (Aircraft& a, sf::Time) { a.fire(); });
	mActionBinding[LaunchMissile].action = derivedAction<Aircraft>([] (Aircraft& a, sf::Time) { a.launchMissile(); });
	mActionBinding[LaunchMissile1].action = derivedAction<Aircraft>([](Aircraft& a, sf::Time) { a.launchMissile1(); });
	mActionBinding1[Left].action = derivedAction<Aircraft>(AircraftMover(-1,  0));
	mActionBinding1[Right].action = derivedAction<Aircraft>(AircraftMover(+1, 0));

}
//float Player::getRotation(sf::RenderWindow &mWindow)
//{
//	sf::Vector2f curPos;  //current position(not cursor position, sorry for ambiguous var names!)
//	curPos.x = mSprite.getGlobalBounds().width;
//	curPos.y = mSprite.getGlobalBounds().top;
//	sf::Vector2i position = sf::Mouse::getPosition(mWindow);
//
//	const float PI = 3.14159;
//
//	float dx = position.x - curPos.x;
//	float dy = position.y - curPos.y;
//
//	float rotation = (atan2(dy, dx)) * 180 / PI;
//
//	return rotation;
//}
//void Player::LookMouse(sf::RenderWindow &mWindow)
//{
//	float rotation = getRotation(mWindow);
//	mSprite.setRotation(rotation);
//}
//void Player::Shoot(float deltaT, sf::RenderWindow &mWindow)
//{
//	mSprite.move(cos(getRotation(mWindow)*3.14159265 / 180) * 4, sin(getRotation(mWindow)*3.14159265 / 180)*-4);
//}

bool Player::isRealtimeAction(Action action)
{
	switch (action)
	{
		case MoveLeft:
		case MoveRight:
		case MoveDown:
		case MoveUp:
		case Fire:
		//case Left:
			return true;

		default:
			return false;
	}
}
bool Player::isRealtimeAction(Buttons action)
{
	switch (action)
	{
	
		case Left:
		case Right:
		return true;

	default:
		return false;
	}
}
void Player::mousemove()
{
	sf::Window window;
	sf::Sprite mSprite;
	//float airdis = mSprite.getLocalBounds().left;
	//float xDistance = sf::Mouse::getPosition(window).x - mSprite.getLocalBounds().width;

	//float yDistance = sf::Mouse::getPosition(window).y - mSprite.getLocalBounds().height;
	//float easingAmount = 0.15;

	//float distance = sqrt((xDistance * xDistance) + (yDistance * yDistance));

	//if (distance >= 1)

	//{

	//	mSprite.getLocalBounds().width == xDistance * easingAmount;
	//	mSprite.getLocalBounds().height == yDistance * easingAmount;

	//	//joueur.setY(joueur.getY() + yDistance * easingAmount);

	//}
	sf::Vector2f curPos;  //current position(not cursor position, sorry for ambiguous var names!)
	curPos.x = mSprite.getGlobalBounds().left;
	curPos.y = mSprite.getGlobalBounds().top;
	sf::Vector2i position = sf::Mouse::getPosition(window);

	const float PI = 3.14159;

	float dx = position.x - curPos.x;
	float dy = position.y - curPos.y;

	float rotation = (atan2(dy, dx)) * 180 / PI;
	//float rotation = getRotation(win);
	mSprite.setPosition(dx,dy);

	//return rotation;

}