#ifndef EMITTER_H_
#define EMITTER_H_

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

namespace Feanwork
{
	struct Particle
	{
		Particle()
		{
			age		 = 1.0f;
			lifeTime = 0.0f;
		}

		sf::Sprite		sprite;
		sf::Vector2f	direction; //unit vector
		float			speed;
		float			age;
		float			lifeTime;

		sf::Color		colourStart;
		sf::Color		colourEnd;

		float			sizeStart;
		float			sizeEnd;
		float			speedStart;
		float			speedEnd;
	};

	enum EMITTERTYPE
	{
		EMITTERTYPE_Circle = 0,
		EMITTERTYPE_Directional,
	};

	class Game;
	class Emitter
	{
	public:
		Emitter(sf::Vector2f _position, sf::Vector2f _direction, EMITTERTYPE _type, bool _loop);
		~Emitter();

	void update(Game* _game);
	void render(Game* _game);
	void clean();
	void emit(Particle& _particle, unsigned _index);
	bool loadTexture(int _resourceID);

	void		 setPosition(float _x, float _y) { mPosition.x = _x; mPosition.y = _y; }
	sf::Vector2f getPosition()					 { return mPosition;				   }
	bool		 isActive()						 { return mActive;					   }

	void		 setNumParticles(int _num);
	void		 calculateBirthRate();

	void		 setEmitter(EMITTERTYPE _type)   { mEmitter = _type; }
	bool		 parseParticleFile(std::string _filePath);

	protected:
		// Emitter Specific Variables
		Particle*		mParticles;
		sf::Vector2f	mPosition;
		sf::Vector2f	mForce;
		sf::Vector2f	mDirection;
		sf::Texture		mTexture;
		float			mBirthRate;
		float			mPulseRate; //Hz = 1/s
		float			mPulseLength; //ms
		unsigned		mNumParticles;
		EMITTERTYPE		mEmitter;

		float			mAccum;
		bool			mLoop;
		bool			mActive;

		// Particle Specific Variables
		sf::Color		mColourStart;
		sf::Color		mColourEnd;
		sf::Color		mColourStartVariant;
		sf::Color		mColourEndVariant;

		float			mSizeStart;
		float			mSizeEnd;
		float			mSizeStartVariant;
		float			mSizeEndVariant;

		float			mSpeedStart;
		float			mSpeedEnd;
		float			mSpeedStartVariant;
		float			mSpeedEndVariant;

		float			mLifeTime;
		float			mLifeTimeVariant;

		sf::Vector2f	mPositionVariant;
	};
}

#endif