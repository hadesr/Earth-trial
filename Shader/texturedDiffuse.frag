#version 330 core

in vec4 FragPos;
in vec2 TexCoords;
in vec4 Normal;
in mat3 TBN;

uniform vec4 EyePosW;   // Eye position in world space.
uniform vec4 LightPosW; // Light's position in world space.
uniform vec4 LightColor; // Light's diffuse and specular contribution.

uniform vec4 MaterialEmissive;
uniform vec4 MaterialDiffuse;
uniform vec4 MaterialSpecular;
uniform float MaterialShininess;

uniform vec4 Ambient; // Global ambient contribution.

uniform sampler2D diffuseSampler;
uniform sampler2D normalSampler;
//uniform sampler2D specularSampler;

uniform bool np;


layout (location=0) out vec4 out_color;

void main()
{
    // Compute the emissive term.
    vec4 Emissive = MaterialEmissive;

    // Compute the diffuse term.

    vec4 N = normalize( Normal );

    if(np){
    	vec3 Nm = texture(normalSampler, TexCoords).rgb;
    	Nm = Nm * 2.0 - 1.0;
	Nm = normalize(TBN * Nm);
    	N = vec4(Nm,0);
    }

    vec4 L = normalize( LightPosW - FragPos );
    float NdotL = max( dot( N, L ), 0 );
    vec4 Diffuse =  NdotL * LightColor * MaterialDiffuse;
    
    // Compute the specular term.
    vec4 V = normalize( EyePosW - FragPos );
    vec4 H = normalize( L + V );
    vec4 R = reflect( -L, N );
    float RdotV = max( dot( R, V ), 0 );
    float NdotH = max( dot( N, H ), 0 );
    vec4 Specular = pow( RdotV, MaterialShininess ) * LightColor * MaterialSpecular;
    
    out_color = ( Emissive + Ambient + Diffuse + Specular ) * texture( diffuseSampler, TexCoords );
}
