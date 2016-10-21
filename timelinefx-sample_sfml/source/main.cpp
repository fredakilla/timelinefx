#include "SfmlEffectsLibrary.h"
#include <TLFXEffectsLibrary.h>
#include <TLFXParticleManager.h>
#include <TLFXEffect.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include  <sstream>

TLFX::EffectsLibrary *gEffects = NULL;
SfmlParticleManager *gPM = NULL;
TLFX::Effect* gCurrentEffect = NULL;

static const unsigned WIDTH = 1024;
static const unsigned HEIGHT = 768;
sf::RenderWindow* g_renderWindow = 0;


void Init()
{
    gEffects = new SfmlEffectsLibrary();
    gEffects->Load("particles/data.xml");

    gPM = new SfmlParticleManager(10000, 1);
    gPM->SetScreenSize(WIDTH, HEIGHT);
    gPM->SetOrigin(0, 0);

    //TLFX::Effect *eff = gEffects->GetEffect("Sub Effects/Space Creature");
    //TLFX::Effect *eff = gEffects->GetEffect("Spacey/Plasma Flare");
    //TLFX::Effect *eff = gEffects->GetEffect("Area Effects/Area Test");
    //TLFX::Effect *eff = gEffects->GetEffect("Area Effects/Gas Hob");
    //TLFX::Effect *eff = gEffects->GetEffect("Sub Effects/DirectionTest");
    //TLFX::Effect *eff = gEffects->GetEffect("Area Effects/Laser Beam Test");
    //TLFX::Effect *eff = gEffects->GetEffect("Pyro/Complex Explosion 2");
    //TLFX::Effect *eff = gEffects->GetEffect("Spacey/Cosmic Alien Spiral");
    //TLFX::Effect *eff = gEffects->GetEffect("Spacey/Birth of a Red Giant");
    TLFX::Effect *eff = gEffects->GetEffect("Sprays/Lava Spew");
    //TLFX::Effect *eff = gEffects->GetEffect("Pyro/Long Smoke trail");

    gCurrentEffect = new TLFX::Effect(*eff, gPM);

    gCurrentEffect->SetPosition(0.0f, 0.0f);
    gPM->AddEffect(gCurrentEffect);
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
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TimelineFX SFML");
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);
    g_renderWindow = &window;

    // load effect
    Init();

    // load font for text stats
    sf::Font font;
    font.loadFromFile("arial.ttf");

    // create clock for fps
    sf::Clock clock;

    // center mouse in windows
    sf::Vector2i center = sf::Vector2i(WIDTH/2, HEIGHT/2);
    sf::Mouse::setPosition(center, window);
    bool capturedMouse = false;

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);

                //glViewport(0,0,width,height) ; // viewport definition
                glMatrixMode(GL_PROJECTION) ;
                glLoadIdentity() ;
                glOrtho(0, event.size.width, event.size.height, 0, -1, 1) ;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    capturedMouse = true;
            }
            else if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                    capturedMouse = false;
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                if (capturedMouse)
                {
                    float mouseX = sf::Mouse::getPosition(window).x - (WIDTH/2.0f);
                    float mouseY = sf::Mouse::getPosition(window).y - (HEIGHT/2.0f);
                    gCurrentEffect->SetPosition(mouseX, mouseY);
                }
            }
        }

        // clear the buffers
        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw particles
        Update();
        Render();

        // print stats
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(14);
        text.setColor(sf::Color::White);
        text.setPosition(5,5);
        text.setStyle(sf::Text::Regular);
        std::ostringstream ss;
        ss << "FPS : " << 1.0f/clock.getElapsedTime().asSeconds() << std::endl;
        ss << "Effect Name : " << gPM->GetEffectNames() << std::endl;
        ss << "Effect Count : " << gPM->GetEffectCount() << std::endl;
        ss << "Particles In Use : " << gPM->GetParticlesInUse() << ", Unused : "<< gPM->GetParticlesUnused() << std::endl;
        text.setString(ss.str());
        window.draw(text);

        // end the current frame (internally swaps the front and back buffers)
        window.display();

        // restart clock for fps
        clock.restart().asSeconds();
    }

    // release resources...
    ShutDown();

    return 0;
}
