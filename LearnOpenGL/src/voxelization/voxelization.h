/*
TODO
	-separate ILV functions for adding to list vs octree? something to think about i suppose
	-implement fillInterior algorithm
		-learn about iterators. it's gonna be the only efficient way to go through all these lists

	-implement way to do this for every triangle in mesh. may be difficult because of index nonsense.
		-perhaps not, we get easy access to every triangle in a blender mesh. in here it might be
		more difficult
	-create custom vector class of uint16_t's for maybe half the memory? something to test out i suppose
	
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

#include <stdexcept>
#include <list>

#include "octree.h"
#include "math.h"


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
std::list<glm::ivec3> getSubSequence(std::list<glm::ivec3> edge, axis w, int compare);
bool lineCondition(glm::ivec3 point, axis w, int dU, int dV, int U, int V);

void addVoxelToOctree(glm::ivec3 P, uint8_t depth, Octnode root);

int smallIntPow(int x, uint8_t p); //from math.h





/*
* function that voxelizes a triangle given three floating point vertices, p0, p1, and p2
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
* 
*	glm::vec3 p - the point to be voxelized
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
* 
*	glm::ivec3 P0, P1, P2 - the voxelized triangle vertices
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
	
	glm::ivec3 P0, P1, P2 - the voxelized triangle vertices
	axis domAxis - the dominant axis of the triangle
*/
void sortThreeIntPoints(glm::ivec3 P0, glm::ivec3 P1, glm::ivec3 P2, axis domAxis)
{

	if (P0[domAxis] > P1[domAxis])
		std::swap(P0[domAxis], P1[domAxis]);
	if (P0[domAxis] > P2[domAxis])
		std::swap(P0[domAxis], P2[domAxis]);
	if (P1[domAxis] > P2[domAxis])
		std::swap(P1[domAxis], P2[domAxis]);
}

/*
* function that voxelizes a 3D line with integer endpoints
* not perfectly accurate, but more efficient than a floating point implementation
* 
* glm::ivec3 P0, P1, P2 - the voxelized triangle vertices
* std::list<glm::ivec3> list - the list that new voxels will be added to
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

/*
	function to fill the interior of the triangle

	it splices the triangle by the dominant axis, creating a bunch of 2D polygons
	then, it determines the best endpoints on opposite sides of the triangle
	so we don't have to call ILV() more times than we need to

	std::list<glm::ivec3> E0, E1 - the edges of the original triangle
		-note: E0 is a hybrid of the original E0 and E2
	glm::ivec3 P0, P2 - the two points connected directly by E1
	axis domAxis - the dominant axis of the triangle
*/
void fllInterior(std::list<glm::ivec3> E0, 
				 std::list<glm::ivec3> E1, 
				 glm::ivec3 P0, 
				 glm::ivec3 P2, 
				 axis domAxis)
{
	std::list<glm::ivec3> fullTriangle;

	//splices triangle into 2D splices based on the dominant axis
	for (uint8_t i = 0; i < P2[domAxis] - P0[domAxis]; i++)
	{
		int slice = P0[domAxis] + i;
		std::list<glm::ivec3> sliceE0 = getSubSequence(E0, domAxis, slice);
		std::list<glm::ivec3> sliceE1 = getSubSequence(E1, domAxis, slice);

		auto E0It = sliceE0.begin();
		
		auto E1It = sliceE1.begin();
		
		ILV(*E0It, *E1It, fullTriangle);
		
		/*
		while (*E0It != NULL && *E1It != NULL)
		{
			if (lineCondition(*std::next(E0It, 1), domAxis, 
		}
		*/
		
	}

	/*
		for x in fullTriangle: //might wanna make separate ILV functions for adding to list and to Octree
			addVoxelToOctree(x, depth, root);
	*/
}

/*
	helper function to fillInterior() that gives subsequence of edge by the dominant axis

	std::list<glm::ivec3> edge - the triangle edge we are splicing
	axis w - the dominant axis
	int compare - what w-level we are comparing it to

	NTS: needs optimized so we only traverse the original edge once, probably need to use iterators
*/
std::list<glm::ivec3> getSubSequence(std::list<glm::ivec3> edge, axis w, int compare)
{
	std::list<glm::ivec3> subsequence;
	for (const auto& point : edge)
	{
		if (point[w] == compare)
			subsequence.push_back(point);
	}
		
}

/*
	helper function to fillInterior() that determines which endpoints to draw lines at
	if the condition is true, then we can try the next point in the list

	glm::ivec3 point - the potential endpoint
	axis w - the dominant axis
	int dU, dV - the respective distances along the non-dominant axis of the last drawn line
	int U, V - the non-dominant axis coordinates of the last point on the edge we are testing on
*/
bool lineCondition(glm::ivec3 point, axis w, int dU, int dV, int U, int V)
{
	int m = dV * U - dU * V;
	int n = abs(dU) + abs(dV);
	
	int u, v;
	
	if (w == 0)
	{
		u = point.y;
		v = point.z;
	}
	else if (w == 1)
	{
		u = point.z;
		v = point.x;
	}
	else if (w == 2)
	{
		u = point.x;
		v = point.y;
	}
	else
		throw std::invalid_argument("Axis w must be 0, 1, or 2");

	int compare = dV * u - dU * v;
	return (m - n <= compare && compare <= m + n);
	
}

/*
	adds a voxel to an octree

	glm::ivec3 P: coordinates of the voxel
	uint8_t depth: user-defined depth of the octree
	Octnode root: the root of our octree, used for easy level of detail calculations
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