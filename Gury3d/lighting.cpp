#include "lighting.h"

RBX::Lighting* lighting;

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

    device->setLight(0, GLight::directional(lighting.lightDirection, spotLight_color, 1, 0));
    device->setLight(1, GLight::directional(toLight, dynamicBottom, 1, 1));
    device->setLight(2, GLight::directional(Vector3::unitY(), dynamicTop, 1, 1));

    device->setAmbientLightColor(top_ambient);

}

void RBX::Lighting::end(RenderDevice* device)
{
    device->disableLighting();
    device->popState();
}

LightingParameters RBX::Lighting::getParameters()
{
    LightingParameters lighting = LightingParameters();

    lighting.setLatitude(latitude);
    lighting.setTime(toSeconds(14, 0, PM));
    lighting.lightDirection = lighting.trueSunPosition;

    return lighting;
}
