/*
Copyright (c) 2016, Payet Thibault
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Monwarez Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <S3DE_Shader.h>
using namespace S3DE;
// Constructeurs et Destructeur

Shader::Shader() : 	m_vertexID(0), m_fragmentID(0), m_programID(0), m_vertexSource(), 
					m_fragmentSource()
{
}


Shader::Shader(Shader const &shaderToCopy)
{
    // Copy sources files

    m_vertexSource 		= 	shaderToCopy.m_vertexSource;
    m_fragmentSource 	= 	shaderToCopy.m_fragmentSource;

    // Load the new shader

    this->Load();
}


Shader::Shader(std::string const &vertexSource, std::string const &fragmentSource): m_vertexID(0),
													 	m_fragmentID(0), m_programID(0),
                                                     	m_vertexSource(vertexSource), m_fragmentSource(fragmentSource)
{
	this->Load();
}


Shader::~Shader()
{
    // Delete shader

    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragmentID);
    glDeleteProgram(m_programID);
}


// Méthodes

Shader& Shader::operator=(Shader const &shaderToCopy)
{
    // Copy source

    m_vertexSource 		= 	shaderToCopy.m_vertexSource;
    m_fragmentSource 	= 	shaderToCopy.m_fragmentSource;

    // Load new shader

    this->Load();

    return *this;
}


void Shader::Load()
{
    // Destroy old  shader

    if(glIsShader(m_vertexID) == GL_TRUE)
        glDeleteShader(m_vertexID);

    if(glIsShader(m_fragmentID) == GL_TRUE)
        glDeleteShader(m_fragmentID);

    if(glIsProgram(m_programID) == GL_TRUE)
        glDeleteProgram(m_programID);


    // Build Shader

    if(!( (this->BuildShader(m_vertexID, GL_VERTEX_SHADER, m_vertexSource)) && 
			(this->BuildShader(m_fragmentID, GL_FRAGMENT_SHADER, m_fragmentSource)) ) )
        throw std::string("Error during BuildShader");


    // Create program

    m_programID = glCreateProgram();


    // Attach Shader 

    glAttachShader(m_programID, m_vertexID);
    glAttachShader(m_programID, m_fragmentID);


    // Verrouillage des entrées shader

    glBindAttribLocation(m_programID, 0, "in_Vertex");
    glBindAttribLocation(m_programID, 1, "in_Color");
    glBindAttribLocation(m_programID, 2, "in_TexCoord0");
	glBindAttribLocation(m_programID, 3, "in_Normal");
	glBindAttribLocation(m_programID, 4, "in_BoneID");
	glBindAttribLocation(m_programID, 5, "in_BoneW");

    // Link  program

    glLinkProgram(m_programID);

    // Vérification du linkage

    GLint erreurLink(0);
    glGetProgramiv(m_programID, GL_LINK_STATUS, &erreurLink);


    // S'il y a eu une erreur

    if(erreurLink != GL_TRUE)
    {
        // Récupération de la taille de l'erreur

        GLint tailleErreur(0);
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de mémoire

        char *erreur = new char[tailleErreur + 1];


        // Récupération de l'erreur

        glGetShaderInfoLog(m_programID, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';




        // Libération de la mémoire et retour du booléen false

        delete[] erreur;
        glDeleteProgram(m_programID);

        throw std::string(std::string("Linking error ") + std::string(erreur));
    }



}

void Shader::Enable()
{
	glUseProgram(m_programID);
}
void Shader::Disable()
{
	glUseProgram(0);
}
GLuint Shader::GetUniformLocation(std::string const &name) const
{
	return glGetUniformLocation(m_programID,name.c_str());
}
bool Shader::BuildShader(GLuint &shader, GLenum type, std::string const &source)
{
    // Création du shader

    shader = glCreateShader(type);


    // Vérification du shader

    if(shader == 0)
    {
        std::cout << "Erreur, le type de shader (" << type << ") n'existe pas" << std::endl;
        return false;
    }


    // Flux de lecture

    std::ifstream fichier(source.c_str());


    // Test d'ouverture

    if(!fichier)
    {
        std::cout << "Erreur le fichier " << source << " est introuvable" << std::endl;
        glDeleteShader(shader);

        return false;
    }


    // Strings permettant de lire le code source

    std::string ligne;
    std::string codeSource;


    // Lecture

    while(getline(fichier, ligne))
        codeSource += ligne + '\n';


    // Fermeture du fichier

    fichier.close();


    // Récupération de la chaine C du code source

    const GLchar* chaineCodeSource = codeSource.c_str();


    // Envoi du code source au shader

    glShaderSource(shader, 1, &chaineCodeSource, 0);


    // Compilation du shader

    glCompileShader(shader);


    // Vérification de la compilation

    GLint erreurCompilation(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &erreurCompilation);


    // S'il y a eu une erreur

    if(erreurCompilation != GL_TRUE)
    {
        // Récupération de la taille de l'erreur

        GLint tailleErreur(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &tailleErreur);


        // Allocation de mémoire

        char *erreur = new char[tailleErreur + 1];


        // Récupération de l'erreur

        glGetShaderInfoLog(shader, tailleErreur, &tailleErreur, erreur);
        erreur[tailleErreur] = '\0';


        // Affichage de l'erreur

        std::cerr << "Error during building shader "<< erreur << std::endl;


        // Libération de la mémoire et retour du booléen false

        delete[] erreur;
        glDeleteShader(shader);

        return false;
    }


    // Sinon c'est que tout s'est bien passé

    else
        return true;
}

