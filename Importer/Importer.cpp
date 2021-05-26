#include "Importer.h"
#include <iostream>
#include <fstream>
#include "TempTimer.h"


namespace Importer
{
	std::vector<SceneData> Data::scenes;

	class Reader
	{
		std::ifstream file;
		int currentPosition = 0;
		std::vector<unsigned char> buffer;

		template <typename T>
		void ReadVector(T vec[], int size)
		{
			for (int i = 0; i < size; ++i)
			{
				std::memcpy(&vec[i], buffer.data() + currentPosition, sizeof(T));
				currentPosition += sizeof(T);
			}
		}

		template <typename T>
		void Read(T& type)
		{
			std::memcpy(&type, buffer.data() + currentPosition, sizeof(T));
			currentPosition += sizeof(T);
		}

		void ReadName(char* name)
		{
			std::memcpy(name, buffer.data() + currentPosition, MAX_CHAR);
			currentPosition += MAX_CHAR;
		}

		void ReadTextureFile(char* data, int size)
		{
			std::memcpy(data, buffer.data() + currentPosition, size);
			currentPosition += size;
		}

	public:
		void ReadScene(std::string path)
		{
			TempTimer tempTimer;

			file.open(path, std::ios::in | std::ios::binary);

			if (!file.is_open())
				std::cerr << "COULD NOT OPEN FILE" << std::endl;

			tempTimer.Start();

			buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});
			std::cout << "LOAD FILE DATA: " << tempTimer.DeltaTime() << std::endl;

			SceneData scene;

			int header;
			tempTimer.Start();
			while (currentPosition < buffer.size())
			{
				std::memcpy(&header, buffer.data() + currentPosition, sizeof(int));
				currentPosition += sizeof(int);

				switch ((DataType)header)
				{
				case DataType::MESH:
					ReadMesh(scene);
					break;

				case DataType::MATERIAL:
					ReadMaterial(scene);
					break;

				case DataType::VERTEXBUFFER:
					ReadVertexBuffer(scene);
					break;

				case DataType::SKELETON:
					ReadSkeleton(scene);
				}
			}
			std::cout << "ACTUALLY READ DATA: " << tempTimer.DeltaTime() << std::endl;

			Data::scenes.emplace_back(scene);
			file.close();
		}

	private:
		void ReadMesh(SceneData& scene)
		{
			Mesh mesh;
			
			Read(mesh.ID);
			ReadName(mesh.name);

			Read(mesh.materialCount);
			Read(mesh.attributeCount);

			Read(mesh.parentID);
			Read(mesh.skeletonID);
			Read(mesh.vertexBufferID);

			ReadVector(mesh.translation, 3);
			ReadVector(mesh.rotation, 4);
			ReadVector(mesh.scale, 3);

			for (unsigned int i = 0; i < mesh.materialCount; ++i)
			{
				int ID;
				Read(ID);
				mesh.materialIDs.emplace_back(ID);
			}

			mesh.sceneID = Data::scenes.size();
			scene.meshes.emplace_back(mesh);
		}

		void ReadVertexBuffer(SceneData& scene)
		{
			VertexBuffer vertexBuffer;

			Read(vertexBuffer.ID);
			Read(vertexBuffer.vertexCount);

			for (int i = 0; i < vertexBuffer.vertexCount; ++i)
			{
				Vertex vertex;

				ReadVector(vertex.position, 3);
				ReadVector(vertex.normal, 3);
				ReadVector(vertex.uv, 2);
				ReadVector(vertex.tangent, 3);
				ReadVector(vertex.binormal, 3);
				ReadVector(vertex.weights, 4);
				Read(vertex.boneIDs[0]);
				Read(vertex.boneIDs[1]);
				Read(vertex.boneIDs[2]);
				Read(vertex.boneIDs[3]);
				//ReadVector(vertex.boneIDs, 4);

				vertexBuffer.vertices.emplace_back(vertex);
			}

			vertexBuffer.sceneID = Data::scenes.size();
			scene.vertexBuffers.emplace_back(vertexBuffer);
		}

		void ReadMaterial(SceneData& scene)
		{
			Material material;

			Read(material.ID);
			ReadName(material.name);
			ReadVector(material.diffuse, 3);
			ReadVector(material.ambient, 3);
			ReadVector(material.emissive, 3);
			ReadVector(material.specular, 3);
			Read(material.opacity);
			Read(material.shininess);
			Read(material.reflectivity);

			Read(material.numTextures);
			for (int i = 0; i < material.numTextures; ++i)
			{
				Texture texture;

				Read(texture.ID);
				Read(texture.type);
				Read(texture.width);
				Read(texture.height);
				Read(texture.fileSize);

				texture.data = (char*)malloc(texture.fileSize);
				ReadTextureFile(texture.data, texture.fileSize);

				material.textures.emplace_back(texture);
			}

			material.sceneID = Data::scenes.size();
			scene.materials.emplace_back(material);
		}

		//SKELETON
		void Read(Shape& shape)
		{
			Read(shape.vertexCount);
			for (int i = 0; i < shape.vertexCount; ++i)
			{
				std::array<float, 4> quaternion;
				std::array<float, 3> position;

				for (int j = 0; j < 4; ++j)
					Read(quaternion[j]);

				for (int j = 0; j < 3; ++j)
					Read(position[j]);

				shape.transform.push_back(std::make_pair(quaternion, position));
			}
		}

		void Read(MorphAnimation& morphAnim)
		{
			Read(morphAnim.shapeCount);
			for (int i = 0; i < morphAnim.shapeCount; ++i)
			{
				Shape shape;
				Read(shape);
				morphAnim.shapes.push_back(shape);
			}	
		}

		void Read(Skinning& skinning)
		{
			Read(skinning.weightCount);
			Read(skinning.indexCount);

			for (int i = 0; i < skinning.weightCount; ++i)
			{
				float weight;
				Read(weight);

				skinning.weights.push_back(weight);
			}

			for (int i = 0; i < skinning.indexCount; ++i)
			{
				unsigned int index;
				Read(index);

				skinning.indices.push_back(index);
			}
		}

		void Read(KeyFrame& keyFrame)
		{
			Read(keyFrame.timeStamp);
			ReadVector(keyFrame.matrix, 16);
			//ReadVector(keyFrame.transform.translation, 3);
			//ReadVector(keyFrame.transform.rotation, 4);
			//ReadVector(keyFrame.transform.scale, 3);
		}

		void Read(Animation& animation)
		{
			ReadName(animation.name);
			Read(animation.keyFrameCount);

			for (int i = 0; i < animation.keyFrameCount; ++i)
			{
				KeyFrame keyFrame;
				Read(keyFrame);
				animation.keyFrames.push_back(keyFrame);
			}
		}

		void Read(Joint& joint)
		{
			Read(joint.parentID);
			Read(joint.ID);
			ReadName(joint.name);
			ReadVector(joint.inverseBP, 16);

			Read(joint.animation);
			Read(joint.skinning);
		}

		void ReadSkeleton(SceneData& scene)
		{
			Skeleton skeleton;

			Read(skeleton.ID);
			Read(skeleton.morphAnim);
			Read(skeleton.jointCount);

			for (int i = 0; i < skeleton.jointCount; ++i)
			{
				Joint joint;
				Read(joint);
				skeleton.joints.push_back(joint);
			}

			scene.skeletons.push_back(skeleton);
		}
	};

	void LoadScene(std::string path)
	{
		Reader reader;
		reader.ReadScene(path);
	}

	bool Initialize(ID3D11Device& device)
	{
		TempTimer temp;
		temp.Start();

		for (auto& scene : Data::scenes)
		{
			if (!scene.isInitialized)
			{
				for (auto& material : scene.materials)
				{
					for (auto& texture : material.textures)
					{
						if (!texture.Load(device))
							return false;
					}
				}

				for (int i = 0; i < scene.vertexBuffers.size(); ++i)
				{
					if (!scene.vertexBuffers[i].Initialize(device))
						return false;
				}

				scene.isInitialized = true;
			}
		}

		std::cout << "INIT TIME: " << temp.DeltaTime() << std::endl;
		
		return true;
	}
}