#include "Data/Mesh.h"
#include "Data/Shader.h"
#include <map>
#include <string>
#include <fstream>

std::vector<Args::Mesh> Args::Mesh::meshes;
std::unordered_map<std::string, size_t> Args::Mesh::meshIndices;
std::set<std::string> Args::Mesh::containedModels;

Args::Mesh::Mesh() : indexBufferId(0), vertexBufferId(0), normalBufferId(0), uvBufferId(0), vertices(), normals(), uvs(), indices()
{
	//ctor
}

/**
 * Load reads the obj data into a new mesh using C++ combined with c style coding.
 * The result is an indexed mesh for use with glDrawElements.
 * Expects a obj file with following layout v/vt/vn/f eg
 *
 * For example the obj file for a simple plane describes two triangles, based on
 * four vertices, with 4 uv's all having the same vertex normals (NOT FACE NORMALS!)
 *
 * v 10.000000 0.000000 10.000000              //vertex 1
 * v -10.000000 0.000000 10.000000             //vertex 2
 * v 10.000000 0.000000 -10.000000             //vertex 3
 * v -10.000000 0.000000 -10.000000            //vertex 4
 * vt 0.000000 0.000000                        //uv 1
 * vt 1.000000 0.000000                        //uv 2
 * vt 1.000000 1.000000                        //uv 3
 * vt 0.000000 1.000000                        //uv 4
 * vn 0.000000 1.000000 -0.000000              //normal 1 (normal for each vertex is same)
 * s off
 *
 * Using these vertices, uvs and normals we can construct faces, made up of 3 triplets (vertex, uv, normal)
 * f 2/1/1 1/2/1 3/3/1                         //face 1 (triangle 1)
 * f 4/4/1 2/1/1 3/3/1                         //face 2 (triangle 2)
 *
 * So although this is a good format for blender and other tools reading .obj files, this is
 * not an index mechanism that OpenGL supports out of the box.
 * The reason is that OpenGL supports only one indexbuffer, and the value at a certain point in the indexbuffer, eg 3
 * refers to all three other buffers (v, vt, vn) at once,
 * eg if index[0] = 5, opengl will stream vertexBuffer[5], uvBuffer[5], normalBuffer[5] into the shader.
 *
 * So what we have to do after reading the file with all vertices, is construct unique indexes for
 * all pairs that are described by the faces in the object file, eg if you have
 * f 2/1/1 1/2/1 3/3/1                         //face 1 (triangle 1)
 * f 4/4/1 2/1/1 3/3/1                         //face 2 (triangle 2)
 *
 * v/vt/vn[0] will represent 2/1/1
 * v/vt/vn[1] will represent 1/2/1
 * v/vt/vn[2] will represent 3/3/1
 * v/vt/vn[3] will represent 4/4/1
 *
 * and that are all unique pairs, after which our index buffer can contain:
 *
 * 0,1,2,3,0,2
 *
 * So the basic process is, read ALL data into separate arrays, then use the faces to
 * create unique entries in a new set of arrays and create the indexbuffer to go along with it.
 *
 * Note that loading this mesh isn't cached like we do with texturing, this is an exercise left for the students.
 */
Args::Mesh* Args::Mesh::CreateMesh(const std::string& name, const std::string& filename)
{
	if (containedModels.count(name))//if mesh already in meshes then return that mesh
		return &meshes[meshIndices[name]];

	Debug::Log(DebugInfo, "Loading %s%s...", ModelDir.c_str(), filename.c_str());

	Mesh mesh;

	std::ifstream file(ModelDir + filename, std::ios::in);

	if (file.is_open()) {
		//these three vectors will contains data as taken from the obj file
		//in the order it is encountered in the object file
		std::vector<Vector3> vertices;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;

		//in addition we create a map to store the triplets found under the f(aces) section in the
		//object file and map them to an index for our index buffer (just number them sequentially
		//as we encounter them and store references to the pack
		std::map <FaceIndexTriplet, unsigned int> mappedTriplets;

		std::string line; // to store each line in
		while (getline(file, line)) {

			// c-type string to store cmd read from obj file (cmd is v, vt, vn, f)
			char cmd[10];
			cmd[0] = 0;

			//get the first string in the line of max 10 chars (c-style)
			sscanf_s(line.c_str(), "%10s", cmd, 10);

			//note that although the if statements below seem to imply that we can
			//read these different line types (eg vertex, normal, uv) in any order,
			//this is just convenience coding for us (instead of multiple while loops)
			//we assume the obj file to list ALL v lines first, then ALL vt lines,
			//then ALL vn lines and last but not least ALL f lines last

			//so... start processing lines
			//are we reading a vertex line? straightforward copy into local vertices vector
			if (strcmp(cmd, "v") == 0) {
				Vector3 vertex;
				sscanf_s(line.c_str(), "%10s %f %f %f ", cmd, 10, &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);

				//or are we reading a normal line? straightforward copy into local normal vector
			}
			else if (strcmp(cmd, "vn") == 0) {
				Vector3 normal;
				sscanf_s(line.c_str(), "%10s %f %f %f ", cmd, 10, &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);

				//or are we reading a uv line? straightforward copy into local uv vector
			}
			else if (strcmp(cmd, "vt") == 0) {
				Vector2 uv;
				sscanf_s(line.c_str(), "%10s %f %f ", cmd, 10, &uv.x, &uv.y);
				uvs.push_back(uv);

				//this is where it gets nasty. After having read all vertices, normals and uvs into
				//their own buffer
			}
			else if (strcmp(cmd, "f") == 0) {

				//an f lines looks like
				//f 2/1/1 1/2/1 3/3/1
				//in other words
				//f v1/u1/n1 v2/u2/n2 v3/u3/n3
				//for each triplet like that we need to check whether we already encountered it
				//and update our administration based on that
				IVector3 vertexIndex;
				IVector3 normalIndex;
				IVector3 uvIndex;
				int count = sscanf_s(line.c_str(), "%10s %d/%d/%d %d/%d/%d %d/%d/%d", cmd, 10, &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

				//Have we read exactly 10 elements?
				if (count == 10) {

					//process 3 triplets, one for each vertex (which is first element of the triplet)
					for (int i = 0; i < 3; ++i) {
						//create key out of the triplet and check if we already encountered this before
						FaceIndexTriplet triplet(vertexIndex[i], uvIndex[i], normalIndex[i]);
						std::map<FaceIndexTriplet, unsigned int>::iterator found = mappedTriplets.find(triplet);

						//if iterator points at the end, we haven't found it
						if (found == mappedTriplets.end())
						{
							//so create a new index value, and map our triplet to it
							uint index = (uint)mappedTriplets.size();
							mappedTriplets[triplet] = index;

							//now record this index
							mesh.indices.push_back(index);
							//and store the corresponding vertex/normal/uv values into our own buffers
							//note the -1 is required since all values in the f triplets in the .obj file
							//are 1 based, but our vectors are 0 based
							mesh.vertices.push_back(vertices[vertexIndex[i] - 1]);
							mesh.normals.push_back(normals[normalIndex[i] - 1]);
							mesh.uvs.push_back(uvs[uvIndex[i] - 1]);
						}
						else
						{
							//if the key was already present, get the index value for it
							unsigned int index = found->second;
							//and update our index buffer with it
							mesh.indices.push_back(index);
						}
					}
				}
				else {
					//If we read a different amount, something is wrong
					Debug::Error(DebugInfo, "Error reading obj, needing v,vn,vt. Does the model contain any quads?");
					return nullptr;
				}
			}

		}

		file.close();
		mesh.CalculateTangents();
		mesh.Buffer();
		mesh.name = std::string(name.c_str());
		Debug::Success(DebugInfo, "Mesh loaded and buffered: %i triangles.", mesh.indices.size() / 3);
		meshIndices[name] = meshes.size();
		meshes.push_back(mesh);
		containedModels.insert(name);
		return &meshes[meshIndices[name]];
	}
	else
	{
		Debug::Error(DebugInfo, "Could not read %s%s", ModelDir.c_str(), filename.c_str());
		return nullptr;
	}
}

Args::Mesh* Args::Mesh::GetMesh(const std::string& name)
{
	if (meshIndices.count(name))
		return &meshes[meshIndices[name]];
	return nullptr;
}

void Args::Mesh::Buffer()
{
	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vector3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vector3), &normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vector2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &tangentBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, tangentBufferId);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vector3), &tangents[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Args::Mesh::CalculateTangents()
{
	for (unsigned i = 0; i < normals.size(); i++)
		tangents.push_back(Vector3(0));

	for (unsigned i = 0; i < indices.size(); i += 3)
	{
		Vector3 vtx0 = vertices[indices[i]];
		Vector3 vtx1 = vertices[indices[i + 1]];
		Vector3 vtx2 = vertices[indices[i + 2]];

		Vector2 uv0 = uvs[indices[i]];
		Vector2 uv2 = uvs[indices[i + 2]];
		Vector2 uv1 = uvs[indices[i + 1]];

		Vector3 edge0 = vtx1 - vtx0;
		Vector3 edge1 = vtx2 - vtx0;

		Vector2 deltaUV0 = uv1 - uv0;
		Vector2 deltaUV1 = uv2 - uv0;

		float uvDetFrac = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

		Vector3 tangent;
		tangent.x = uvDetFrac * (deltaUV1.y * edge0.x - deltaUV0.y * edge1.x);
		tangent.y = uvDetFrac * (deltaUV1.y * edge0.y - deltaUV0.y * edge1.y);
		tangent.z = uvDetFrac * (deltaUV1.y * edge0.z - deltaUV0.y * edge1.z);
		tangent = glm::normalize(tangent);

		tangents[indices[i]] += tangent;
		tangents[indices[i + 1]] += tangent;
		tangents[indices[i + 2]] += tangent;
	}

	for (unsigned i = 0; i < tangents.size(); i++)
		tangents[i] = glm::normalize(tangents[i]);
}

void Args::Mesh::Bind(Attribute* pVerticesAttrib, Attribute* pNormalsAttrib, Attribute* pUVsAttrib, Attribute* pTangentsAttrib) const
{
	if (pVerticesAttrib && pVerticesAttrib->IsValid()) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glEnableVertexAttribArray(pVerticesAttrib->GetLocation());
		glVertexAttribPointer(pVerticesAttrib->GetLocation(), 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (pNormalsAttrib && pNormalsAttrib->IsValid()) {
		glBindBuffer(GL_ARRAY_BUFFER, normalBufferId);
		glEnableVertexAttribArray(pNormalsAttrib->GetLocation());
		glVertexAttribPointer(pNormalsAttrib->GetLocation(), 3, GL_FLOAT, GL_TRUE, 0, 0);
	}

	if (pUVsAttrib && pUVsAttrib->IsValid()) {
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
		glEnableVertexAttribArray(pUVsAttrib->GetLocation());
		glVertexAttribPointer(pUVsAttrib->GetLocation(), 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (pTangentsAttrib && pTangentsAttrib->IsValid()) {
		glBindBuffer(GL_ARRAY_BUFFER, tangentBufferId);
		glEnableVertexAttribArray(pTangentsAttrib->GetLocation());
		glVertexAttribPointer(pTangentsAttrib->GetLocation(), 3, GL_FLOAT, GL_TRUE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
}

void Args::Mesh::Draw(unsigned count) const
{
	glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, (GLvoid*)0, (GLsizei)count);
}

void Args::Mesh::Unbind(Attribute* pVerticesAttrib, Attribute* pNormalsAttrib, Attribute* pUVsAttrib, Attribute* pTangentsAttrib)
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//fix for serious performance issue
	if (pTangentsAttrib && pTangentsAttrib->IsValid()) glDisableVertexAttribArray(pTangentsAttrib->GetLocation());
	if (pUVsAttrib && pUVsAttrib->IsValid()) glDisableVertexAttribArray(pUVsAttrib->GetLocation());
	if (pNormalsAttrib && pNormalsAttrib->IsValid()) glDisableVertexAttribArray(pNormalsAttrib->GetLocation());
	if (pVerticesAttrib && pVerticesAttrib->IsValid()) glDisableVertexAttribArray(pVerticesAttrib->GetLocation());
}

void Args::Mesh::DrawDebugInfo(const Matrix4& pModelMatrix, const Matrix4& pViewMatrix, const Matrix4& pProjectionMatrix) {
	//demo of how to render some debug info using the good ol' direct rendering mode...
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(pProjectionMatrix));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(pViewMatrix * pModelMatrix));

	glBegin(GL_LINES);
	//for each index draw the normal starting at the corresponding vertex
	for (size_t i = 0; i < indices.size(); i++) {
		//draw normal for vertex
		if (true) {
			//now get normal end
			Vector3 normal = normals[indices[i]];
			glColor3fv(glm::value_ptr(normal));

			Vector3 normalStart = vertices[indices[i]];
			glVertex3fv(glm::value_ptr(normalStart));
			Vector3 normalEnd = normalStart + normal * 0.2f;
			glVertex3fv(glm::value_ptr(normalEnd));
		}
	}
	glEnd();
}