#ifndef OCTREE_H
#define OCTREE_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Octnode
{
public:
	glm::ivec3 coordinate;
	Octnode* node0;
	Octnode* node1;
	Octnode* node2;
	Octnode* node3;
	Octnode* node4;
	Octnode* node5;
	Octnode* node6;
	Octnode* node7;

	Octnode(glm::ivec3 setCoordinate)
	{
		coordinate = setCoordinate;

		//necessary?
		node0 = NULL;
		node1 = NULL;
		node2 = NULL;
		node3 = NULL;
		node4 = NULL;
		node5 = NULL;
		node6 = NULL;
		node7 = NULL;
	}
};

#endif
