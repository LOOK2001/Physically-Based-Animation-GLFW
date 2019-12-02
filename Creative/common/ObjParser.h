#ifndef ____PBA_PLYPARSER_H____
#define ____PBA_PLYPARSER_H____

#include <string>
using namespace std;
#include "AsciiParser.h"
#include "../pba/base/CollisionSurface.h"
#include "../pba/core/DynamicalState.h"
//#include "SoftBodyState.h"

namespace pba
{
	class ObjParser
	{
	public:

		ObjParser() {}
		~ObjParser() {}

		const bool ParseFile(const string& filename, meshData& _mesh);
		const bool Fill(CollisionSurface& g);
		//const bool Fill(DynamicalState& g);
		//const bool Fill(RigidBodyState& g);
		//const bool Fill(SoftBodyState& g);

	private:
		void processMesh(meshData& mesh, std::vector<Vertex>& vertices);

		meshData mesh;
		AsciiParser parser;
		int nb_vertices;
		int nb_faces;
	};
}

#endif