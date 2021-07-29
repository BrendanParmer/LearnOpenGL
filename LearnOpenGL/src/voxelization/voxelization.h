/*
MUSINGS
	-would it be more efficient if we converted all the vertices to their voxels first,
	reconnected the mesh, and then filled it in?
		-how would we make sure we refilled the mesh right? (ex. holes)
			-might be why i haven't come across methods like this (as they would require intersection tests!)
	-learn about octrees and how they're used in voxelization

TODO
	-implement markVoxel function
		-just have a list of ivec3s? seems awful expensive memory-wise
		-we do need access to the x, y, and z components of previously marked voxels tho, 
		 so maybe a list is good. perhaps separate lists for separate edges? and then after the whole
		 triangle is voxelized we add them to the overall mesh voxel data structure
			-I like this idea, I think it might work well. 
			-We do need this implemented before the fillInterior algorithm can properly be finished
	-implement fillInterior algorithm
	-implement way to do this for every triangle in mesh. may be difficult because of index nonsense.
	-create custom vector class of short ints for maybe half the memory? something to test out i suppose
	
	(separate header file so voxelization stays kinda general?)
	-implement voxel data structure to cubes function 
	-implement cross-section viewer
		-probably should optimize so we only draw the 2D component (4 vertices) instead of
		 the whole cube (8 vertices), since we only need to see the top anyways
			-might get tricky if we want a smooth transition from 2D to 3D
			-does this matter if we're ultimately making this a Blender add-on anyways?
				-let's be real this engine's pretty crappy lol
	-figure out way to visualize difference between -1, 0 (current), and +1 cross sections
		-probably involves using color and alpha channels

*/



#ifndef VOXELIZATION_H
#define VOXELIZATION_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "octree.h"
#include "math.h"
#include <list>

typedef uint8_t axis; //x=0, y=1, z=2

void voxelizeTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
glm::ivec3 voxelizePoint(glm::vec3 p);
axis dominantAxis(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2);

void sortThreeIntPoints(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2, axis anAxis);
void ILV(glm::ivec3 P0, glm::ivec3 P1, std::list<glm::ivec3> list);
void fillInterior(std::list<glm::ivec3> E1, 
				  std::list<glm::ivec3> E2, 
				  glm::ivec3 P0, 
				  glm::ivec3 P2, 
				  axis domAxis);

void addVoxelToOctree(glm::ivec3 P, uint8_t depth, Octnode root);

int smallIntPow(int x, uint8_t p); //from math.h

/*
* function that voxelizes a triangle given three floating point vertices
*/
void voxelizeTriangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::ivec3 P0 = voxelizePoint(p0);
	glm::ivec3 P1 = voxelizePoint(p1);
	glm::ivec3 P2 = voxelizePoint(p2);
	
	axis domAxis = dominantAxis(P0, P1, P2);
	sortThreeIntPoints(P0, P1, P2, domAxis);

	//establishes edge voxels in a linked list
	std::list<glm::ivec3> E0;
	std::list<glm::ivec3> E1;
	std::list<glm::ivec3> E2;

	ILV(P0, P1, E0);
	ILV(P0, P2, E1);
	ILV(P1, P2, E2);

	//creates hybrid edge
	for (glm::ivec3 x : E2)
	{
		E0.push_back(x);
	}
	fillInterior(E0, E1, P0, P2, domAxis);
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
*	approximates the dominant axis by taking the cross product of the 
*	integer approximations of the edge vectors, determined by voxelized
*	vertex vectors
*/
axis dominantAxis(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2)
{
	glm::ivec3 E0 = P0 - P1;
	glm::ivec3 E1 = P0 - P2;

	int x = E0.y * E1.z - E0.z * E1.y;
	int max = x;
	int y = E0.z * E1.x - E0.x * E1.z;
	if (max < y)
		max = y;
	int z = E0.x * E1.y - E0.y * E1.x;
	if (max < z)
		max = z;

	if (max == x)
		return 0;
	if (max == y)
		return 1;
	if (max == z)
		return 2;
}

/*
	helper function that reassigns ordering based on a given axis
	maybe unnecessary now that we've reordered where we determine the dominant axis
*/
void sortThreeIntPoints(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2, axis swapAxis)
{

	if (P0[swapAxis] > P1[swapAxis])
		std::swap(P0[swapAxis], P1[swapAxis]);
	if (P0[swapAxis] > P2[swapAxis])
		std::swap(P0[swapAxis], P2[swapAxis]);
	if (P1[swapAxis] > P2[swapAxis])
		std::swap(P1[swapAxis], P2[swapAxis]);
}

/*
* function that voxelizes a 3D line with integer endpoints
* isn't quite as accurate as a floating-point algorithm would be but also realtime would be nice
*/
void ILV(glm::ivec3 P0, glm::ivec3 P1, std::list<glm::ivec3> list)
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
		list.push_back(currentP);
	}
}

void fillInterior(std::list<glm::ivec3> E0, 
				  std::list<glm::ivec3> E1, 
				  glm::ivec3 P0, 
				  glm::ivec3 P2, 
				  axis domAxis)
{
	for (uint8_t i = 0; i < P2[domAxis] - P0[domAxis]; i++)
	{
		/*
		slice? = P0[domAxis] + i + 1/2(why 1/2? wouldn't this mean floating point arithmetic?)
		std::list<glm::ivec3> E0_slice = GetSubSequence(E0, slice)
		std::list<glm::ivec3> E1_slice = GetSubSequence(E1, slice)
		while (E0_slice != NULL || E1_slice != NULL)
		{
			startP = GetNextInSlice(E0_slice)
			stopP = GetNextInSlice(E1_slice)
			ILV(startP, stopP)
		}
		*/
		
	}
}

/*
	adds a voxel to an octree

	P: coordinates of the voxel
	depth: user-defined depth of the octree
*/
void addVoxelToOctree(glm::ivec3 P, uint8_t depth, Octnode root)
{
	//bitwise little thing to keep track of which octant our point is in 
	//relative to the main node's coordinate
	
	// x = 4 (100), y = 2 (010), and z = 1 (001)
	//if the point's x-coordinate is greater than the main node's, then we put a 1 in the 4 spot
	//this helps us avoid messy, long chains of if statements
	uint8_t a = 0;

	Octnode main = root; //root's parent is NULL

	uint8_t i = 1;
	while (i < depth)
	{
		//determines relative octant and sets a new point
		int half = smallIntPow(2, depth - 1 - i);
		glm::ivec3 newPoint = main.coordinate;

		//x
		if (P.x > main.coordinate.x)
		{
			a |= 4;
			newPoint.x += half;
		}
		else
			newPoint.x -= half;

		//y
		if (P.y > main.coordinate.y)
		{
			a |= 2;
			newPoint.y += half;
		}
		else
			newPoint.y -= half;
		
		//z
		if (P.z > main.coordinate.z)\
		{
			a |= 1;
			newPoint.z += half;
		}
		else
			newPoint.z -= half;

		//assign child
		if (i == 1)
		{
			if (root.children[a] == NULL)
				root.children[a] = new Octnode(newPoint, &main);
			main = *root.children[a]; //doesn't seem good
		}
		else
		{
			if (main.children[a] == NULL)
				main.children[a] = new Octnode(newPoint, &main);
			main = *main.children[a];
		}
		i++;
	}
}
#endif