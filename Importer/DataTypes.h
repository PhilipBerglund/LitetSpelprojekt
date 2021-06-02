#pragma once

#include <d3d11.h>
#include <vector>
#include <iostream>
#include "Print.h"

#include <wrl.h>
template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define MAX_CHAR 100

enum class DataType
{
	UNKNOWN,
	VERTEXBUFFER,
	MESH,
	MATERIAL,
	GROUP,
	CAMERA,
	LIGHT,
	SKELETON
};

//----------------------- ATTRIBUTE -----------------------
enum class AttributeType { UNKNOWN, VECTOR3, VECTOR4, INTEGER, STRING, FLOAT, BOOLEAN, DOUBLE, ENUM };

struct Value	{	virtual ~Value() {};	};

struct Boolean : Value		{	bool value = false;		};

struct Integer : Value		{	int value = 0;		};

struct Float : Value		{	float value = 0.0f;		};

struct Double : Value		{	double value = 0.0;		};

struct String : Value		{	char value[MAX_CHAR] = "";	};

struct Vector3 : Value		{	double x = 0;	double y = 0;	double z = 0;	};

struct Vector4 : Value		{	double x = 0;	double y = 0;	double z = 0;	double w = 0;	};

struct Attribute
{
	int ID = -1;
	char name[MAX_CHAR] = "";
	bool isAnimateable = false;

	bool hasMaxLimit = false;
	double maxLimit = 0;

	bool hasMinLimit = false;
	double minLimit = 0;

	std::string types[9] = { "Unknown", "Vector3", "Vector4", "Integer", "String", "Float", "Boolean", "Double", "Enum" };
	AttributeType type = AttributeType::UNKNOWN;
	Value* value;

	void PrintAttribute()
	{
		Print(ID, "ID: ");
		Print(name, "Name: ");

		Print(isAnimateable, "Is Animateable: ");

		Print(hasMaxLimit, "Has Max Limit: ");
		Print(maxLimit, "Max Limit: ");

		Print(hasMinLimit, "Has Min Limit: ");
		Print(minLimit, "Min Limit: ");

		Print(types[(int)type], "Attribute Type: ");

		switch (type)
		{
		default: case AttributeType::UNKNOWN:
			break;

		case AttributeType::VECTOR3:
		{
			Vector3* tempVec3 = dynamic_cast<Vector3*>(value);
			float temp3[3] = { tempVec3->x, tempVec3->y, tempVec3->z };
			Print(temp3, 3, "Value: ");
			break;
		}
		case AttributeType::VECTOR4:
		{
			Vector4* tempVec4 = dynamic_cast<Vector4*>(value);
			float temp4[4] = { tempVec4->x, tempVec4->y, tempVec4->z, tempVec4->w };
			Print(temp4, 4, "Value: ");
			break;
		}
		case AttributeType::INTEGER: case AttributeType::ENUM:
		{
			Integer* tempInt = dynamic_cast<Integer*>(value);
			Print(tempInt->value, "Value: ");
			break;
		}
		case AttributeType::STRING:
		{
			String* tempStr = dynamic_cast<String*>(value);
			Print(tempStr->value, "Value: ");
			break;
		}
		case AttributeType::FLOAT:
		{
			Float* tempFloat = dynamic_cast<Float*>(value);
			Print(tempFloat->value, "Value: ");
			break;
		}
		case AttributeType::BOOLEAN:
		{
			Boolean* tempBool = dynamic_cast<Boolean*>(value);
			Print(tempBool->value, "Value: ");
			break;
		}
		case AttributeType::DOUBLE:
		{
			Double* tempDbl = dynamic_cast<Double*>(value);
			Print(tempDbl->value, "Value: ");
			break;
		}
		}

		Print("");
	}
};

//----------------------- NODE -----------------------
struct Node
{
	std::string types[8] = { "Unknown", "VertexBuffer", "Mesh", "Material",
								"Group", "Camera", "Light", "Skeleton" };
	DataType type = DataType::UNKNOWN;
	unsigned int sceneID = 0;

	unsigned int attributeCount = 0;
	std::vector<Attribute> attributes;

	virtual void PrintNode() = 0;
	virtual ~Node() {};
};

//----------------------- MATERIAL / TEXTURE -----------------------
enum class TextureType { UNKNOWN, DIFFUSE_COLOR, NORMAL_MAP, TRANSPARENT_COLOR };

struct Texture
{
	int ID = -1;
	char name[MAX_CHAR] = "";
	TextureType type = TextureType::UNKNOWN;
	int width = 0;
	int height = 0;
	int fileSize = 0;
	char path[MAX_CHAR] = "";
	char* data = nullptr;

	ID3D11ShaderResourceView** Get() 
	{	
		if (view.Get() == nullptr)
			return nullptr;

		return this->view.GetAddressOf(); 
	}

	bool Load(ID3D11Device& device)
	{
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.MiscFlags = 0;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_IMMUTABLE;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = this->data;
		data.SysMemPitch = width * 4;

		if (this->data)
		{
			ID3D11Texture2D* img;
			if FAILED(device.CreateTexture2D(&textureDesc, &data, &img))
			{
				std::cout << "FAILED TO CREATE TEXTURE 2D" << std::endl;
				return false;
			}

			if FAILED(device.CreateShaderResourceView(img, nullptr, &view))
			{
				std::cout << "FAILED TO CREATE SHADER RESOURCE VIEW" << std::endl;
				return false;
			}

			img->Release();
			img = nullptr;
		}

		return true;
	}

	Texture() = default;
private:
	ComPtr<ID3D11ShaderResourceView> view = nullptr;
};

struct Material :public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";
	float diffuse[3] = { 0 };
	float ambient[3] = { 0 };
	float specular[3] = { 0 };

	unsigned int numTextures = 0;
	std::vector<Texture> textures;
	std::string textureTypes[4] = { "Unknown", "Diffuse Color", "Normalmap", "Transparent Color" };

	Material() = default;

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "Type: ");
		Print(ID, "ID: ");
		Print(name, "Name: ");

		Print(diffuse, 3, "Diffuse: ");
		Print(ambient, 3, "Ambient: ");
		Print(specular, 3, "Specular: ");

		Print(numTextures, "Number Of Textures: ");

		if (numTextures == 0)
			return;

		Print("Textures: ");
		for (int i = 0; i < numTextures; ++i)
		{
			Print(textures[i].ID, "ID: ");
			Print(textures[i].name, "Name: ");
			Print(textureTypes[(int)textures[i].type], "Texture Type: ");
			Print(textures[i].width, "Width: ");
			Print(textures[i].height, "Height: ");
			Print(textures[i].fileSize, "File Size: ");
			Print(textures[i].path, "File Path: ");
		}
	}
};

//----------------------- VERTEX / VERTEXBUFFER -----------------------
struct Vertex
{
	float position[3] = { 0, 0, 0 };
	float normal[3] = { 0, 0, 0 };
	float uv[2] = { 0, 0 };
	float tangent[3] = { 0, 0, 0 };
	float binormal[3] = { 0, 0, 0 };
	float weights[4] = { 0, 0, 0, 0 };
	int boneIDs[4] = { -1, -1, -1, -1 };
};

struct VertexBuffer :public Node
{
	int sceneID = 0;
	int ID = -1;
	unsigned int vertexCount = 0;
	std::vector<Vertex> vertices;
	ID3D11Buffer** GetBuffer() { return this->buffer.GetAddressOf(); }

	bool Initialize(ID3D11Device& device)
	{
		D3D11_BUFFER_DESC desc = {};
		desc.ByteWidth = vertexCount * sizeof(Vertex);
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = sizeof(Vertex);

		Vertex* v = vertices.data();

		D3D11_SUBRESOURCE_DATA data = {};
		data.pSysMem = vertices.data();

		if (FAILED(device.CreateBuffer(&desc, &data, &buffer)))
			return false;

		return true;
	}

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "\nType: ");
		Print(ID, "ID: ");
		Print(vertexCount, "Vertex Count: ");
		Print("");

		for (int i = 0; i < vertexCount; ++i)
		{
			Print(i, "Vertex: ");
			Print(vertices[i].position, 3, "Position: ");
			Print(vertices[i].normal, 3, "Normal: ");
			Print(vertices[i].uv, 2, "UVs: ");
			Print(vertices[i].tangent, 3, "Tangent: ");
			Print(vertices[i].binormal, 3, "Binormal: ");
			Print(vertices[i].weights, 4, "Weights: ");
			Print(vertices[i].boneIDs, 4, "BoneIDs: ");
			Print("");
		}
	}
private:
	ComPtr<ID3D11Buffer> buffer;
};

//----------------------- LIGHT -----------------------
enum class LightType { POINT, DIRECTIONAL, SPOT, AREA, VOLUME, UNDEFINED };
struct Light :public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";

	std::string lightTypes[6] = { "Point Light", "Directional Light", "Spot Light", "Area Light", "Volume Light", "Undefined" };
	LightType lightType = LightType::UNDEFINED;

	float translation[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 1.0f, 1.0f, 1.0f };
	float direction[3] = { 0.0f, 0.0f, 0.0f };

	float color[3] = { 0.0f, 0.0f, 0.0f };
	float shadowColor[3] = { 0.0f, 0.0f, 0.0f };

	double intensity = 0.0;
	double innerAngle = 0.0;
	double outerAngle = 0.0;
	double fog = 0.0f;
	double decayStart = 0.0;
	double nearAttenuationStart = 0.0;
	double nearAttenuationEnd = 0.0;
	double farAttenuationStart = 0.0;
	double farAttenuationEnd = 0;

	float leftBarnDoor = 0.0f;
	float rightBarnDoor = 0.0f;
	float topBarnDoor = 0.0f;
	float bottomBarnDoor = 0.0f;

	int decayType = 0;
	int areaLightShape = 0;

	bool castLight = false;
	bool drawVolumetricLight = false;
	bool drawGroundProjection = false;
	bool drawFrontFacingVolumetricLight = false;
	bool enableNearAttenuation = false;
	bool enableFarAttenuation = false;
	bool castShadows = false;
	bool enableBarnDoor = false;

	Light() = default;

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "Type: ");
		Print(ID, "ID: ");
		Print(name, "Name: ");

		Print(lightTypes[(int)lightType], "Light Type: ");
		Print(castLight, "CastLight: ");
		Print(drawVolumetricLight, "DrawVolumericLight: ");
		Print(drawGroundProjection, "DrawGroundProjection: ");
		Print(drawFrontFacingVolumetricLight, "DrawFrontFacingVolumericLight: ");
		Print(enableNearAttenuation, "EnableNearAttenuation: ");
		Print(enableFarAttenuation, "EnableFarAttenuation: ");
		Print(castShadows, "CastShadows: ");
		Print(enableBarnDoor, "EnableBarnDoor: ");
		Print(areaLightShape, "AreaLightShape: ");

		Print(translation, 3, "Translation: ");
		Print(rotation, 3, "Rotation: ");
		Print(scale, 3, "Scale: ");

		Print(direction, 3, "Direction: ");

		Print(color, 3, "Color: ");
		Print(shadowColor, 3, "ShadowColor: ");

		Print(intensity, "Intensity: ");
		Print(innerAngle, "InnerAngle: ");
		Print(outerAngle, "OuterAngle: ");
		Print(fog, "Fog: ");
		Print(decayStart, "DecayStart: ");
		Print(farAttenuationStart, "FarAttenuationStart: ");
		Print(farAttenuationEnd, "FarAttenuationEnd: ");
		Print(nearAttenuationStart, "NearAttenuationStart: ");
		Print(nearAttenuationEnd, "NearAttenuationEnd: ");
		Print(leftBarnDoor, "LeftBarnDoor: ");
		Print(rightBarnDoor, "RightBarnDoor: ");
		Print(topBarnDoor, "TopBarnDoor: ");
		Print(bottomBarnDoor, "BottomBarnDoor: ");
	}
};

//----------------------- CAMERA -----------------------
struct Camera : public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";

	float position[3] = { 0 };
	float target[3] = { 0 };
	float upVector[3] = { 0 };
	float FOV = 0;
	float nearPlaneDistance = 0;
	float farPlaneDistance = 0;

	Camera() = default;

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "Type: ");
		Print(ID, "ID: ");
		Print(name, "Name: ");

		Print(FOV, "FOV: ");
		Print(nearPlaneDistance, "NearPlane: ");
		Print(farPlaneDistance, "FarPlane: ");
		Print(position, 3, "Position: ");
		Print(target, 3, "TargetPos: ");
		Print(upVector, 3, "UpVector: ");
	}
};

//----------------------- ANIMATION -----------------------
struct Keyframe
{
	float timestamp;

	float translation[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 1.0f, 1.0f, 1.0f };

	void PrintKeyframe()
	{
		Print(timestamp, "Timestamp: ");
		Print(translation, 3, "Translation: ");
		Print(rotation, 3, "Rotation: ");
		Print(scale, 3, "Scale: ");
	}
};

struct Animation
{
	char name[MAX_CHAR] = "";
	float duration;
	float framerate;

	std::vector<Keyframe> keyframes;

	void PrintAnimation()
	{
		Print(name, "Name: ");
		Print(duration, "Duration: ");
		Print(framerate, "Framerate: ");

		for (auto& keyframe : keyframes)
			keyframe.PrintKeyframe();
	}
};

struct Shape
{
	char name[MAX_CHAR] = "";
	int vertexCount = 0;
	std::vector<std::pair<float[3], float[3]>> transform;

	void PrintShape()
	{
		Print(name, "\nName: ");
		Print(vertexCount, "Vertex Count: ");

		for (int i = 0; i < vertexCount; ++i)
		{
			Print(i, "\nVertex Index: ");
			Print(transform[i].first, 3, "Position: ");
			Print(transform[i].second, 3, "Normal: ");
		}
	}
};

struct MorphAnimation
{
	char name[MAX_CHAR] = "";
	int shapeCount = 0;
	std::vector<Shape> shapes;

	void PrintMorphAnimation()
	{
		Print(name, "Name: ");
		Print(shapeCount, "Shape Count: ");
		for (auto& shape : shapes)
			shape.PrintShape();
	}
};

//----------------------- SKELETON -----------------------
struct Joint
{
	int ID = -1;
	char name[MAX_CHAR] = "";
	int parentID = -1;
	float inverseBP[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f };

	Animation animation;

	Joint() = default;
	void PrintJoint()
	{
		Print(ID, "\nID: ");
		Print(name, "Name: ");
		Print(parentID, "Parent ID: ");
		Print(inverseBP, 16, "Inverse Bind Pose: ");

		Print("Animation: ");
		animation.PrintAnimation();
	}
};

struct Skeleton : Node
{
	int ID = -1;

	unsigned int jointCount = 0;
	std::vector<Joint> joints;

	Skeleton() = default;

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "Type: ");
		Print(ID, "ID: ");

		Print(jointCount, "\nJoint Count: ");
		for (auto& joint : joints)
			joint.PrintJoint();
	}
};

//----------------------- GROUP -----------------------
struct Group :public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";

	float translation[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 1.0f, 1.0f, 1.0f };

	bool visibility = false;
	int childCount = 0;

	struct Child
	{
		char name[MAX_CHAR] = "";
	};

	std::vector<Child> children;

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "Type: ");
		Print(ID, "ID: ");
		Print(name, "Name: ");

		Print(translation, 3, "Translation: ");
		Print(rotation, 3, "Rotation: ");
		Print(scale, 3, "Scale: ");

		Print(visibility, "Visible: ");
		Print(childCount, "Number Of Children: ");
		Print("Children: ");

		for (auto& child : children)
			Print(child.name, "  ");
	}
};

//----------------------- MESH -----------------------
struct Mesh :public Node
{
	int ID = -1;
	char name[MAX_CHAR] = "";

	unsigned int materialCount = 0;

	int skeletonID = -1;
	int vertexBufferID = -1;

	float translation[3] = { 0.0f, 0.0f, 0.0f };
	float rotation[3] = { 0.0f, 0.0f, 0.0f };
	float scale[3] = { 0.0f, 0.0f, 0.0f };

	std::vector<int> materialIDs;

	MorphAnimation morphAnimation;

	Mesh() = default;

	// Inherited via Node
	virtual void PrintNode() override
	{
		Print(types[(int)type], "Type: ");
		Print(ID, "ID: ");
		Print(name, "Name: ");

		Print(materialCount, "Material Count: ");
		Print(skeletonID, "Skeleton ID: ");
		Print(vertexBufferID, "Vertex Buffer ID: ");
		Print(translation, 3, "Translation: ");
		Print(rotation, 3, "Rotation: ");
		Print(scale, 3, "Scale: ");

		Print("Material IDs: ");
		for (auto& ID : materialIDs)
			Print(ID);

		if (morphAnimation.shapeCount != 0)
		{
			Print("Morph Animation: ");
			morphAnimation.PrintMorphAnimation();
		}
	}
};

//----------------------- SCENE -----------------------
struct SceneData
{
	bool isInitialized = false;
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	std::vector<VertexBuffer> vertexBuffers;
	std::vector<Skeleton> skeletons;
	std::vector<Camera> cameras;
	std::vector<Light> lights;
	std::vector<Group> groups;

	//Print all scene components
	void PrintScene()
	{
		//MESHES
		Print("\n---------------------- MESHES ----------------------");
		for (auto& mesh : meshes)
		{
			Print("\n--------- NEW MESH ---------");
			mesh.PrintNode();
		}
			
		//MATERIALS
		Print("\n---------------------- MATERIALS ----------------------");
		for (auto& material : materials)
		{
			Print("\n--------- NEW MATERIAL ---------");
			material.PrintNode();
		}
			
		//VERTEX BUFFERS
		/*Print("\n---------------------- VERTEX BUFFERS ----------------------");
		for (auto& vertexBuffer : vertexBuffers)
		{
			Print("\n--------- NEW VERTEX BUFFER ---------");
			vertexBuffer.PrintNode();
		}*/
			
		//SKELETONS
		Print("\n---------------------- SKELETONS ----------------------");
		for (auto& skeleton : skeletons)
		{
			Print("\n--------- NEW SKELETON ---------");
			skeleton.PrintNode();
		}	

		//CAMERAS
		Print("\n---------------------- CAMERAS ----------------------");
		for (auto& camera : cameras)
		{
			Print("\n--------- NEW CAMERA ---------");
			camera.PrintNode();
		}
			
		//LIGHTS
		Print("\n---------------------- LIGHTS ----------------------");
		for (auto& light : lights)
		{
			Print("\n--------- NEW LIGHT ---------");
			light.PrintNode();
		}

		//GROUPS
		Print("\n---------------------- GROUPS ----------------------");
		for (auto& group : groups)
		{
			Print("\n--------- NEW GROUP ---------");
			group.PrintNode();
		}	
	}
};