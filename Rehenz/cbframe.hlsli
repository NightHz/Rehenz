struct CBFrame
{
    matrix view;
    matrix inv_view;
    matrix proj;
    float3 light_intensity;
    float mat_roughness;
    float3 light_direction;
    float _pad1;
    matrix world;
    matrix inv_world;
    float4 color;
    matrix light_view_proj_tex;
};
