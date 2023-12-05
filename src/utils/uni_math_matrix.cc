#include "uni_math_matrix.h"

namespace universe 
{

namespace util
{

UNI_EXPORT void UNI_EXPORTED_FUNC unit_m4(float m[4][4])
{
  m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
  m[0][1] = m[0][2] = m[0][3] = 0.0f;
  m[1][0] = m[1][2] = m[1][3] = 0.0f;
  m[2][0] = m[2][1] = m[2][3] = 0.0f;
  m[3][0] = m[3][1] = m[3][2] = 0.0f;
}

}

}