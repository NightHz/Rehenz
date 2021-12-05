# Rehenz

some code writed by NightHz

## code style

- file: `render_soft.h`
- var: `int player_speed = 10;`
- func: `int GetPlayerSpeed(int number) { ... }`
- type: `class MovableObj { ... }`
- enum: `enum ObjType { Movable = 0, ... }`
- #define: `#define PI 3.14f`
- namespace: `namespace Rehenz { ... }`

## file description

- `clipper.h`: clip geometry
- `drawer.h`: draw geometry
- `math.h`: math
- `mesh.h`: save model
- `noise_gen.h`: generate a noise, such as Perlin noise
- `rehenz.h`: include all header file
- `render_soft.h`: use cpu to render 3-dim world
- `surface_dx8.h`: use DirectDraw in DirectX8 to create surface which can draw image
- `type.h`: define basic type such as `uint`


## program description

### render_soft

**data**

- `Mesh` saves vertices and triangles
- `Object` saves `Mesh` pointer and transform information
- `Camera` saves camera information, including last buffer image
- The renderer uses smart pointer to save objects such as `Object` and `Camera`, and user should do the same. When user modifies the data, renderer can know immediately.

**pipeline**

- Copy and transform vertices (vertex shader)
- Clipping and back-face culling
- Mapping to screen
- Traverse all triangles and sampling
- Compute color for all sampling points (pixel shader)
- Use z-buffer merge multiple colors

**rasterization rules**

- [-1,1]x[1,-1] map to [0,w]x[0,h], then take the integer part. Specially, (1,-1) map to (w,h) then become (w-1,h-1)
- When rasterize lines, draw all end points
- When rasterize triangles, draw all edges

**comments**

- Two attempts of perspective control interpolation
  1. When screen point is linearly interpolated according to t, it's according to t' that vertex is linearly interpolated, and t' = (w1 * t) / (w1 * t + w2 * (1 - t))
  2. Vertex times 1/w is linearly interpolated according to t which be same with screen point
