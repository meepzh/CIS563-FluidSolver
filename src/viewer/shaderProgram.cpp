//
//  shaderProgram.cpp
//  MFluidSolver

#include "shaderProgram.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <streambuf>
#include <vector>

ShaderProgram::ShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
 : _vertexShader(vertexShader), _fragmentShader(fragmentShader),
   programID(-1),
   vertexShaderID(-1), fragmentShaderID(-1),
   aVertexColorArrID(-1), aVertexPositionArrID(-1),
   uModelMatID(-1), uViewProjectionMatID(-1) {
  compile();

  // Get vertex buffer IDs
  aVertexColorArrID = glGetAttribLocation(programID, "avs_Color");
  aVertexPositionArrID = glGetAttribLocation(programID, "avs_Position");

  // Get uniform IDs
  uModelMatID = glGetUniformLocation(programID, "u_Model");
  uViewProjectionMatID = glGetUniformLocation(programID, "u_ViewProjection");

  #if MFluidSolver_DEBUG
  printf("DEBUG:SHADER: Created program ID %d\n", programID);
  if (aVertexColorArrID == -1) {
    printf("- WARN: avs_Color is not bound\n");
  }
  if (aVertexPositionArrID == -1) {
    printf("- WARN: avs_Position is not bound\n");
  }
  if (uModelMatID == -1) {
    printf("- WARN: u_Model is not bound\n");
  }
  if (uViewProjectionMatID == -1) {
    printf("- WARN: u_ViewProjection is not bound\n");
  }
  #endif
}

ShaderProgram::~ShaderProgram() {
  glDeleteProgram(programID);
}

void ShaderProgram::draw(Drawable *d) {
  glUseProgram(programID);

  // Insert data to attribute variables
  if (aVertexColorArrID != -1 && d->bindColorBuffer()) {
    glEnableVertexAttribArray(aVertexColorArrID);
    glVertexAttribPointer(aVertexColorArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }
  if (aVertexPositionArrID != -1 && d->bindPositionBuffer()) {
    glEnableVertexAttribArray(aVertexPositionArrID);
    glVertexAttribPointer(aVertexPositionArrID, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  }

  // Draw (indices)
  d->bindIndexBuffer();
  glDrawElements(d->drawMode(), d->idxCount(), GL_UNSIGNED_INT, NULL);

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

// Source: https://github.com/opengl-tutorials/ogl/blob/master/common/texture.cpp
#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint ShaderProgram::loadDDS(const std::string &file) {
  unsigned char header[124];
  FILE *fp;

  // Try to open the file
  fp = fopen(file.c_str(), "rb");
  if (fp == NULL){
    std::fprintf(stderr, "ERROR: %s could not be opened.\n", file.c_str()); getchar();
    return 0;
  }

  // Verify the type of file
  char filecode[4];
  fread(filecode, 1, 4, fp);
  if (std::strncmp(filecode, "DDS ", 4) != 0) {
    fclose(fp);
    return 0;
  }

  // Get the surface desc
  fread(&header, 124, 1, fp);

  unsigned int height      = *(unsigned int *)&(header[8 ]);
  unsigned int width       = *(unsigned int *)&(header[12]);
  unsigned int linearSize  = *(unsigned int *)&(header[16]);
  unsigned int mipMapCount = *(unsigned int *)&(header[24]);
  unsigned int fourCC      = *(unsigned int *)&(header[80]);

  unsigned char *buffer;
  unsigned int bufsize;
  // How big is it going to be including all mipmaps?
  bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
  buffer = (unsigned char *) malloc(bufsize * sizeof(unsigned char));
  fread(buffer, 1, bufsize, fp);
  // Close the file pointer
  fclose(fp);

  unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
  unsigned int format;
  switch (fourCC) {
  case FOURCC_DXT1:
    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    break;
  case FOURCC_DXT3:
    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    break;
  case FOURCC_DXT5:
    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    break;
  default:
    free(buffer);
    return 0;
  }

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);
  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

  unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  unsigned int offset = 0;

  // Load the mipmaps
  for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
    unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
      0, size, buffer + offset);

    offset += size;
    width  /= 2;
    height /= 2;

    // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
    if (width < 1) width = 1;
    if (height < 1) height = 1;
  }

  printf("INFO: Loaded texture %s\n", file.c_str());

  free(buffer);
  return textureID;
}

void ShaderProgram::compile() {
  std::printf("INFO: Loading shader program (VS:%s, FS:%s)\n", _vertexShader.c_str(), _fragmentShader.c_str());

  vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read shader files to string
  std::ifstream vsStream(_vertexShader);
  std::string vsText((std::istreambuf_iterator<char>(vsStream)), std::istreambuf_iterator<char>());
  std::ifstream fsStream(_fragmentShader);
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
    std::fprintf(stderr, "ERROR: Error compiling vertex shader %s %d: %s\n", _vertexShader.c_str(), &vsErrMessage[0]);
  }

  char const *fsTextPtr = fsText.c_str();
  glShaderSource(fragmentShaderID, 1, &(fsTextPtr), NULL);
  glCompileShader(fragmentShaderID);
  glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileResult);
  glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &compileLogLen);
  if (compileLogLen > 1) {
    std::vector<char> fsErrMessage(compileLogLen + 1);
    glGetShaderInfoLog(fragmentShaderID, compileLogLen, NULL, &fsErrMessage[0]);
    std::fprintf(stderr, "ERROR: Error compiling fragment shader %s: %s\n", _fragmentShader.c_str(), &fsErrMessage[0]);
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
    std::fprintf(stderr, "ERROR: Error linking program (VS:%s, FS:%s): %s\n", _vertexShader.c_str(), _fragmentShader.c_str(), &programErrMessage[0]);
  }

  // Free space: http://gamedev.stackexchange.com/questions/47910/after-a-succesful-gllinkprogram-should-i-delete-detach-my-shaders
  glDetachShader(programID, vertexShaderID);
  glDetachShader(programID, fragmentShaderID);
  glDeleteShader(vertexShaderID);
  glDeleteShader(fragmentShaderID);
}