#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "renderer.h"

class Model {
private:
	std::vector<vec4> vertices;
	std::vector<std::vector<int> > faces;

public:
	Model(const char *filename);
	~Model();
	int num_vertices();
	int num_faces();
	vec4 get_vertex(int i);
	std::vector<int> get_face(int i);
};

#endif //__MODEL_H__
