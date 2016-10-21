#ifdef _MSC_VER
#pragma once
#endif

/*
 * SFML for rendering
 * PugiXML for parsing data
 */

#ifndef _SFMLEFFECTSLIBRARY_H
#define _SFMLEFFECTSLIBRARY_H

#include "TLFXEffectsLibrary.h"
#include "TLFXParticleManager.h"
#include "TLFXAnimImage.h"
#include <SFML/Graphics.hpp>

class XMLLoader;

class SfmlEffectsLibrary : public TLFX::EffectsLibrary
{
public:
    virtual TLFX::XMLLoader* CreateLoader() const;
    virtual TLFX::AnimImage* CreateImage() const;
};

class SfmlParticleManager : public TLFX::ParticleManager
{
public:
    SfmlParticleManager(int particles = TLFX::ParticleManager::particleLimit, int layers = 1);
    void Flush();
protected:
    virtual void DrawSprite(TLFX::AnimImage* sprite, float px, float py, float frame, float x, float y, float rotation,
                            float scaleX, float scaleY, unsigned char r, unsigned char g, unsigned char b, float a, bool additive);

    // batching
    struct Batch
    {
        float px, py;
        float frame;
        float x, y;
        float rotation;
        float scaleX, scaleY;
        sf::Color color;
    };
    std::list<Batch> _batch;
    TLFX::AnimImage *_lastSprite;
    bool             _lastAdditive;
};

class SfmlImage : public TLFX::AnimImage
{
public:
    SfmlImage();
    ~SfmlImage();

    bool Load(const char *filename);
    sf::Texture* GetTexture() const;


protected:
    sf::Texture *_texture;
};

#endif // _SFMLEFFECTSLIBRARY_H
