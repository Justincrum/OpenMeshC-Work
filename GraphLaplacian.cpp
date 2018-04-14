// GraphLaplacian.cpp : Defines the entry point for the console application.
//

#include <utility>
#include <iostream>  //Including the required packages to run the code.
#include <vector>//Required package
#include <numeric>
#include <set>

// -------------------- OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh> //Required package
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>  //Required package

typedef std::pair <int, int> IntPair;
typedef OpenMesh::PolyMesh_ArrayKernelT<>  MyMesh; //Typedef defines a simpler name for a data type.


int main(int argc, char **argv)  /* int main tells the program the output should be of integer type.
								 int argc tells the program to let me input c integer-valued arguments
								 from the command line.  char **argv tells stores the inputs as an
								 array.  If I do not want to run from command line, I can skip these?*/
{

	std::vector<std::vector<int>> Laplacian;  //Creates an initial storage location for the graph Laplacian.
	int nvertices = 0; //Starting with 0 vertices.
		MyMesh  mesh; //I think this creates a base data type that we call mesh.  This is what we will add vertices and edges to.
					  // check command line options
		if (argc != 2) //Checking if the number of arguments being input is not equal to 4.
		{
			std::cerr << "Usage:  " << argv[0] << " #infile\n"; //std is a built in namespace of functions in c++
																				   //std::cerr calls function cerr from std.  cerr stands for console error and is used to send an error message.
			return 1;  //1 represents an error occured.
		}
		// read mesh from stdin
		if (!OpenMesh::IO::read_mesh(mesh, argv[1]))  //Making sure the program has a mesh to read (?)
		{
			std::cerr << "Error: Cannot read mesh from " << argv[1] << std::endl;  //If the above if statement fails to find a mesh, return an error.
			return 1; //1 symbolizes error.
		}
								 //Checking neighbors of each vertex.
		MyMesh::VertexIter          v_it, v_end(mesh.vertices_end());  //This is what allows me to iterate through each vertex (?)
		MyMesh::VertexVertexIter    vv_it;  /*VertexVertexIter I think will sub-iterate through at each step of VertexIter.
											So this line lets me access the neighbors of each vertex in the in mesh.*/
		//unsigned int                i, N(atoi(argv[1])); //Best guess is this is taking in the input number of smoothing iterations we want.

		int nv;
		//Count the number of vertices on the mesh.
		nv = mesh.n_vertices();

			//Next loop reshapes the vector of vectors to be a square matrix of size degree x degree.
			Laplacian.resize(nv);
			for (int i = 0; i < nv; i++) {
				Laplacian[i].resize(nv);
			}

			for (int i = 0; i < nv; i++) {
				for (int j = 0; j < nv; j++) {
					Laplacian[i][j] = 0;
				}
			}

			std::vector<IntPair> identities;

			for (v_it = mesh.vertices_begin(); v_it != v_end; ++v_it) //Cycle through each vertex in the mesh.
			{
							   //The following line is the circulator that checks for neighbors of the original vertex.
				for (vv_it = mesh.vv_iter(*v_it); vv_it ; ++vv_it) //This loop inputs a value of 1 at each neighbor.
				{
					Laplacian[v_it ->idx()][vv_it ->idx()]=1;
					//identities.push_back(std::make_pair(v_it->idx(), vv_it->idx()));
				}
			}

//			for (int i = 0; i < identities.size(); i++)
//			{
//				std::cout << identities[i].first << ", " << identities[i].second << std::endl;
//			}


			std::vector<int> val; //Create a storage location for the valence of a vertex.
			val.resize(nv);
			//Compute the valence of each vertex.
			for (int i = 0; i < nv; i++) {
				int sum = 0;
				for (int j = 0; j < nv; j++) {
					sum = sum + Laplacian[i][j];
				}
				val[i] = sum;
				
				}
			//In the diagonal, store the negative of the valence of the vertex.
			for (int i = 0; i < nv; i++) {
				for (int j = 0; j < nv; j++) {
					if (i == j)
						Laplacian[i][j] = -val[i];
				}
			}

			for (const std::vector<int> &v : Laplacian)
			{
				for (int x : v) std::cout << x << ' ';
				std::cout << std::endl;
			}

			
			std::cin.get();

		return 0; //If everything runs correctly it returns 0 to symbolize no errors.
	}
