#include "Components/Light.h"

Args::Light::Light(Entity* entity) : Component<Light>(entity), lightData()
{
	type = LightType::POINT;
	attenuation = 10;
	direction = Vector3();
	position = Vector3();
	colour = Vector3(1, 1, 1);
	intensity = 1;
	angle = radians(180.f);
	falloff = pi<float>();
}

const Args::LightData& Args::Light::GetLightData()
{
	Transform* transform = owner->GetComponent<Transform>();

	if (type != LightType::DIRECTIONAL)
		position = transform->GetWorldPosition();
	else
		attenuation = FLT_MAX;

	if (type != LightType::POINT)
		direction = transform->GetWorldForward();

	return lightData;
}

void Args::Light::SetType(int type)
{
	this->type = type;
}

void Args::Light::SetAttenuation(float attenuation)
{
	this->attenuation = attenuation;
}

void Args::Light::SetIntensity(float intensity)
{
	this->intensity = intensity;
}

void Args::Light::SetColour(Vector3 colour)
{
	this->colour = colour;
}

void Args::Light::SetAngle(float angle)
{
	this->angle = angle;
}

void Args::Light::SetFalloffPower(float power)
{
	falloff = power;
}

std::string Args::Light::ObjectType()
{
	return std::string();
}

bool Args::Light::SetData(const std::string& name, const std::string& value)
{
	return false;
}

bool Args::Light::GetData(const std::string& name, std::string& value)
{
	return false;
}
