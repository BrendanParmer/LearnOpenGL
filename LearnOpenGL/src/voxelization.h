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
void markVoxel(glm::ivec3 P);

void voxelizeTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::ivec3 P0 = voxelizePoint(p0);
	glm::ivec3 P1 = voxelizePoint(p1);
	glm::ivec3 P2 = voxelizePoint(p2);

	//necessary since we're filling in lines?
	/*
	markVoxel(P0);
	markVoxel(P1);
	markVoxel(P2);
	*/


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
/*
* function that voxelizes a 3D line with integer endpoints
* isn't quite as accurate as a floating-point algorithm would be but also realtime would be nice
*/
void ILV(glm::ivec3 P0, glm::ivec3 P1)
{
	glm::ivec3 dP = glm::ivec3(P1.x - P0.x,
							   P1.y - P0.y,
							   P1.z - P0.z);
	glm::ivec3 M = glm::ivec3(abs(dP.y * dP.z),
							  abs(dP.x * dP.z),
							  abs(dP.x * dP.y));
	glm::ivec3 L = M;
	glm::ivec3 currentP = P0;
	while (currentP != P1)
	{
		//find axis with minimum distance to next voxel region face
		unsigned int min = 0;
		for (int i = 1; i < 3; i++) //worth it to even have a for loop?
		{
			if (L[i] > L[min])
				min = i;
		}
		currentP[min] += dP[min];
		L -= glm::ivec3(L[min], L[min], L[min]);
		L[min] = 2 * M[min];
		markVoxel(currentP);
	}
}
void fillInterior(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2)
{

}
/*
TODO: actually figure out how to implement this.
I imagine it'll probably be different in this OpenGL engine and Blender
But maybe not

	-look up octrees and advantages of using them? i remember them being involved somehow
	-incorporate cube spawning stuff here too? might not be too difficult, just instance a cube at each point
*/
void markVoxel(glm::ivec3 P)
{

}
#endif