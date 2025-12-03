#pragma once
#ifndef MATRIX_MATH
#define MATRIX_MATH

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdio.h>

typedef float vec4[4];
typedef float mat4[16];
typedef float mat3[9];

void copyFromVec(vec4 orig, vec4 copy);
void copyToMat(mat4 orig, mat4 copy);

void multVecByVec(vec4 vec1, vec4 vec2)
{
  for(int i = 0; i < 4; i++)
    vec1[i] = vec1[i] * vec2[i];
}

void matToVec(mat4 mat, vec4 vecToBe)
{
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      vecToBe[i] += mat[i * 4 + j];
}

void multMatByVec(mat4 mat, vec4 vec)
{
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      mat[i * 4 + j] *= vec[i];
}

float magnitudeOfVec(vec4 vec)
{
  return sqrtf(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

void getMat3FromMat4(mat3 m3, mat4 m4, int R, int C)
{
  int k = 0;
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
    {
      if (i == C || j == R) continue;
      m3[k] = m4[i * 4 + j];
      k += (k < 9 ? 1 : 0);
    }
}

float getDetFromMat3(mat3 mat)
{
  float p1 = mat[0 * 3 + 2] * mat[1 * 3 + 1] * mat[2 * 3 + 0];
  float p2 = mat[0 * 3 + 1] * mat[1 * 3 + 0] * mat[2 * 3 + 2];
  float p3 = mat[0 * 3 + 0] * mat[1 * 3 + 2] * mat[2 * 3 + 1];
  float s1 = p1 + p2 + p3;

  float p4 = mat[0 * 3 + 0] * mat[1 * 3 + 1] * mat[2 * 3 + 2];
  float p5 = mat[0 * 3 + 1] * mat[1 * 3 + 2] * mat[2 * 3 + 0];
  float p6 = mat[0 * 3 + 2] * mat[1 * 3 + 0] * mat[2 * 3 + 1];
  float s2 = p4 + p5 + p6;

  return s2 - s1;
}

float vecDotProduct(vec4 vec1, vec4 vec2)
{
  float sum = 0;
  for(int i = 0; i < 4; i++)
    sum += vec1[i] * vec2[i];
  return sum;
}

void vecCrossProduct(vec4 vec1, vec4 vec2)
{
  vec4 crossProduct;
  crossProduct[0] = (vec1[1] * vec2[2] - vec1[2] * vec2[1]);
  crossProduct[1] = (vec1[2] * vec2[0] - vec1[0] * vec2[2]);
  crossProduct[2] = (vec1[0] * vec2[1] - vec1[1] * vec2[0]);
  crossProduct[3] = 0;

  copyFromVec(vec1, crossProduct);
}

void addScalarToVec(vec4 vec, float s)
{
  for(int i = 0; i < 4; i++)
    vec[i] += s;
}

void subScalarToVec(vec4 vec, float s)
{
  for(int i = 0; i < 4; i++)
    vec[i] -= s;
}

void multScalarToVec(vec4 vec, float s)
{
  for(int i = 0; i < 4; i++)
    vec[i] *= s;
}

void addVecToVec(vec4 vec1, vec4 vec2)
{
  for(int i = 0; i < 4; i++)
    vec1[i] += vec2[i];
}

void subVecFromVec(vec4 vec1, vec4 vec2)
{
  for(int i = 0; i < 4; i++)
    vec1[i] -= vec2[i];
}

void normalizeVector(vec4 vec)
{
  float magni = magnitudeOfVec(vec);
  if (magni == 0.0f) return;
  for(int i = 0; i < 4; i++)
    vec[i] /= magni;
}

void multMayByLeftMat(mat4 mat, mat4 mult)
{
  mat4 cpy;
  copyToMat(cpy, mat);
  for(int i = 0; i < 16; i++)
  {
    float val = 0;
    int r = i / 4;
    int c = i % 4;
    val += mult[r * 4 + 0] * cpy[0 * 4 + c];
    val += mult[r * 4 + 1] * cpy[1 * 4 + c];
    val += mult[r * 4 + 2] * cpy[2 * 4 + c];
    val += mult[r * 4 + 3] * cpy[3 * 4 + c];

    mat[i] = val;
  }
}

void initScalingMatrix(mat4 mat, float x, float y, float z)
{
  for(int i = 0; i < 16; i++) mat[i] = 0;
  mat[0 * 4 + 0] = x;
  mat[1 * 4 + 1] = y;
  mat[2 * 4 + 2] = z;
  mat[3 * 4 + 3] = 1.0f;
}

void initScalingMatrixS(mat4 mat, float s)
{
  for(int i = 0; i < 16; i++) mat[i] = 0;
  mat[0 * 4 + 0] = s;
  mat[1 * 4 + 1] = s;
  mat[2 * 4 + 2] = s;
  mat[3 * 4 + 3] = 1.0f;
}

void initRotationalMatrix(mat4 mat, float x, float y, float z)
{
  
  for(int i = 0; i < 16; i++) mat[i] = 0;
  //main diag
  mat[0 * 4 + 0] = cos(y) * cos(z);
  mat[1 * 4 + 1] = cos(x) * cos(z);
  mat[2 * 4 + 2] = cos(x) * cos(y);
  mat[3 * 4 + 3] = 1;

  // X
  mat[0 * 4 + 1] = -sin(z);
  mat[0 * 4 + 2] = sin(y);

  // Y
  mat[1 * 4 + 0] = sin(z);
  mat[1 * 4 + 2] = -sin(x);

  // Z
  mat[2 * 4 + 0] = -sin(y);
  mat[2 * 4 + 1] = sin(x);
}

void initTranslationalMatrix(mat4 mat, float x, float y, float z)
{
  for(int i = 0; i < 16; i++) mat[i] = (i % 4 == i / 4 ? 1 : 0);
  mat[0 * 4 + 3] = x;
  mat[1 * 4 + 3] = y;
  mat[2 * 4 + 3] = z;
}


void initProjectionMatrix(mat4 mat, float FOV, float AspectRatio, float near, float far)
{
  for(int i = 0; i < 16; i++) mat[i] = 0;

  float range = near - far;
  float A = (-far - near) / range;
  float B = 2.0f * far * near / range;

  mat[2 * 4 + 2] = A;
  mat[2 * 4 + 3] = B;
  mat[3 * 4 + 2] = 1;

  float f = 1 / tanf((FOV / 2.0f) * (M_PI / 180));

  mat[0 * 4 + 0] = f / AspectRatio;
  mat[1 * 4 + 1] = f;

}

void initLookAtMatrix(mat4 mat, vec4 position, vec4 target, vec4 upVector)
{
  vec4 cameraDirection;
  copyFromVec(cameraDirection, target);
  subVecFromVec(cameraDirection, position);
  normalizeVector(cameraDirection);

  vec4 cameraRight;
  vec4 multVec;
  copyFromVec(cameraRight, upVector);
  copyFromVec(multVec, cameraDirection);
  vecCrossProduct(cameraRight, multVec);
  normalizeVector(cameraRight);

  vec4 cameraUp;
  copyFromVec(cameraUp, cameraDirection);
  copyFromVec(multVec, cameraRight);
  vecCrossProduct(cameraUp, multVec);
  normalizeVector(cameraUp);

  initScalingMatrixS(mat, 1);
  mat[0 * 4 + 0] = cameraRight[0];
  mat[0 * 4 + 1] = cameraRight[1]; // U
  mat[0 * 4 + 2] = cameraRight[2];

  mat[1 * 4 + 0] = cameraUp[0];
  mat[1 * 4 + 1] = cameraUp[1]; // V
  mat[1 * 4 + 2] = cameraUp[2];

  mat[2 * 4 + 0] = cameraDirection[0];
  mat[2 * 4 + 1] = cameraDirection[1]; // N
  mat[2 * 4 + 2] = cameraDirection[2];

  mat4 mult;
  copyToMat(mult, mat);
  initTranslationalMatrix(mat, -position[0], -position[1], -position[2]);
  multMayByLeftMat(mat, mult);

}

void applyPerspectiveToMatrix(mat4 mat, float w)
{
  for(int i = 0; i < 16; i++)
  {
      mat[i] /= w;
  }
  mat[15] = w;
}

void initVec(vec4 vec, float k)
{
  for(int i = 0; i < 3; i++)
    vec[i] = k;
  vec[3] = 1;
}

void copyToMat(mat4 orig, mat4 copy)
{
  for(int i = 0; i < 16; i++)
      orig[i] = copy[i];
}

void copyFromVec(vec4 orig, vec4 copy)
{
  for(int i = 0; i < 4; i++)
      orig[i] = copy[i];
}

#endif
