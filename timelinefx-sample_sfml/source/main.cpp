#include "SfmlEffectsLibrary.h"
#include <TLFXEffectsLibrary.h>
#include <TLFXParticleManager.h>
#include <TLFXEffect.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

TLFX::EffectsLibrary *gEffects = NULL;
MarmaladeParticleManager *gPM = NULL;
static const unsigned WIDTH = 800;
static const unsigned HEIGHT = 600;
sf::RenderWindow* g_renderWindow = 0;


void Init()
{
    gEffects = new MarmaladeEffectsLibrary();
    gEffects->Load("particles/data.xml");

    gPM = new MarmaladeParticleManager();
    gPM->SetScreenSize(WIDTH, HEIGHT);
    gPM->SetOrigin(0, 0);

    //TLFX::Effect *eff = gEffects->GetEffect("Sub Effects/Space Creature");
    //TLFX::Effect *eff = gEffects->GetEffect("Spacey/Plasma Flare");
    TLFX::Effect *eff = gEffects->GetEffect("Area Effects/Area Test");
    //TLFX::Effect *eff = gEffects->GetEffect("Sub Effects/DirectionTest");
    //TLFX::Effect *eff = gEffects->GetEffect("Pyro/Complex Explosion 2");

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
    // create the window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TimelineFX SFML");
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(30);
    g_renderWindow = &window;

    // load effect
    Init();

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
        }

        // clear the buffers
        glClearColor(0, 0, 0.10, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw...
        Update();
        Render();     

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...
    ShutDown();

    return 0;
}
