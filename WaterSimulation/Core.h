#pragma once
// Set up GLM
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/matrix.hpp"
#include "glm/gtc/type_ptr.hpp"// added for the use of value_ptr()

//#include "D:/GitHub/waterSim/packages/nupengl.core.0.1.0.1/build/native/include/GL/glew.h"
//#include "D:/GitHub/waterSim/packages/nupengl.core.0.1.0.1/build/native/include/GLFW/glfw3.h"


#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include "GL/glew.h"
#endif
#include "GLFW/glfw3.h"

#include <vector>
#include <ctype.h>
#include <iostream>
#include <algorithm>

#include <fstream>
#include <string>
#include <sstream>
