#include "lighting.h"

RBX::Lighting* lighting;

RBX::Lighting* RBX::Lighting::singleton()
{
    if (!lighting)
        lighting = new Lighting();
    return lighting;
}

void RBX::Lighting::begin(RenderDevice* device, Vector3 Position, float shininess)
{

    LightingParameters lighting;
    lighting.setLatitude(latitude);
    lighting.setTime(toSeconds(14, 0, PM));

    device->pushState();

    device->enableLighting();

    if (shininess > 0)
    {
        device->setSpecularCoefficient(1);
        device->setSpecularCoefficient(Color3::white());
        device->setShininess(shininess);
    }

    device->setColorClearValue(clear_color);
    device->setAmbientLightColor(Color3(1,1,1));

    device->setLight(0, GLight::directional(lighting.lightDirection, bottom_ambient));
    device->setAmbientLightColor(bottom_ambient);
    //device->setAmbientLightColor(bottom_ambient);

}

void RBX::Lighting::end(RenderDevice* device)
{
     device->setLight(0, 0);
    // device->setLight(1, 0);
    // device->setLight(2, 0);
    device->disableLighting();
    device->popState();
}
