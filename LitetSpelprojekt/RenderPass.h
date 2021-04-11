#pragma once
class Scene;
class Graphics;

class RenderPass
{
public:
	virtual void Execute(const Scene& scene, Graphics& graphics) = 0;
};