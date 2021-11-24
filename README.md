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

- `math.h`: math
- `noise_gen.h`: generate a noise, such as Perlin noise
- `rehenz.h`: include all header file
- `render_soft.h`: use cpu to render 3-dim world
- `surface_dx8.h`: use DirectDraw in DirectX8 to create surface which can draw image


## program design

### render_soft

**data**

- `Mesh` saves vertices and triangles
- `Object` saves `Mesh` pointer and transform information
- `Camera` saves camera information, including last buffer image
- Rendering engine uses smart pointer to save objects such as `Object` and `Camera`, and user should do the same. When user modifies the data, rendering engine can know immediately.

**pipeline**

- Copy and transform all objects (vertex shader)
- Clipping and back-face culling
- Mapping to screen
- Traverse all triangles and compute color for all sampling points (pixel shader)
- Use z-buffer merge multiple colors
