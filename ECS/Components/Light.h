// *********************************************************************************
// Filename:     Light.h
// Description:  contains different data structures for lighting computations
//
// Created:      29.03.24 (moved into ECS at 22.08.24)
// *********************************************************************************
#pragma once

#include "../Common/ECSTypes.h"
#include <Types.h>
#include <cvector.h>
#include <DirectXMath.h>


namespace ECS
{

// *********************************************************************************
//        STRUCTURES TO REPRESENT TYPES OF LIGHTS (DIRECTIONAL, POINT, SPOT)
// *********************************************************************************

enum LightType
{
    DIRECTIONAL,
    POINT,
    SPOT,
};

// Common description for members:
// 1. ambient:     the amount of ambient light emitted by the light source
// 2. diffuse:     the amount of diffuse light emitted by the light source
// 3. specular:    the amount of specular light emitted by the light source
// 4. direction:   the direction of the light
// 5. position:    the position of the light
// 6. range:       the range of the light. A point whose distance from the light source is 
//                 greater than the range is not lit.
// 7. attenuation: stores the three attenuation constant in the format (a0, a1, a2) that
//                 control how light intensity falls off with distance
// 8. spot:        the exponent used in the spotlight calculation to control the spotlight cone

enum LightProp
{
    AMBIENT,
    DIFFUSE,
    SPECULAR,
    POSITION,
    DIRECTION,
    RANGE,
    ATTENUATION,
    SPOT_EXP
};

///////////////////////////////////////////////////////////

struct DirLight
{
    DirLight() {}

    DirLight(const DirLight& rhs)
    {
        *this = rhs;
    }

    DirLight& operator=(const DirLight& rhs)
    {
        if (this == &rhs)
            return *this;

        ambient   = rhs.ambient;
        diffuse   = rhs.diffuse;
        specular  = rhs.specular;

        return *this;
    }

    DirLight(
        const DirectX::XMFLOAT4& ambient,
        const DirectX::XMFLOAT4& diffuse,
        const DirectX::XMFLOAT4& specular)
        :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular)
        {}
    

    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;
};

///////////////////////////////////////////////////////////

struct PointLight
{
    PointLight() :
        ambient{ NAN, NAN, NAN, NAN },
        diffuse{ NAN, NAN, NAN, NAN },
        specular{ NAN, NAN, NAN, NAN },
        range{ NAN },
        att{ NAN, NAN, NAN }
    {}

    PointLight(const PointLight& rhs)
    {
        *this = rhs;
    }

    PointLight& operator=(const PointLight& rhs)
    {
        if (this == (&rhs))
            return *this;

        ambient  = rhs.ambient;
        diffuse  = rhs.diffuse;
        specular = rhs.specular;
        range    = rhs.range;
        att      = rhs.att;

        return *this;
    }

    PointLight(
        const XMFLOAT4& ambient,
        const XMFLOAT4& diffuse,
        const XMFLOAT4& specular,
        const float range,
        const XMFLOAT3& attenuation)
        :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        range(range)
    {
        att.x = (attenuation.x > 0.01f) ? attenuation.x : 0.01f;
        att.y = (attenuation.y > 0.01f) ? attenuation.y : 0.01f;
        att.z = (attenuation.z > 0.01f) ? attenuation.z : 0.01f;
    }


    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;

    // packed into 4D vector: (att(A0,A1,A2), range)
    DirectX::XMFLOAT3 att;            // attenuation
    float range;
};

///////////////////////////////////////////////////////////

struct SpotLight
{
    SpotLight() :
        ambient{ NAN, NAN, NAN, NAN },
        diffuse{ NAN, NAN, NAN, NAN },
        specular{ NAN, NAN, NAN, NAN },
        range{ NAN },
        spot{ NAN },
        att{ NAN, NAN, NAN }
    {}


    SpotLight(
        const XMFLOAT4& ambient,
        const XMFLOAT4& diffuse,
        const XMFLOAT4& specular,
        const float range,
        const float spot,
        const XMFLOAT3& attenuation)
        :
        ambient(ambient),
        diffuse(diffuse),
        specular(specular),
        range(range),
        spot(spot)
    {
        att.x = (attenuation.x > 0.01f) ? attenuation.x : 0.01f;
        att.y = (attenuation.y > 0.01f) ? attenuation.y : 0.01f;
        att.z = (attenuation.z > 0.01f) ? attenuation.z : 0.01f;
    }


    DirectX::XMFLOAT4 ambient;
    DirectX::XMFLOAT4 diffuse;
    DirectX::XMFLOAT4 specular;

    // packed into 4D vector: (attenuation(A0,A1,A2), range)
    DirectX::XMFLOAT3 att;
    float range;

    // spot exponent: light intensity fallof (for control the spotlight cone)
    float spot;
};


// *********************************************************************************
//        STRUCTURES TO REPRESENT CONTAINERS FOR INIT PARAMS FOR LIGHT SOURCES
// *********************************************************************************

struct DirLightsInitParams
{
    cvector<DirLight> data;
};

struct PointLightsInitParams
{
    cvector<PointLight> data;
};

struct SpotLightsInitParams
{
    cvector<SpotLight> data;
};


// *********************************************************************************
//           STRUCTURES TO REPRESENT CONTAINERS FOR LIGHT SOURCES
// *********************************************************************************

__declspec(align(16)) struct DirLights
{
    cvector<EntityID> ids;
    cvector<DirLight> data;
};

__declspec(align(16)) struct PointLights
{
    cvector<EntityID> ids;
    cvector<PointLight> data;
};

__declspec(align(16)) struct SpotLights
{
    cvector<EntityID> ids;
    cvector<SpotLight> data;
};

// *********************************************************************************

__declspec(align(16)) struct PosAndRange
{
    DirectX::XMFLOAT3 position{0,0,0};
    float range = 0.0f;
};


// *********************************************************************************
//                              COMPONENT
// *********************************************************************************

struct Light
{
    cvector<EntityID>   ids;
    cvector<LightType>  types;
    cvector<bool>       isActive;
    DirLights           dirLights;
    PointLights         pointLights;
    SpotLights          spotLights;
};


} // namespace ECS
