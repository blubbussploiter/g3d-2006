#include "lighting.h"

RBX::Lighting* lighting;

const Reflection::PropertyDescriptor<RBX::Lighting, Color3>  RBX::Lighting::prop_spotLight("SpotLightV9", Reflection::Types::TYPE_Color3,&RBX::Lighting::getSpotLight, &RBX::Lighting::setSpotLight, RBX::Lighting::properties);
const Reflection::PropertyDescriptor<RBX::Lighting, Color3>  RBX::Lighting::prop_topAmbient("TopAmbientV9", Reflection::Types::TYPE_Color3,&RBX::Lighting::getTopAmbient, &RBX::Lighting::setTopAmbient, RBX::Lighting::properties);
const Reflection::PropertyDescriptor<RBX::Lighting, Color3>  RBX::Lighting::prop_bottomAmbient("BottomAmbientV9", Reflection::Types::TYPE_Color3,&RBX::Lighting::getBottomAmbient, &RBX::Lighting::setBottomAmbient, RBX::Lighting::properties);

RBX::Lighting* RBX::Lighting::singleton()
{
    if (!lighting)
        lighting = new Lighting();
    return lighting;
}

void RBX::Lighting::begin(RenderDevice* device, float shininess)
{

    LightingParameters lighting;
    Color3 ambientColor, dynamicTop, dynamicBottom;
    Vector3 toLight;
    
    int num = 0;

    lighting = getParameters();
    device->pushState();

    device->enableLighting();

    device->setSpecularCoefficient(0);
    device->setSpecularCoefficient(Color3::white());
    device->setShininess(shininess);

    toLight.x = -0.40000001f;
    toLight.y = -1.0f;
    toLight.z = 0.1f;

    ambientColor = (bottom_ambient + top_ambient) / 2.0f;

    dynamicTop = top_ambient - ambientColor;
    dynamicBottom = bottom_ambient - ambientColor;

    device->setColorClearValue(clear_color);
    device->setAmbientLightColor(ambientColor);

    device->setLight(0, GLight::directional(lighting.lightDirection, spotLight_color * 0.8999999761581421f, 1, 0));
    device->setLight(1, GLight::directional(toLight, dynamicBottom, 0, 1));
    device->setLight(2, GLight::directional(Vector3(lighting.trueMoonPosition.x, 1, lighting.trueMoonPosition.z), dynamicTop, 0, 1));
    
}

void RBX::Lighting::end(RenderDevice* device)
{
    device->disableLighting();
    device->popState();
}

LightingParameters RBX::Lighting::getParameters()
{
    if (!params)
    {
        params = new LightingParameters();

        params->setLatitude(latitude);
        params->setTime(toSeconds(14, 0, PM));
        params->lightDirection = params->trueSunPosition;
    }

    return *params;
}
