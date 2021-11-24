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

- 基本数据
  - `Object` 存储顶点、三角片元与整体变换信息
  - `Camera` 存储相机信息，包含上次渲染的图像
  - 用户提交 `Object` 与 `Camera` 等对象，使用智能指针共享内存，用户可直接修改物件参数
- 管线