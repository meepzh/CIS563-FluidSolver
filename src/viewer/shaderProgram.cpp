//
//  shaderProgram.cpp
//  MFluidSolver

#include "shaderProgram.hpp"

#include <cstdio>
#include <fstream>
#include <streambuf>
#include <vector>

ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader) {
  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read shader files to string
  std::ifstream vsStream(vertexShader);
  std::string vsText((std::istreambuf_iterator<char>(vsStream)), std::istreambuf_iterator<char>());
  std::ifstream fsStream(fragmentShader);
  std::string fsText((std::istreambuf_iterator<char>(vsStream)), std::istreambuf_iterator<char>());

  // TODO: What happens if files don't exist or are empty???

  // Compile
  GLint compileResult = GL_FALSE;
  int compileLogLen;

  char const *vsTextPtr = vsText.c_str();
  glShaderSource(vertexShaderID, 1, &(vsTextPtr), NULL);
  glCompileShader(vertexShaderID);
  glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileResult);
  glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 0) {
    std::vector<char> vsErrMessage(compileLogLen + 1);
    glGetShaderInfoLog(vertexShaderID, compileLogLen, NULL, &vsErrMessage[0]);
    printf("Error compiling vertex shader %s: %s\n", vertexShader, &vsErrMessage[0]);
  }

  char const *fsTextPtr = fsText.c_str();
  glShaderSource(fragmentShaderID, 1, &(fsTextPtr), NULL);
  glCompileShader(fragmentShaderID);
  glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileResult);
  glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 0) {
    std::vector<char> fsErrMessage(compileLogLen + 1);
    glGetShaderInfoLog(fragmentShaderID, compileLogLen, NULL, &fsErrMessage[0]);
    printf("Error compiling fragment shader %s: %s\n", fragmentShader, &fsErrMessage[0]);
  }

  // Link the two programs
  programID = glCreateProgram();
  glAttachShader(programID, vertexShaderID);
  glAttachShader(programID, fragmentShaderID);
  glLinkProgram(programID);
  glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 0 ){
    std::vector<char> programErrMessage(compileLogLen + 1);
    glGetProgramInfoLog(programID, compileLogLen, NULL, &programErrMessage[0]);
    printf("Error linking program with %s and %s: %s\n", vertexShader, fragmentShader, &programErrMessage[0]);
  }

  // Free space: http://gamedev.stackexchange.com/questions/47910/after-a-succesful-gllinkprogram-should-i-delete-detach-my-shaders
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);

  // Get vertex buffer IDs
  aVertexPositionArrID = glGetAttribLocation(programID, "avs_Position");

  // Get uniform IDs
  uColorVecID = glGetUniformLocation(programID, "uColor");
  uModelMatID = glGetUniformLocation(programID, "uModel");
  uViewProjectionMatID = glGetUniformLocation(programID, "uViewProjection");
}

void ShaderProgram::draw(Drawable *d) {

}

void ShaderProgram::glDelete() {
  glDeleteProgram(programID);
}