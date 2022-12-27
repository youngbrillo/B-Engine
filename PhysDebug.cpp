#include "PhysDebug.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static void t_CheckOpenGLError(const char* location)
{
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		fprintf(stderr, "%s::\tOpenGL error = %d\n", location, errCode);
		//assert(false);
	}
	
}


GLRender_Base::GLRender_Base()
	: m_vaoId(0), m_programId(0), m_MVPUniform(0)
	, m_vertexAttribute(0), m_colorAttribute(0), m_sizeAttribute(0)
	, m_count(0), hasPointBuffer(false)
	, m_vboIds_size(0)
{
}

#define T_BUFFER_OFFSET(x)  ((const void*) (x))


const char* vs = \
"#version 330\n"
"layout(location = 0) in vec2 v_position;\n"
"layout(location = 1) in vec4 v_color;\n"
"layout(location = 2) in float v_size;\n"
"out vec4 f_color;\n"
"\n"
"uniform mat4 view;\n"
"\n"
"void main(void)\n"
"{\n"
"	f_color = v_color;\n"
"	gl_Position = view * vec4(v_position, 0.0f, 1.0f);\n"
"   gl_PointSize = v_size;\n"
"}\0";

const char* fs = \
"#version 330\n"
"in vec4 f_color;\n"
"out vec4 color;\n"
"void main(void)\n"
"{\n"
"	color = f_color;\n"
"}\0";


void GLRender_Base::Create(Shader* _externShader, bool _hasPB)
{
	hasPointBuffer = _hasPB;
	m_shader = _externShader;

	//if (!_externShader)
	{
		compileProgramId(vs, fs, m_programId);
	//	printf("GL_RENDER_BASE::CREATE::\n\tCompiling default program id (%i)\n", m_programId);
	}
	//else
	//{
	//	m_programId = m_shader->id;
	//	printf("GL_RENDER_BASE::CREATE::\n\tCopying custom program id(%i)\n", m_programId);
	//}

	m_MVPUniform = glGetUniformLocation(m_programId, "view");
	//printf("\tprogramId? (%i), MVP Uniform? (%i)\n", m_programId, m_MVPUniform);


	m_vertexAttribute = 0;
	m_colorAttribute = 1;
	//determine how large or small vboIds should be
	m_vboIds_size = hasPointBuffer ? 3 : 2;
	m_vboIds = new GLuint[m_vboIds_size];
	//m_vboIds;

	// Generate
	glGenVertexArrays(1, &m_vaoId);
	glGenBuffers(m_vboIds_size, m_vboIds);

	glBindVertexArray(m_vaoId);
	glEnableVertexAttribArray(m_vertexAttribute);
	glEnableVertexAttribArray(m_colorAttribute);
	if (hasPointBuffer) { m_sizeAttribute = 2; glEnableVertexAttribArray(m_sizeAttribute); }

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
	glVertexAttribPointer(m_vertexAttribute, 2, GL_FLOAT, GL_FALSE, 0, T_BUFFER_OFFSET(0));
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

	// Color buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
	glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, T_BUFFER_OFFSET(0));
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

	// point buffer
	if (hasPointBuffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
		glVertexAttribPointer(m_sizeAttribute, 1, GL_FLOAT, GL_FALSE, 0, T_BUFFER_OFFSET(0));
		glBufferData(GL_ARRAY_BUFFER, sizeof(sizes), sizes, GL_DYNAMIC_DRAW);
	//	printf("\t> > > establishing point buffer\n");
	}


	//printf("\tVBO size (%i) > Vertex buffer (%i), color buffer (%i), pointBuffer (%i)\n", m_vboIds_size, m_vertexAttribute, m_colorAttribute, m_sizeAttribute);
	t_CheckOpenGLError(" - Creation of GL Render Base Class - ");
	//printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_count = 0;
}

void GLRender_Base::Destroy()
{
	if (m_vaoId)
	{
		glDeleteVertexArrays(1, &m_vaoId);
		glDeleteBuffers(3, m_vboIds);
		m_vaoId = 0;
	}

	if (m_programId && m_shader != NULL)
	{
		glDeleteProgram(m_programId);
		m_programId = 0;
	}

}

void GLRender_Base::Vertex(const b2Vec2& v, const b2Color& c, float size)
{
	if (m_count == e_maxVertices)
		Flush();

	m_vertices[m_count] = v;
	m_colors[m_count] = c;
	if (size > -1)
	{
		sizes[m_count] = size;
	}
	++m_count;
}

void GLRender_Base::bindMVP(glm::mat4 _MVP)
{
	glUniformMatrix4fv(m_MVPUniform, 1, GL_FALSE, glm::value_ptr(_MVP));
}

/// POINTS -----------------------------------------------------------------------------------
/// POINTS -----------------------------------------------------------------------------------
/// POINTS -----------------------------------------------------------------------------------
void GlRender_t_Points::Flush()
{
	if (m_count == 0) return;

	glUseProgram(m_programId); //bind shader
	bindMVP(MVP); //set Model-view-projection mat

	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[2]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(float), sizes);

	glDrawArrays(GL_POINTS, 0, m_count);

	t_CheckOpenGLError(" - GLRender Test Points ::Flush - ");


	glBindVertexArray(0);
	glUseProgram(0); //unbind shader

	m_count = 0;
}


/// LINES -----------------------------------------------------------------------------------
/// LINES -----------------------------------------------------------------------------------
/// LINES -----------------------------------------------------------------------------------

void GlRender_t_Lines::Flush()
{

	if (m_count == 0) return;

	glUseProgram(m_programId); //bind shader
	bindMVP(MVP); //set Model-view-projection mat
	

	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

	t_CheckOpenGLError(" - GLRender test lines::Flush - ");

	glDrawArrays(GL_LINES, 0, m_count);

	glBindVertexArray(0);
	glUseProgram(0); //unbind shader

	m_count = 0;

}



/// TRIANGLES  -----------------------------------------------------------------------------------
/// TRIANGLES  -----------------------------------------------------------------------------------
/// TRIANGLES  -----------------------------------------------------------------------------------
void GlRender_t_Triangles::Flush()
{
	if (m_count == 0) return;

	//if (m_shader)
	//{
	//	//should have the previous view and projection mats set already, so now modify the model only
	//	glm::mat4 model = glm::mat4(1);
	//	glm::vec3 position(0,0,0), orientationAxis(0,0,1), size(1);
	//	float orientation = 0;

	//	model = glm::translate(model, position);
	//	model = glm::rotate(model, glm::radians(orientation), orientationAxis);
	//	model = glm::scale(model, size);
	//	//m_shader->Use().SetMatrix4("model", model).SetVector4f("setColor", glm::vec4(1));
	//}
	//else
	{
		glUseProgram(m_programId); //bind shader
		bindMVP(MVP); //set Model-view-projection mat
	}

	glBindVertexArray(m_vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[0]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Vec2), m_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(b2Color), m_colors);

	glDrawArrays(GL_TRIANGLES, 0, m_count);
	//glDisable(GL_BLEND);

	t_CheckOpenGLError(" - GLRender test Triangle::Flush - ");

	glBindVertexArray(0);
	glUseProgram(0); //unbind shader

	m_count = 0;
}

/// PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER -------
/// PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER -------
/// PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER -------
/// PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER ------- PHYS DRAWER -------
	
void PhysicsDebugger::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p2 = vertices[i];
		lines->Vertex(p1, color);
		lines->Vertex(p2, color);
		p1 = p2;
	}
}

void PhysicsDebugger::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

	for (int32 i = 1; i < vertexCount - 1; ++i)
	{
		triangles->Vertex(vertices[0], fillColor);
		triangles->Vertex(vertices[i], fillColor);
		triangles->Vertex(vertices[i + 1], fillColor);
	}

	b2Vec2 p1 = vertices[vertexCount - 1];
	for (int32 i = 0; i < vertexCount; ++i)
	{
		b2Vec2 p2 = vertices[i];
		lines->Vertex(p1, color);
		lines->Vertex(p2, color);
		p1 = p2;
	}
}

void PhysicsDebugger::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
	const float k_segments = 16.0f;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);
	b2Vec2 r1(1.0f, 0.0f);
	b2Vec2 v1 = center + radius * r1;
	for (int32 i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		lines->Vertex(v1, color);
		lines->Vertex(v2, color);
		r1 = r2;
		v1 = v2;
	}
}

void PhysicsDebugger::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
	const float k_segments = 16.0f;
	const float k_increment = 2.0f * b2_pi / k_segments;
	float sinInc = sinf(k_increment);
	float cosInc = cosf(k_increment);
	b2Vec2 v0 = center;
	b2Vec2 r1(cosInc, sinInc);
	b2Vec2 v1 = center + radius * r1;
	b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	for (int32 i = 0; i < k_segments; ++i)
	{
		// Perform rotation to avoid additional trigonometry.
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		triangles->Vertex(v0, fillColor);
		triangles->Vertex(v1, fillColor);
		triangles->Vertex(v2, fillColor);
		r1 = r2;
		v1 = v2;
	}

	r1.Set(1.0f, 0.0f);
	v1 = center + radius * r1;
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 r2;
		r2.x = cosInc * r1.x - sinInc * r1.y;
		r2.y = sinInc * r1.x + cosInc * r1.y;
		b2Vec2 v2 = center + radius * r2;
		lines->Vertex(v1, color);
		lines->Vertex(v2, color);
		r1 = r2;
		v1 = v2;
	}

	// Draw a line fixed in the circle to animate rotation.
	b2Vec2 p = center + radius * axis;
	lines->Vertex(center, color);
	lines->Vertex(p, color);
}

void PhysicsDebugger::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	lines->Vertex(p1, color);
	lines->Vertex(p2, color);
}

void PhysicsDebugger::DrawTransform(const b2Transform& xf)
{
	const float k_axisScale = 0.4f;
	b2Color red(1.0f, 0.0f, 0.0f);
	b2Color green(0.0f, 1.0f, 0.0f);
	b2Vec2 p1 = xf.p, p2;

	lines->Vertex(p1, red);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	lines->Vertex(p2, red);

	lines->Vertex(p1, green);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	lines->Vertex(p2, green);
}

void PhysicsDebugger::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
	points->Vertex(p, color, size);
}

void PhysicsDebugger::DrawString(int x, int y, const char* string, ...)
{
	if (showUI == false) return;

	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(ImVec2(float(x), float(y)));
	ImGui::TextColoredV(ImColor(255, 255, 255, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

void PhysicsDebugger::DrawString(const b2Vec2& p, const char* string, ...)
{
	if (showUI == false) return;
	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(ImVec2(p.x, p.y));
	ImGui::TextColoredV(ImColor(255, 255, 255, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

void PhysicsDebugger::DrawAABB(b2AABB* aabb, const b2Color& color)
{
	b2Vec2 p1 = aabb->lowerBound;
	b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
	b2Vec2 p3 = aabb->upperBound;
	b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);

	lines->Vertex(p1, color);
	lines->Vertex(p2, color);

	lines->Vertex(p2, color);
	lines->Vertex(p3, color);

	lines->Vertex(p3, color);
	lines->Vertex(p4, color);

	lines->Vertex(p4, color);
	lines->Vertex(p1, color);
}

void PhysicsDebugger::setMat4(glm::mat4 _MAT) const
{
	triangles->setMatVP(_MAT);
	lines->setMatVP(_MAT);
	points->setMatVP(_MAT);
}

void PhysicsDebugger::Flush()
{
	triangles->Flush();
	lines->Flush();
	points->Flush();
}

void PhysicsDebugger::renderDebug()
{
	if (ImGui::TreeNode("Phys Viewer"))
	{

		ImGui::Checkbox("showUI", &showUI);
		ImGui::Checkbox("shape (debug)", &debugShape);
		ImGui::Checkbox("joint (debug)", &debugJoint);
		ImGui::Checkbox("aabb (debug)", &debugAABB);
		ImGui::Checkbox("center of mass (debug)", &debugCenterOfMass);

		ImGui::TreePop();
	}
}

void PhysicsDebugger::SetFlags_Internal()
{
	uint32 flags = 0;
	flags += debugShape * b2Draw::e_shapeBit;
	flags += debugJoint * b2Draw::e_jointBit;
	flags += debugAABB * b2Draw::e_aabbBit;
	flags += debugCenterOfMass * b2Draw::e_centerOfMassBit;
	SetFlags(flags);
}
