#include "SfmlEffectsLibrary.h"
#include <TLFXEffectsLibrary.h>
#include <TLFXParticleManager.h>
#include <TLFXEffect.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <sstream>

TLFX::EffectsLibrary *gEffects = NULL;
SfmlParticleManager *gPM = NULL;
TLFX::Effect* gCurrentEffect = NULL;
sf::RenderWindow* g_renderWindow = NULL;

static const unsigned WIDTH = 1024;             // window width
static const unsigned HEIGHT = 768;             // window height
const char* g_effectDirectory = "FireandSmoke";    // directory name where is located the effect data.xml

#include "miniz.c"
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;

const char* effectFileName = "LibraryExamples.eff";

int Unzip()
{
    int i, sort_iter;
    mz_bool status;
    size_t uncomp_size;
    mz_zip_archive zip_archive;
    void *p;
    //const int N = 50;
    //char data[2048];
    //char archive_filename[64];
    //static const char *s_Test_archive_filename = "__mz_example2_test__.zip";



    memset(&zip_archive, 0, sizeof(zip_archive));
    status = mz_zip_reader_init_file(&zip_archive, effectFileName, 0);
    assert(status);

    for (i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
    {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
        {
            printf("mz_zip_reader_file_stat() failed!\n");
            mz_zip_reader_end(&zip_archive);
            return EXIT_FAILURE;
        }

        printf("Filename: \"%s\", Comment: \"%s\", Uncompressed size: %u, Compressed size: %u, Is Dir: %u\n",
               file_stat.m_filename,
               file_stat.m_comment,
               (uint)file_stat.m_uncomp_size,
               (uint)file_stat.m_comp_size,
               mz_zip_reader_is_file_a_directory(&zip_archive, i));

        if (!strcmp(file_stat.m_filename, "directory/"))
        {
            if (!mz_zip_reader_is_file_a_directory(&zip_archive, i))
            {
                printf("mz_zip_reader_is_file_a_directory() didn't return the expected results!\n");
                mz_zip_reader_end(&zip_archive);
                return EXIT_FAILURE;
            }
        }




        // Try to extract all the files to the heap.
        p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncomp_size, 0);
        if (!p)
        {
            printf("mz_zip_reader_extract_file_to_heap() failed!\n");
            mz_zip_reader_end(&zip_archive);
            return EXIT_FAILURE;
        }

        // Make sure the extraction really succeeded.
        //if ((uncomp_size != (strlen(data) + 1)) || (memcmp(p, data, strlen(data))))
        if (uncomp_size != file_stat.m_uncomp_size)
        {
            printf("mz_zip_reader_extract_file_to_heap() failed to extract the proper data\n");
            mz_free(p);
            mz_zip_reader_end(&zip_archive);
            return EXIT_FAILURE;
        }

        printf("Successfully extracted file \"%s\", size %u\n", file_stat.m_filename, (uint)uncomp_size);
        //printf("File data: \"%s\"\n", (const char *)p);


    }

    // Close the archive, freeing any resources it was using
    mz_zip_reader_end(&zip_archive);



    // Now verify the compressed data
    /*for (sort_iter = 0; sort_iter < 2; sort_iter++)
    {
        memset(&zip_archive, 0, sizeof(zip_archive));
        status = mz_zip_reader_init_file(&zip_archive, effectFileName, sort_iter ? MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY : 0);
        if (!status)
        {
            printf("mz_zip_reader_init_file() failed!\n");
            return EXIT_FAILURE;
        }

        for (i = 0; i < N; i++)
        {
            sprintf(archive_filename, "%u.txt", i);
            //sprintf(data, "%u %s %u", (N - 1) - i, s_pTest_str, i);

            // Try to extract all the files to the heap.
            p = mz_zip_reader_extract_file_to_heap(&zip_archive, archive_filename, &uncomp_size, 0);
            if (!p)
            {
                printf("mz_zip_reader_extract_file_to_heap() failed!\n");
                mz_zip_reader_end(&zip_archive);
                return EXIT_FAILURE;
            }

            // Make sure the extraction really succeeded.
            if ((uncomp_size != (strlen(data) + 1)) || (memcmp(p, data, strlen(data))))
            {
                printf("mz_zip_reader_extract_file_to_heap() failed to extract the proper data\n");
                mz_free(p);
                mz_zip_reader_end(&zip_archive);
                return EXIT_FAILURE;
            }

            printf("Successfully extracted file \"%s\", size %u\n", archive_filename, (uint)uncomp_size);
            printf("File data: \"%s\"\n", (const char *)p);

            // We're done.
            mz_free(p);
        }

        // Close the archive, freeing any resources it was using
        mz_zip_reader_end(&zip_archive);
    }*/

    printf("Success.\n");



    return EXIT_SUCCESS;
}

void Init()
{
    std::string filename(g_effectDirectory);
    filename.append("/data.xml");

    gEffects = new SfmlEffectsLibrary();
    gEffects->Load(filename.c_str());

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
    //TLFX::Effect *eff = gEffects->GetEffect("Readouts/Monitor Readout Scanlines");
    //TLFX::Effect *eff = gEffects->GetEffect("Pyro/Long Smoke trail");
    TLFX::Effect *eff = gEffects->GetEffect("Tests/Muzzle2");
    //TLFX::Effect *eff = gEffects->GetEffect("Elements/Hundredths");
    //TLFX::Effect *eff = gEffects->GetEffect("Readouts/Monitor Readout Waves 3");
    //TLFX::Effect *eff = gEffects->GetEffect("Elements/LetterFalls");
    //TLFX::Effect *eff = gEffects->GetEffect("Sprays/Lava Spew");

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
    assert(!Unzip());
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
