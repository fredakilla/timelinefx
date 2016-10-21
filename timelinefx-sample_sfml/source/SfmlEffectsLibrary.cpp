#include "SfmlEffectsLibrary.h"
#include "TLFXPugiXMLLoader.h"

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <cmath>
#include <assert.h>


extern sf::RenderWindow* g_renderWindow;


TLFX::XMLLoader* MarmaladeEffectsLibrary::CreateLoader() const
{
    return new TLFX::PugiXMLLoader(0);
}

TLFX::AnimImage* MarmaladeEffectsLibrary::CreateImage() const
{
    return new MarmaladeImage();
}

bool MarmaladeImage::Load( const char *filename )
{
    _texture = new sf::Texture();
    assert(_texture->loadFromFile(filename));
    return true;
}

MarmaladeImage::MarmaladeImage()
    : _texture(NULL)
{

}

MarmaladeImage::~MarmaladeImage()
{
    if (_texture)
        delete _texture;
}

sf::Texture *MarmaladeImage::GetTexture() const
{
    return _texture;
}

MarmaladeParticleManager::MarmaladeParticleManager( int particles /*= particleLimit*/, int layers /*= 1*/ )
    : TLFX::ParticleManager(particles, layers)
    , _lastSprite(NULL)
    , _lastAdditive(true)
{

}

void MarmaladeParticleManager::DrawSprite( TLFX::AnimImage* sprite, float px, float py, float frame, float x, float y, float rotation, float scaleX, float scaleY, unsigned char r, unsigned char g, unsigned char b, float a , bool additive )
{
    assert(frame == 0);

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

void MarmaladeParticleManager::Flush()
{
    if (g_renderWindow && !_batch.empty() && _lastSprite)
    {
        int count = _batch.size();
        int count4 = count * 4;

        sf::VertexArray m_vertices(sf::Quads, 4);
        m_vertices.resize(count4);

        int index = 0;
        for (auto it = _batch.begin(); it != _batch.end(); ++it, index += 4)
        {
            sf::Vertex* quad = &m_vertices[index];

            // color

            for (int i=0; i<4; ++i)
            {
                quad[i].color = sf::Color(it->color.r, it->color.g, it->color.b, it->color.a);
            }


            // pos

            float x0 = -it->x * it->scaleX;
            float y0 = -it->y * it->scaleY;
            float x1 = x0;
            float y1 = (-it->y + _lastSprite->GetHeight()) * it->scaleY;
            float x2 = (-it->x + _lastSprite->GetWidth()) * it->scaleX;
            float y2 = y1;
            float x3 = x2;
            float y3 = y0;

            float cos = cosf(it->rotation / 180.f * (float)M_PI);
            float sin = sinf(it->rotation / 180.f * (float)M_PI);

            quad[0].position.x = it->px + x0 * cos - y0 * sin;
            quad[0].position.y = it->py + x0 * sin + y0 * cos;
            quad[1].position.x = it->px + x1 * cos - y1 * sin;
            quad[1].position.y = it->py + x1 * sin + y1 * cos;
            quad[2].position.x = it->px + x2 * cos - y2 * sin;
            quad[2].position.y = it->py + x2 * sin + y2 * cos;
            quad[3].position.x = it->px + x3 * cos - y3 * sin;
            quad[3].position.y = it->py + x3 * sin + y3 * cos;


            // uvs

            quad[0].texCoords.x = 0.0f;
            quad[0].texCoords.y = 0.0f;
            quad[1].texCoords.x = 0.0f;
            quad[1].texCoords.y = 1.0f * _lastSprite->GetHeight();
            quad[2].texCoords.x = 1.0f * _lastSprite->GetWidth();
            quad[2].texCoords.y = 1.0f * _lastSprite->GetHeight();
            quad[3].texCoords.x = 1.0f * _lastSprite->GetWidth();
            quad[3].texCoords.y = 0.0f;

        }



        sf::RenderStates states;
        states.texture = static_cast<MarmaladeImage*>(_lastSprite)->GetTexture();
        states.blendMode = _lastAdditive ? sf::BlendAlpha : sf::BlendAdd;

        g_renderWindow->draw(m_vertices, states);

        /*glEnable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
        //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);*/

        _batch.clear();

    }
}
