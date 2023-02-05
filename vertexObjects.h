#pragma once
namespace GLWrapper
{
	class VertexObject
	{
	public:
		VertexObject();
		virtual ~VertexObject();
		unsigned int GetId() const { return id; }
	protected:
		unsigned int id;

	};

	//VERTEX BUFFER OBJECT
	class VBO : VertexObject
	{
	public:
		VBO();
		~VBO();

		void Generate();
		void Bind();
	};

	//VERTEXT ARRAY OBJECT
	class VAO : VertexObject
	{
	public:
		VAO();
		~VAO();

		void Generate();
		void Bind();

	};
};
