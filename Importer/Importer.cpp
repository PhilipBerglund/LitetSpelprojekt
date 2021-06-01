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
			file.open(path, std::ios::in | std::ios::binary);

			if (!file.is_open())
				std::cerr << "COULD NOT OPEN FILE" << std::endl;

			buffer = std::vector<unsigned char>(std::istreambuf_iterator<char>(file), {});

			SceneData scene;

			int header;
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
					break;

				case DataType::LIGHT:
					ReadLight(scene);
					break;

				case DataType::CAMERA:
					ReadCamera(scene);
					break;

				case DataType::GROUP:
					ReadGroup(scene);
					break;
				}
			}
	
			Data::scenes.emplace_back(scene);
			file.close();
		}

	private:
		void ReadAttribute(Node& node)
		{
			Attribute attribute;

			Read(attribute.ID);
			ReadName(attribute.name);
			Read(attribute.isAnimateable);
			Read(attribute.hasMaxLimit);
			Read(attribute.maxLimit);
			Read(attribute.hasMinLimit);
			Read(attribute.minLimit);
			Read(attribute.type);
			
			switch (attribute.type)
			{
			default: case AttributeType::UNKNOWN:
				break;

			case AttributeType::VECTOR3:
			{
				float temp[3];
				ReadVector(temp, 3);

				attribute.value = new Vector3();
				dynamic_cast<Vector3*>(attribute.value)->x = temp[0];
				dynamic_cast<Vector3*>(attribute.value)->y = temp[1];
				dynamic_cast<Vector3*>(attribute.value)->z = temp[2];
				break;
			}
			case AttributeType::VECTOR4:
			{
				float temp[4];
				ReadVector(temp, 4);

				attribute.value = new Vector4();
				dynamic_cast<Vector4*>(attribute.value)->x = temp[0];
				dynamic_cast<Vector4*>(attribute.value)->y = temp[1];
				dynamic_cast<Vector4*>(attribute.value)->z = temp[2];
				break;
			}
			case AttributeType::INTEGER: case AttributeType::ENUM:
			{
				int temp;
				Read(temp);
				attribute.value = new Integer();
				dynamic_cast<Integer*>(attribute.value)->value = temp;
				break;
			}
			case AttributeType::STRING:
			{
				String temp;
				ReadName(temp.value);
				attribute.value = new String();
				strcpy_s(dynamic_cast<String*>(attribute.value)->value, MAX_CHAR, temp.value);
				break;
			}
			case AttributeType::FLOAT:
			{
				float temp;
				Read(temp);
				attribute.value = new Float();
				dynamic_cast<Float*>(attribute.value)->value = temp;
				break;
			}
			case AttributeType::BOOLEAN:
			{
				bool temp;
				Read(temp);
				attribute.value = new Boolean();
				dynamic_cast<Boolean*>(attribute.value)->value = temp;
				break;
			}
			case AttributeType::DOUBLE:
			{
				double temp;
				Read(temp);
				attribute.value = new Double();
				dynamic_cast<Double*>(attribute.value)->value = temp;
				break;
			}
			}

			node.attributes.push_back(attribute);
		}

		void ReadMesh(SceneData& scene)
		{
			Mesh mesh;
			
			Read(mesh.ID);
			ReadName(mesh.name);

			Read(mesh.materialCount);

			Read(mesh.skeletonID);
			Read(mesh.vertexBufferID);

			ReadVector(mesh.translation, 3);
			ReadVector(mesh.rotation, 3);
			ReadVector(mesh.scale, 3);

			for (unsigned int i = 0; i < mesh.materialCount; ++i)
			{
				int ID;
				Read(ID);
				mesh.materialIDs.emplace_back(ID);
			}

			//ATTRIBUTES
			//Read(mesh.attributeCount);
			//for (int i = 0; i < mesh.attributeCount; ++i)
			//	ReadAttribute(mesh);

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
				ReadVector(vertex.boneIDs, 4);

				vertexBuffer.vertices.emplace_back(vertex);
			}

			//Read(vertexBuffer.attributeCount);

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
			ReadVector(material.specular, 3);

			Read(material.numTextures);
			for (int i = 0; i < material.numTextures; ++i)
			{
				Texture texture;

				Read(texture.ID);
				ReadName(texture.name);
				Read(texture.type);
				Read(texture.width);
				Read(texture.height);
				Read(texture.fileSize);
				ReadName(texture.path);

				texture.data = (char*)malloc(texture.fileSize);
				ReadTextureFile(texture.data, texture.fileSize);

				material.textures.emplace_back(texture);
			}

			material.sceneID = Data::scenes.size();

			//ATTRIBUTES
			//Read(material.attributeCount);
			//for (int i = 0; i < material.attributeCount; ++i)
			//	ReadAttribute(material);

			scene.materials.emplace_back(material);
		}

		void ReadLight(SceneData& scene)
		{
			Light light;

			Read(light.ID);
			ReadName(light.name);
			Read(light.lightType);

			Read(light.castLight);
			Read(light.drawVolumetricLight);
			Read(light.drawGroundProjection);
			Read(light.drawFrontFacingVolumetricLight);
			Read(light.enableNearAttenuation);
			Read(light.enableFarAttenuation);
			Read(light.castShadows);
			Read(light.enableBarnDoor);
			Read(light.areaLightShape);

			ReadVector(light.translation, 3);
			ReadVector(light.rotation, 3);
			ReadVector(light.scale, 3);

			ReadVector(light.direction, 3);

			ReadVector(light.color, 3);
			ReadVector(light.shadowColor, 3);

			Read(light.intensity);
			Read(light.innerAngle);
			Read(light.outerAngle);
			Read(light.fog);
			Read(light.decayStart);
			Read(light.decayType);

			Read(light.nearAttenuationStart);
			Read(light.nearAttenuationEnd);
			Read(light.farAttenuationStart);
			Read(light.farAttenuationEnd);
			
			Read(light.leftBarnDoor);
			Read(light.rightBarnDoor);
			Read(light.topBarnDoor);
			Read(light.bottomBarnDoor);

			//ATTRIBUTES
			Read(light.attributeCount);
			for (int i = 0; i < light.attributeCount; ++i)
				ReadAttribute(light);

			scene.lights.push_back(light);
		}

		void ReadCamera(SceneData& scene)
		{
			Camera camera;

			Read(camera.ID);
			ReadName(camera.name);
			ReadVector(camera.position, 3);
			ReadVector(camera.target, 3);
			ReadVector(camera.upVector, 3);
			Read(camera.FOV);
			Read(camera.nearPlaneDistance);
			Read(camera.farPlaneDistance);

			//ATTRIBUTES
			Read(camera.attributeCount);
			for (int i = 0; i < camera.attributeCount; ++i)
				ReadAttribute(camera);

			scene.cameras.push_back(camera);
		}

		void ReadShape(Shape& shape)
		{
			ReadName(shape.name);
			Read(shape.vertexCount);

			for (int i = 0; i < shape.vertexCount; ++i)
			{
				std::pair<float[3], float[3]> vertex;

				ReadVector(vertex.first, 3);
				ReadVector(vertex.second, 3);

				shape.transform.push_back(vertex);
			}
		}

		void ReadMorphAnimation(MorphAnimation& morphAnimation)
		{
			ReadName(morphAnimation.name);
			Read(morphAnimation.shapeCount);

			for (int i = 0; i < morphAnimation.shapeCount; ++i)
			{
				Shape shape;
				ReadShape(shape);
				morphAnimation.shapes.push_back(shape);
			}
		}

		void ReadAnimation(Animation& animation)
		{
			ReadName(animation.name);
			Read(animation.duration);
			Read(animation.framerate);
			
			for (int i = 0; i <= animation.duration; ++i)
			{
				Keyframe keyframe;

				Read(keyframe.timestamp);
				ReadVector(keyframe.translation, 3);
				ReadVector(keyframe.rotation, 3);
				ReadVector(keyframe.scale, 3);

				animation.keyframes.push_back(keyframe);
			}
		}

		void ReadJoint(Joint& joint)
		{
			Read(joint.ID);
			ReadName(joint.name);
			Read(joint.parentID);
			ReadVector(joint.inverseBP, 16);
			ReadAnimation(joint.animation);
		}

		void ReadSkeleton(SceneData& scene)
		{
			Skeleton skeleton;

			Read(skeleton.ID);

			ReadMorphAnimation(skeleton.morphAnimation);

			Read(skeleton.jointCount);

			for (int i = 0; i < skeleton.jointCount; ++i)
			{
				Joint joint;
				ReadJoint(joint);
				skeleton.joints.push_back(joint);
			}

			//ATTRIBUTES
			Read(skeleton.attributeCount);
			for (int i = 0; i < skeleton.attributeCount; ++i)
				ReadAttribute(skeleton);

			scene.skeletons.push_back(skeleton);
		}

		void ReadGroup(SceneData& scene)
		{
			Group group;

			Read(group.ID);
			ReadName(group.name);
			
			ReadVector(group.translation, 3);
			ReadVector(group.rotation, 3);
			ReadVector(group.scale, 3);

			Read(group.visibility);
			Read(group.childCount);

			for (int i = 0; i < group.childCount; ++i)
			{
				Group::Child child;
				ReadName(child.name);
				group.children.push_back(child);
			}

			//ATTRIBUTES
			Read(group.attributeCount);
			for (int i = 0; i < group.attributeCount; ++i)
				ReadAttribute(group);

			scene.groups.push_back(group);
		}
	};

	void LoadScene(std::string path)
	{
		Reader reader;
		reader.ReadScene(path);
	}

	bool Initialize(ID3D11Device& device)
	{
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
		
		return true;
	}
}