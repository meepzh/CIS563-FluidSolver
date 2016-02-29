//
//  shaderProgram.cpp
//  MFluidSolver

#include "shaderProgram.hpp"

#include <cstdio>
#include <fstream>
#include <streambuf>
#include <vector>

ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
 : programID(-1),
   vertexShaderID(-1), fragmentShaderID(-1),
   aVertexColorArrID(-1), aVertexPositionArrID(-1),
   uModelMatID(-1), uViewProjectionMatID(-1) {
  std::printf("Loading shader program (VS:%s, FS:%s)\n", vertexShader.c_str(), fragmentShader.c_str());

  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read shader files to string
  std::ifstream vsStream(vertexShader);
  std::string vsText((std::istreambuf_iterator<char>(vsStream)), std::istreambuf_iterator<char>());
  std::ifstream fsStream(fragmentShader);
  std::string fsText((std::istreambuf_iterator<char>(fsStream)), std::istreambuf_iterator<char>());

  // Compile
  GLint compileResult = GL_FALSE;
  int compileLogLen = 0;

  char const *vsTextPtr = vsText.c_str();
  glShaderSource(vertexShaderID, 1, &(vsTextPtr), NULL);
  glCompileShader(vertexShaderID);
  glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileResult);
  glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 1) {
    std::vector<char> vsErrMessage(compileLogLen + 1);
    glGetShaderInfoLog(vertexShaderID, compileLogLen, NULL, &vsErrMessage[0]);
    printf("Error compiling vertex shader %s %d: %s\n", vertexShader.c_str(), &vsErrMessage[0]);
  }

  char const *fsTextPtr = fsText.c_str();
  glShaderSource(fragmentShaderID, 1, &(fsTextPtr), NULL);
  glCompileShader(fragmentShaderID);
  glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileResult);
  glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 1) {
    std::vector<char> fsErrMessage(compileLogLen + 1);
    glGetShaderInfoLog(fragmentShaderID, compileLogLen, NULL, &fsErrMessage[0]);
    printf("Error compiling fragment shader %s: %s\n", fragmentShader.c_str(), &fsErrMessage[0]);
  }

  // Link the two programs
  programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);
  glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 1){
    std::vector<char> programErrMessage(compileLogLen + 1);
    glGetProgramInfoLog(programID, compileLogLen, NULL, &programErrMessage[0]);
    printf("Error linking program (VS:%s, FS:%s): %s\n", vertexShader.c_str(), fragmentShader.c_str(), &programErrMessage[0]);
  }

  // Free space: http://gamedev.stackexchange.com/questions/47910/after-a-succesful-gllinkprogram-should-i-delete-detach-my-shaders
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  // Get vertex buffer IDs
  aVertexColorArrID = glGetAttribLocation(programID, "avs_Color");
  aVertexPositionArrID = glGetAttribLocation(programID, "avs_Position");

  // Get uniform IDs
  uModelMatID = glGetUniformLocation(programID, "u_Model");
  uViewProjectionMatID = glGetUniformLocation(programID, "u_ViewProjection");
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(programID);
}

void ShaderProgram::draw(Drawable *d) {
  glUseProgram(programID);

  // Insert data to attribute variables
  if (aVertexColorArrID != -1 && d->bindPositionBuffer()) {
    glEnableVertexAttribArray(aVertexColorArrID);
    glVertexAttribPointer(aVertexColorArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }
  if (aVertexPositionArrID != -1 && d->bindPositionBuffer()) {
    glEnableVertexAttribArray(aVertexPositionArrID);
    glVertexAttribPointer(aVertexPositionArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }

  // Draw (indices)
  d->bindIndexBuffer();
  glDrawArrays(d->drawMode(), 0, d->idxCount());

  // Disable attributes
  if (aVertexColorArrID != -1)
    glDisableVertexAttribArray(aVertexColorArrID);
  if (aVertexPositionArrID != -1)
    glDisableVertexAttribArray(aVertexPositionArrID);
}

void ShaderProgram::setModelMat(const glm::mat4 &modelMat) {
  glUseProgram(programID);

  if (uModelMatID != -1) {
    // http://glm.g-truc.net/0.9.2/api/a00001.html
    glUniformMatrix4fv(uModelMatID, 1, GL_FALSE, &modelMat[0][0]);
  }
}

void ShaderProgram::setViewProjectionMat(const glm::mat4 &viewProjectionMat) {
  glUseProgram(programID);

  if (uViewProjectionMatID != -1) {
    // http://glm.g-truc.net/0.9.2/api/a00001.html
    glUniformMatrix4fv(uViewProjectionMatID, 1, GL_FALSE, &viewProjectionMat[0][0]);
  }
}