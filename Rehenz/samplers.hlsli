#ifndef _SAMPLERS_HLSLH_
#define _SAMPLERS_HLSLH_

SamplerState samp_linear : register(s0);
SamplerState samp_linear_clamp : register(s1);
SamplerState samp_aniso : register(s2);
SamplerState samp_aniso_clamp : register(s3);
SamplerState samp_point : register(s4);
SamplerState samp_point_clamp : register(s5);

#endif
