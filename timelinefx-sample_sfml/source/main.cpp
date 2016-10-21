#include <SFML/Graphics.hpp>
#include "SfmlEffectsLibrary.h"
#include <TLFXEffectsLibrary.h>
#include <TLFXParticleManager.h>
#include <TLFXEffect.h>

TLFX::EffectsLibrary *gEffects = NULL;
MarmaladeParticleManager *gPM = NULL;

static const unsigned WIDTH = 200;
static const unsigned HEIGHT = 200;


void Init()
{
    gEffects = new MarmaladeEffectsLibrary();
    gEffects->Load("particles/data.xml");

    gPM = new MarmaladeParticleManager();
    gPM->SetScreenSize(WIDTH, HEIGHT);
    gPM->SetOrigin(0, 0);

    TLFX::Effect *eff = gEffects->GetEffect("Area Effects/Swirly Balls");
    TLFX::Effect *copy = new TLFX::Effect(*eff, gPM);

    copy->SetPosition(0.0f, 0.0f);
    gPM->AddEffect(copy);
}

void ShutDown()
{
    delete gEffects;
    delete gPM;
}

void Update()
{
    gPM->Update();
}

void Render()
{
    gPM->DrawParticles();
    gPM->Flush();
}

int main()
{
    Init();

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //Update();
        //Render();

        window.clear();
        window.draw(shape);
        window.display();
    }

    ShutDown();

    return 0;
}
