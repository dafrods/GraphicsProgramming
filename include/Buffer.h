// OpenGL uses a context based or is a large state machine that keeps tracks of handlers used in the lifetime of a program.
// You typically won't need to store explicit references or pointers or any data its going to use in an object as 
// OpenGL itself would take care of those itself by refering to calls like "glBufferData"



#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace OpenGL
{


class VertexBuffer
{
    GLuint VBOID;
    GLuint EBOID = -1;
    GLuint VAOID;

public:
    VertexBuffer()
    {
        glGenBuffers(1, &VBOID);
        glGenVertexArrays(1, &VAOID);
    }

    VertexBuffer(const GLfloat *data)
    {
        VertexBuffer();
        bindVBO();
        sendData(data);
    }

    VertexBuffer(const GLfloat *data, const GLfloat *indices)
    {
        VertexBuffer();
    }
    
    void setIndices()
    {

    }

    // Sets currently selected buffer to this handler
    void bindVBO()
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBOID);
    }
    void bindEBO()
    {
        if (EBOID <= 0)
            throw "There is no EBO handler defined";
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOID);
    }

    void bindVAO()
    {
        glBindVertexArray(VAOID);
    }

    // Sends an array of vertices to the buffer
    void sendData(const void *data, GLenum usage = GL_STATIC_DRAW)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, usage);
    }
    
    int getVBOID()
    {
        return VBOID;
    }
    int getEBOID()
    {
        return EBOID;
    }
    int getVAOID()
    {
        return VAOID;
    }
private:

};

}

