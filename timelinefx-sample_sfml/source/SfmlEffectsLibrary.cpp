#include "SfmlEffectsLibrary.h"
#include "TLFXPugiXMLLoader.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <cmath>
#include <assert.h>

extern sf::RenderWindow* g_renderWindow;


TLFX::XMLLoader* SfmlEffectsLibrary::CreateLoader() const
{
    return new TLFX::PugiXMLLoader(0);
}

TLFX::AnimImage* SfmlEffectsLibrary::CreateImage() const
{
    return new SfmlImage();
}

bool SfmlImage::Load( const char *filename )
{
    _texture = new sf::Texture();

    // replace any existing path in filename by the effect directory
    std::string path = filename;
    std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
    //std::string url = std::string(g_effectDirectory) + "/" + base_filename;
    std::string url = std::string("unzipped/" + base_filename);
    assert(_texture->loadFromFile(url.c_str()));

    _texture->setSmooth(true);

    return true;
}

SfmlImage::SfmlImage()
    : _texture(NULL)
{

}

SfmlImage::~SfmlImage()
{
    if (_texture)
        delete _texture;
}

sf::Texture *SfmlImage::GetTexture() const
{
    return _texture;
}

SfmlParticleManager::SfmlParticleManager( int particles /*= particleLimit*/, int layers /*= 1*/ )
    : TLFX::ParticleManager(particles, layers)
    , _lastSprite(NULL)
    , _lastAdditive(true)
{

}

void SfmlParticleManager::DrawSprite( TLFX::AnimImage* sprite, float px, float py, float frame, float x, float y, float rotation, float scaleX, float scaleY, unsigned char r, unsigned char g, unsigned char b, float a , bool additive )
{
    unsigned char alpha = (unsigned char)(a * 255);
    if (alpha == 0 || scaleX == 0 || scaleY == 0) return;

    if (sprite != _lastSprite || additive != _lastAdditive)
        Flush();

    Batch batch;
    batch.px = px;
    batch.py = py;
    batch.frame = frame;
    batch.x = x;
    batch.y = y;
    batch.rotation = rotation;
    batch.scaleX = scaleX;
    batch.scaleY = scaleY;
    batch.color = sf::Color(r, g, b, alpha);
    _batch.push_back(batch);

    _lastSprite = sprite;
    _lastAdditive = additive;
}

void SfmlParticleManager::Flush()
{
    if (g_renderWindow && !_batch.empty() && _lastSprite)
    {
        // get SFML texture of the _lastSprite
        sf::Texture* texture = static_cast<SfmlImage*>(_lastSprite)->GetTexture();

        // compute vertices size
        unsigned int count = (unsigned int)_batch.size();
        unsigned int count4 = count * 4;

        // create vertex array
        sf::VertexArray m_vertices(sf::Quads, 4);
        m_vertices.resize(count4);

        int index = 0;
        for (auto it = _batch.begin(); it != _batch.end(); ++it, index += 4)
        {
            sf::Vertex* quad = &m_vertices[index];

            // color

            for (int i=0; i<4; ++i)
                quad[i].color = it->color;

            // pos

            float x0 = -it->x * it->scaleX;
            float y0 = -it->y * it->scaleY;
            float x1 = x0;
            float y1 = (-it->y + _lastSprite->GetHeight()) * it->scaleY;
            float x2 = (-it->x + _lastSprite->GetWidth()) * it->scaleX;
            float y2 = y1;
            float x3 = x2;
            float y3 = y0;

            float angle = it->rotation / 180.f * (float)M_PI;
            float cos = cosf(angle);
            float sin = sinf(angle);

            quad[0].position.x = it->px + x0 * cos - y0 * sin;
            quad[0].position.y = it->py + x0 * sin + y0 * cos;
            quad[1].position.x = it->px + x1 * cos - y1 * sin;
            quad[1].position.y = it->py + x1 * sin + y1 * cos;
            quad[2].position.x = it->px + x2 * cos - y2 * sin;
            quad[2].position.y = it->py + x2 * sin + y2 * cos;
            quad[3].position.x = it->px + x3 * cos - y3 * sin;
            quad[3].position.y = it->py + x3 * sin + y3 * cos;

            // uv

            sf::Vector2f offset(0.0f, 0.0f);

            // compute offset for animated sprites
            if(_lastSprite->GetFramesCount() > 1)
            {
                const int tiles_in_row = texture->getSize().x / (int)_lastSprite->GetWidth();
                const int tiles_in_column = texture->getSize().y / (int)_lastSprite->GetHeight();

                //const int tileNumber = (int)floor(it->frame);
                //const int tile_x = (tileNumber % tiles_in_row);
                //const int tile_y = (tileNumber / tiles_in_column);
                const int tile_x = (int)fmod(it->frame, tiles_in_row);
                const int tile_y = (int)(it->frame / tiles_in_column);

                offset.x = tile_x * _lastSprite->GetWidth();
                offset.y = tile_y * _lastSprite->GetHeight();
            }

            quad[0].texCoords.x = offset.x;
            quad[0].texCoords.y = offset.y;
            quad[1].texCoords.x = offset.x;
            quad[1].texCoords.y = offset.y + _lastSprite->GetHeight();
            quad[2].texCoords.x = offset.x + _lastSprite->GetWidth();
            quad[2].texCoords.y = offset.y + _lastSprite->GetHeight();
            quad[3].texCoords.x = offset.x + _lastSprite->GetWidth();
            quad[3].texCoords.y = offset.y;
        }

        sf::RenderStates states;
        states.texture = texture;
        states.blendMode = _lastAdditive ? sf::BlendAdd : sf::BlendAlpha;
        //states.blendMode = sf::BlendNone;

        g_renderWindow->draw(m_vertices, states);


        /*glEnable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);*/
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        _batch.clear();

    }
}
