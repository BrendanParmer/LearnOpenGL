#ifndef VOXELIZATION_H
#define VOXELIZATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void voxelizeTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
glm::ivec3 voxelizePoint(glm::vec3 p);
void sortByAxis(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2);
void sortThreeIntPoints(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2, unsigned int axis);
void ILV(glm::ivec3 P0, glm::ivec3 P1);
void fillInterior(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2);

void voxelizeTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::ivec3 P0 = voxelizePoint(p0);
	glm::ivec3 P1 = voxelizePoint(p1);
	glm::ivec3 P2 = voxelizePoint(p2);
}
/*
*	Function to assign a 3D vector composed of floats to a voxel
*/
glm::ivec3 voxelizePoint(glm::vec3 p)
{
	return glm::ivec3(static_cast<int>(p.x + 0.5f),
					  static_cast<int>(p.y + 0.5f),
					  static_cast<int>(p.z + 0.5f));
}

/*
* reorders voxeled vertices by the triangle's dominant axis
*/
void sortByAxis(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2)
{
	//triangle edges
	glm::ivec3 E0 = P0 - P1;
	glm::ivec3 E1 = P0 - P2;

	//determine what coordinate-axis the normal is most aligned with
	int x = E0.y * E1.z - E0.z * E1.y;
	int max = x;
	int y = E0.z*E1.x - E0.x*E1.z;
	if (max < y)
		max = y;
	int z = E0.x*E1.y - E0.y*E1.x;
	if (max < z)
		max = z;

	if (max == x)
		sortThreeIntPoints(P0, P1, P2, 0);
	if (max == y)
		sortThreeIntPoints(P0, P1, P2, 1);
	if (max == z)
		sortThreeIntPoints(P0, P1, P2, 2);
} 
/*
	lol dumb function but i didn't wanna copy-paste
*/
void sortThreeIntPoints(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2, unsigned int axis)
{

	if (P0[axis] > P1[axis])
		std::swap(P0[axis], P1[axis]);
	if (P0[axis] > P2[axis])
		std::swap(P0[axis], P2[axis]);
	if (P1[axis] > P2[axis])
		std::swap(P1[axis], P2[axis]);
}
void ILV(glm::ivec3 P0, glm::ivec3 P1)
{

}
void fillInterior(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2)
{

}
#endif