/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Texture.h
  Description :   declare the Texture class
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <SOIL.h>
#include <vector>
#include <gtc/type_ptr.hpp>
#include "Shader.h"
#include "Mesh.h"

class FrameBuffer 
{
    public:
        FrameBuffer();
        ~FrameBuffer();

        void Render();

    private:
        GLuint renderTexture;
        GLuint frameBuffer;
        GLuint rbo;

        Shader* m_shader;
        Mesh* m_mesh;
};