#pragma once
#include <Args-Core.h>
#include "Data/LightData.h"

namespace Args
{
	enum LightType
	{
		POINT = 0,
		DIRECTIONAL = 1,
		SPOT = 2
	};

	struct Light : public Component<Light>
	{
	public:
		Light(Entity* entity);

		const LightData& GetLightData();
		void SetType(int type);
		void SetAttenuation(float attenuation);
		void SetIntensity(float intensity);
		void SetColour(Vector3 colour);
		void SetAngle(float angle);
		void SetFalloffPower(float power);

		// Inherited via Component
		virtual std::string ObjectType() override;
		virtual bool SetData(const std::string& name, const std::string& value) override;
		virtual bool GetData(const std::string& name, std::string& value) override;
	protected:
		union
		{
			LightData lightData;

			struct
			{
				int type;
				float attenuation;
				float intensity;
				float meta;
				Vector3 direction;
				float falloff;
				Vector3 position;
				float angle;
				Vector3 colour;
				float meta2;
			};
		};
	};
}