
#ifndef DRAW_H
#define DRAW_H

#define GLFW_INCLUDE_NONE
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "b2_math.h"

struct b2AABB;
struct GLRenderPoints;
struct GLRenderLines;
struct GLRenderTriangles;
struct GLFWwindow;

//
struct Camera
{
    Camera()
    {
        m_center.Set(0.0f, 0.0f);
        m_zoom = 1.0f;
        m_width = 1280;
        m_height = 800;
        m_extent = 25.0;
    }

    b2Vec2 ConvertScreenToWorld(const b2Vec2& screenPoint);
    b2Vec2 ConvertWorldToScreen(const b2Vec2& worldPoint);
    void BuildProjectionMatrix(float* m, float zBias);

    b2Vec2 m_center;
    float m_zoom;
    int32 m_width;
    int32 m_height;
    double m_extent;
};

// This class implements debug drawing callbacks that are invoked
// inside b2World::Step.
class DebugDraw
{
public:
    DebugDraw();
    ~DebugDraw();

    void Create();
    void Destroy();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawSolidRect(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);

    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

    void DrawTransform(const b2Transform& xf);

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color);

    void DrawString(int x, int y, const char* string, ...);

    void DrawString(const b2Vec2& p, const char* string, ...);

    void DrawAABB(b2AABB* aabb, const b2Color& color);

    void Flush();

    GLRenderPoints* m_points;
    GLRenderLines* m_lines;
    GLRenderTriangles* m_triangles;
};

extern Camera g_camera;
//extern GLFWwindow* g_mainWindow;

#endif
