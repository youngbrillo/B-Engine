#pragma once
#ifndef PHYSDEBUG_H
#define PHYSDEBUG_H



#include <Box2D/Box2D.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class GLRender_Base {
public:
	GLRender_Base();
	~GLRender_Base() { m_shader = NULL; m_vboIds = NULL; };
	void Create(Shader* _externShader = NULL, bool isPoint = false);
	void Destroy();
	void Vertex(const b2Vec2& v, const b2Color& c, float size = -1);
	void setMatVP(glm::mat4 _MAT) { MVP = _MAT; }
	void bindMVP(glm::mat4 _MVP);

	void Flush(glm::mat4 _MVP) { MVP = _MVP; Flush(); }
	virtual void Flush() = 0;
public:
	unsigned int	m_vaoId, m_programId;
	unsigned int*	m_vboIds;
	unsigned int	m_vboIds_size;

	unsigned int	m_MVPUniform;
	unsigned int	m_vertexAttribute;
	unsigned int	m_colorAttribute;
	unsigned int	m_sizeAttribute;


	enum { e_maxVertices = 3 * 512 };
	b2Vec2 m_vertices[e_maxVertices];
	b2Color m_colors[e_maxVertices];
	float sizes[512];

	int	m_count;
	bool hasPointBuffer;
	glm::mat4 MVP;
	Shader* m_shader;

};

class GlRender_t_Points : public GLRender_Base {
public:
	GlRender_t_Points() : GLRender_Base() {};
	~GlRender_t_Points() {};

	virtual void Flush();
};

class GlRender_t_Lines : public GLRender_Base {
public:
	GlRender_t_Lines() : GLRender_Base() {};
	~GlRender_t_Lines() {};

	virtual void Flush();
};

class GlRender_t_Triangles : public GLRender_Base {
public:
	GlRender_t_Triangles() : GLRender_Base() {};
	~GlRender_t_Triangles() {};

	virtual void Flush();
};

class PhysicsDebugger : public b2Draw
{
public:
	PhysicsDebugger() : showUI(true), points(NULL), lines(NULL), triangles(NULL)
		, debugShape(true), debugAABB(false), debugJoint(true), debugCenterOfMass(false)
		, M_shader(NULL)
	{
		//printf("Initializing Physics Debugger\n");
	}
	~PhysicsDebugger() {
		Destroy();
		b2Assert(points == NULL); b2Assert(lines == NULL); b2Assert(triangles == NULL); M_shader = NULL;
	}

	void Create(Shader* _externShader = NULL)
	{
		points = new GlRender_t_Points(); points->Create(_externShader, true);
		lines = new GlRender_t_Lines(); lines->Create(_externShader);
		triangles = new GlRender_t_Triangles(); triangles->Create(_externShader);
	}
	void Destroy()
	{
		points->Destroy(); delete points; 	points = NULL;
		lines->Destroy(); delete lines; lines = NULL;
		triangles->Destroy(); delete triangles; triangles = NULL;
	}

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	void DrawTransform(const b2Transform& xf) override;

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

	void DrawString(int x, int y, const char* string, ...);

	void DrawString(const b2Vec2& p, const char* string, ...);

	void DrawAABB(b2AABB* aabb, const b2Color& color);

	void setMat4(glm::mat4 _MAT) const;

	void Flush();


	GlRender_t_Points* points;
	GlRender_t_Lines* lines;
	GlRender_t_Triangles* triangles;


public:
	bool showUI;

	bool debugShape;
	bool debugJoint;
	bool debugAABB;
	bool debugCenterOfMass;

	void renderDebug();
	void SetFlags_Internal();

	Shader* M_shader;
};

#endif // !T_PHYS_DEBUGVIEW_H

