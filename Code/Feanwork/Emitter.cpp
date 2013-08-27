#include <cassert>
#include "Emitter.h"
#include "Game.h"
#include "Random.h"
#include "Parser.h"

namespace Feanwork
{
	Emitter::Emitter(sf::Vector2f _position, sf::Vector2f _direction, EMITTERTYPE _type, bool _loop)
	{
		mPosition	  = _position;
		mDirection	  = _direction;
		mEmitter	  = _type;
		mNumParticles = 0;
		mParticles	  = NULL;
		mForce		  = sf::Vector2f(0, 0);
		mAccum		  = 0.0f;
		mLoop		  = _loop;
		mActive		  = true;
	}

	Emitter::~Emitter()
	{
	}

	void Emitter::update(Game* _game)
 	{
 		if(!mActive)
 			return;
 
 		assert(mParticles);
 		Particle* curP;
 
		mAccum += _game->getDelta() * 1000.f; //temp
 		int newParticles = (int)(mBirthRate * mAccum);
 
 		if(newParticles > 0)
 			mAccum = 0.0f;
 
 		bool resume = false;
 
 		for(unsigned i = 0; i < mNumParticles; i++)
 		{
 			curP = &mParticles[i];
			curP->age += _game->getDelta() * 1000.0f;
 
 			if(curP->age > curP->lifeTime)
 			{
 				if(newParticles > 0)
 				{
 					if(!mLoop && (curP->lifeTime > 0.0f))
 						continue;
 
 					emit(*curP, i);
 					newParticles--;
 				}
 
 				resume = true;
 				//continue; //temp
 			}
 
			curP->direction += mForce * _game->getDelta();
			curP->sprite.move(curP->direction * curP->speed * _game->getDelta());
 
 			float ratio		= curP->age / curP->lifeTime;
 			float inv_ratio = (1.0f - ratio);
 
 			if(ratio > 1)
 				ratio = 1;
 
 			float scale		= (inv_ratio * curP->sizeStart + ratio * curP->sizeEnd);
 			curP->sprite.setScale(scale, scale);
 
 			curP->speed	= (inv_ratio * curP->speedStart + ratio * curP->speedEnd);
 
 			sf::Color colour;
 			colour.r		= (sf::Uint8)(inv_ratio * curP->colourStart.r + ratio * curP->colourEnd.r);
 			colour.g		= (sf::Uint8)(inv_ratio * curP->colourStart.g + ratio * curP->colourEnd.g);
 			colour.b		= (sf::Uint8)(inv_ratio * curP->colourStart.b + ratio * curP->colourEnd.b);
 			colour.a		= (sf::Uint8)(inv_ratio * curP->colourStart.a + ratio * curP->colourEnd.a);
 			curP->sprite.setColor(colour);
 
 			resume = true;
 		}
 
 		if(!mLoop && !resume)
 			mActive = false;
 	}


	void Emitter::render(Game* _game)
	{
		if(!mActive)
			return;

		assert(mParticles);

		sf::RenderStates state;
		state.blendMode = sf::BlendAdd;

		for(unsigned i = 0; i < mNumParticles; i++)
			if(mParticles[i].age < mParticles[i].lifeTime)
				_game->getWindow()->draw(mParticles[i].sprite, state);
	}

	void Emitter::clean()
	{
		/*if(mParticles)
			delete[] mParticles;*/
	}

	void Emitter::emit(Particle& _particle, unsigned _index)
	{
		Random* rand = Random::get_singleton();

		if(mEmitter == EMITTERTYPE_Circle)
			_particle.direction		= rand->rand_unit_vec();
		else if(mEmitter == EMITTERTYPE_Directional)
			_particle.direction		= mDirection;

		_particle.sprite.setPosition((sf::Vector2f(rand->rand_range(-1, 1) *  mPositionVariant.x, 
									  rand->rand_range(-1, 1) * mPositionVariant.y)) + mPosition);
		_particle.speedStart		= (rand->rand_range(-1, 1) * mSpeedStartVariant	) + mSpeedStart;
		_particle.speedEnd			= (rand->rand_range(-1, 1) * mSpeedEndVariant	) + mSpeedEnd;
		_particle.age				=  0.0f;
		_particle.lifeTime			= (rand->rand_range(-1, 1) * mLifeTimeVariant	) + mLifeTime;
		_particle.sizeStart			= (rand->rand_range(-1, 1) * mSizeStartVariant	) + mSizeStart;
		_particle.sizeEnd			= (rand->rand_range(-1, 1) * mSizeEndVariant	) + mSizeEnd;

		sf::Color colour;
		float r						= rand->rand_range(-1, 1);
		colour.r					= (sf::Uint8)(r * mColourStartVariant.r) + mColourStart.r;
		colour.g					= (sf::Uint8)(r * mColourStartVariant.g) + mColourStart.g;
		colour.b					= (sf::Uint8)(r * mColourStartVariant.b) + mColourStart.b;
		colour.a					= (sf::Uint8)(r * mColourStartVariant.a) + mColourStart.a;
		_particle.colourStart		=  colour;

		r							= rand->rand_range(-1, 1);
		colour.r					= (sf::Uint8)(r * mColourEndVariant.r)	 + mColourEnd.r;
		colour.g					= (sf::Uint8)(r * mColourEndVariant.g)	 + mColourEnd.g;
		colour.b					= (sf::Uint8)(r * mColourEndVariant.b)	 + mColourEnd.b;
		colour.a					= (sf::Uint8)(r * mColourEndVariant.a)	 + mColourEnd.a;
		_particle.colourEnd			=  colour;

		_particle.speed				= _particle.speedStart;
		_particle.sprite.setScale(_particle.sizeStart, _particle.sizeStart);
	}

	bool Emitter::loadTexture(int _resourceID)
	{
		mTexture = *ResourceManager::getSingleton()->getResource(_resourceID);
		return false;
	}

	void Emitter::setNumParticles(int num)
	{
		if(mParticles)
			delete[] mParticles;

		mNumParticles		= num;
		mParticles			= new Particle[num];

		for(int i = 0; i < num; i++)
		{
			mParticles[i].sprite.setTexture(mTexture);
			mParticles[i].sprite.setOrigin((float)mTexture.getSize().x / 2, (float)mTexture.getSize().y / 2);
		}
	}

	void Emitter::calculateBirthRate()
	{
		static float offsFactor = 0.01f;

		if(mPulseRate > 0.0f)
		{
			mBirthRate	= (mNumParticles / mPulseLength);
			mBirthRate += (mBirthRate * offsFactor);
			mLifeTime	= (1.0f / mPulseRate) * 1000.0f;
		}
		else
		{
			mBirthRate	= sqrt(mNumParticles + (mLifeTime * mLifeTime) / 4.0f) - (mLifeTime / 2.0f);
			mBirthRate -= (mBirthRate * offsFactor);
		}
	}

	bool Emitter::parseParticleFile(std::string _filePath)
	{
		Parser p(_filePath);
		std::string keyword = "";

		while((keyword = p.getNextKeyword()) != "")
		{
			if(keyword == "ResourceID")
				loadTexture(p.getInt(0));
			else if(keyword == "NumParticles")
				setNumParticles(p.getInt(0));
			else if(keyword == "Pulse")
			{
				mPulseRate   = p.getFloat(0);
				mPulseLength = p.getFloat(1);
			}

			else if(keyword == "ColourStart")
			{
				mColourStart.r = (sf::Uint8)p.getFloat(0);
				mColourStart.g = (sf::Uint8)p.getFloat(1);
				mColourStart.b = (sf::Uint8)p.getFloat(2);
				mColourStart.a = (sf::Uint8)p.getFloat(3);
			}
			else if(keyword == "ColourEnd")
			{
				mColourEnd.r = (sf::Uint8)p.getFloat(0);
				mColourEnd.g = (sf::Uint8)p.getFloat(1);
				mColourEnd.b = (sf::Uint8)p.getFloat(2);
				mColourEnd.a = (sf::Uint8)p.getFloat(3);
			}
			else if(keyword == "ColourStartVariant")
			{
				mColourStartVariant.r = (sf::Uint8)p.getFloat(0);
				mColourStartVariant.g = (sf::Uint8)p.getFloat(1);
				mColourStartVariant.b = (sf::Uint8)p.getFloat(2);
				mColourStartVariant.a = (sf::Uint8)p.getFloat(3);
			}
			else if(keyword == "ColourEndVariant")
			{
				mColourEndVariant.r = (sf::Uint8)p.getFloat(0);
				mColourEndVariant.g = (sf::Uint8)p.getFloat(1);
				mColourEndVariant.b = (sf::Uint8)p.getFloat(2);
				mColourEndVariant.a = (sf::Uint8)p.getFloat(3);
			}

			else if(keyword == "SizeStart")
				mSizeStart = p.getFloat(0);
			else if(keyword == "SizeEnd")
				mSizeEnd = p.getFloat(0);
			else if(keyword == "SizeStartVariant")
				mSizeStartVariant = p.getFloat(0);
			else if(keyword == "SizeEndVariant")
				mSizeEndVariant = p.getFloat(0);

			else if(keyword == "SpeedStart")
				mSpeedStart = p.getFloat(0);
			else if(keyword == "SpeedEnd")
				mSpeedEnd = p.getFloat(0);
			else if(keyword == "SpeedStartVariant")
				mSpeedStartVariant = p.getFloat(0);
			else if(keyword == "SpeedEndVariant")
				mSpeedEndVariant = p.getFloat(0);

			else if(keyword == "LifeTime")
				mLifeTime = p.getFloat(0);
			else if(keyword == "LifeTimeVariant")
				mLifeTimeVariant = p.getFloat(0);

			else if(keyword == "PositionVariant")
			{
				mPositionVariant.x = p.getFloat(0);
				mPositionVariant.y = p.getFloat(1);
			}
		}

		calculateBirthRate();
		return true;
	}
}