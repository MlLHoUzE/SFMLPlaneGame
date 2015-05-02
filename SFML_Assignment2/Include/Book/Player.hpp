#ifndef BOOK_PLAYER_HPP
#define BOOK_PLAYER_HPP

#include <Book/Command.hpp>

#include <SFML/Window/Event.hpp>

#include <map>


class CommandQueue;

class Player
{
	public:
		enum Action
		{
			MoveLeft,
			MoveRight,
			MoveUp,
			MoveDown,
			Fire,
			LaunchMissile,
			LaunchMissile1,
			ActionCount
		};
		enum Buttons
		{
			Left,
			Right,
			Middle,
			XButton1,
			XButton2,

			ButtonCount
		};

		enum MissionStatus
		{
			MissionRunning,
			MissionSuccess,
			MissionFailure
		};


	public:
								Player();
//								sf::Sprite				mSprite;
		void					handleEvent(const sf::Event& event, CommandQueue& commands);
		void					handleRealtimeInput(CommandQueue& commands);

		void					assignKey(Action action, sf::Keyboard::Key key);
		sf::Keyboard::Key		getAssignedKey(Action action) const;
		void					assignKey1(Buttons action, sf::Mouse::Button key1);
		sf::Mouse::Button getAssignedKey1(Buttons action) const;
		//sf::Mouse::
		/*float getRotation(sf::RenderWindow &mWindow);
		void LookMouse(sf::RenderWindow &mWindow);
		void Shoot(float deltaT, sf::RenderWindow &mWindow);
		sf::FloatRect getBoundingRect() const;*/
		void mousemove();

		void 					setMissionStatus(MissionStatus status);
		MissionStatus 			getMissionStatus() const;

	private:
		void					initializeActions();
		static bool				isRealtimeAction(Action action);
		bool isRealtimeAction(Buttons action);
		


	private:
		std::map<sf::Keyboard::Key, Action>		mKeyBinding;
		std::map<sf::Mouse::Button, Buttons>		mMouse;
		std::map<Action, Command>				mActionBinding;
		std::map<Buttons, Command>				mActionBinding1;

		MissionStatus 							mCurrentMissionStatus;
};

#endif // BOOK_PLAYER_HPP
