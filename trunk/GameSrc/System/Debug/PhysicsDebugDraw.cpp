#include "../../System/Debug/PhysicsDebugDraw.hpp"
#include "../../Interface/IRenderable.hpp"

#define OUTLINE_THICKNESS 2.0f

DebugDraw::DebugDraw(sf::RenderTarget &window)
{
    this->window = &window;
}

DebugDraw::~DebugDraw()
{

}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
    sf::VertexArray redLine(sf::Lines,2);
    sf::VertexArray greenLine(sf::Lines,2);

    b2Vec2 p1 = xf.p;
    b2Vec2 p2;

    Vec2D beginVector, endVector;

    p2 = p1 + xf.q.GetXAxis();

    beginVector = WorldToScreen( p1 );
    endVector = WorldToScreen( p2 );

    sf::Vertex redBegin(beginVector,sf::Color(255,255,0));
    sf::Vertex redEnd(endVector,sf::Color(255,255,0));
    redLine.append(redBegin);
    redLine.append(redEnd);

    p2 = p1 + xf.q.GetYAxis();

    beginVector = WorldToScreen( p1 );
    endVector = WorldToScreen( p2 );

    sf::Vertex greenBegin(beginVector,sf::Color(255,255,0));
    sf::Vertex greenEnd(endVector,sf::Color(255,255,0));
    greenLine.append(greenBegin);
    greenLine.append(greenEnd);

    window->draw(redLine);
    window->draw(greenLine);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    sf::VertexArray line(sf::Lines,2);
    sf::Color       sf_color(255*color.r,255*color.g,255*color.b);
    Vec2D beginVector, endVector;

    beginVector = WorldToScreen( p1 );
    endVector = WorldToScreen( p2 );

    line.append(sf::Vertex(beginVector,sf_color));
    line.append(sf::Vertex(endVector,sf_color));
    window->draw(line);
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
    Vec2D centerVector;
    Vec2D axisVector;
    sf::Color sf_color(255*color.r,255*color.g,255*color.b);

    centerVector = WorldToScreen( center );
    axisVector = WorldToScreen( center + radius * axis );

    sf::CircleShape circleShape = sf::CircleShape(radius*RATIO);
    circleShape.setPosition(centerVector);
    circleShape.setOrigin(radius*RATIO,radius*RATIO);
    circleShape.setFillColor(sf::Color::Transparent);
    circleShape.setOutlineColor(sf_color);
    circleShape.setOutlineThickness(OUTLINE_THICKNESS);

    sf::VertexArray axisLine(sf::Lines,2);
    axisLine.append(sf::Vertex(centerVector,sf_color));
    axisLine.append(sf::Vertex(axisVector,sf_color));

    window->draw(circleShape);
    window->draw(axisLine);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
    Vec2D centerVector;
    sf::Color sf_color(255*color.r,255*color.g,255*color.b);

    centerVector = WorldToScreen( center );

    sf::CircleShape circleShape = sf::CircleShape(radius*RATIO);
    circleShape.setPosition(centerVector);
    circleShape.setOrigin(radius*RATIO,radius*RATIO);
    circleShape.setFillColor(sf::Color::Transparent);
    circleShape.setOutlineColor(sf_color);
    circleShape.setOutlineThickness(OUTLINE_THICKNESS);

    window->draw(circleShape);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape convexShape(vertexCount);
    sf::Color       sf_color(255*color.r,255*color.g,255*color.b);

    for (int32 i=0; i<vertexCount; i++)
    {
        Vec2D vertexVector = WorldToScreen( vertices[i] );
        convexShape.setPoint(i,vertexVector);
    }

    convexShape.setFillColor(sf::Color::Transparent);
    convexShape.setOutlineColor(sf_color);
    convexShape.setOutlineThickness(OUTLINE_THICKNESS);

    window->draw(convexShape);
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::ConvexShape convexShape(vertexCount);
    sf::Color       sf_color(255*color.r,255*color.g,255*color.b);

    for (int32 i=0; i<vertexCount; i++)
    {
        Vec2D vertexVector = WorldToScreen( vertices[i] );
        convexShape.setPoint(i,vertexVector);
    }

    convexShape.setFillColor(sf::Color::Transparent);
    convexShape.setOutlineColor(sf_color);
    convexShape.setOutlineThickness(OUTLINE_THICKNESS);

    window->draw(convexShape);
}
