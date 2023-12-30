# RAYGIZMO
`raygizmo` was designed as an auxiliar module for [raylib](https://github.com/raysan5/raylib) to create simple interactive gizmo gadget to perform basic object transformation (rotation and translation).

![thumbnail](./thumbnail.gif)


*NOTE: raygizmo is a single-file header-only library (despite its internal dependency on raylib), so, functions definition AND implementation reside in the same file `raygizmo.h`, when including `raygizmo.h` in a module, `RAYGIZMO_IMPLEMENTATION` must be previously defined to include the implementation part of `raygizmo.h` BUT only in one compilation unit, other modules could also include `raygizmo.h` but `RAYGIZMO_IMPLEMENTATION` must not be defined again.*

*NOTE: Current raygizmo implementation is intended to work with [raylib-5.0](https://github.com/raysan5/raylib/releases/tag/5.0) and PLATFORM_DESKTOP*


## Example
In a simplest case gizmo can be used like this:
```c
#define RAYGIZMO_IMPLEMENTATION
#include "../src/raygizmo.h"
#undef RAYGIZMO_IMPLEMENTATION

int main(void) {
    InitWindow(800, 600, "Gizmo");

    Camera3D camera;
    camera.fovy = 45.0f;
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.position = (Vector3){5.0f, 5.0f, 5.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    Model model = LoadModelFromMesh(GenMeshTorus(0.3, 1.5, 16.0, 16.0));

    loadGizmo();

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);

            // Draw the model
            BeginMode3D(camera);
                DrawModel(model, (Vector3){0.0, 0.0, 0.0}, 1.0, PURPLE);
            EndMode3D();

            // Immediately update and draw gizmo
            Vector3 position = {
                model.transform.m12, model.transform.m13, model.transform.m14};
            Matrix transform = updateAndDrawGizmo(camera, position);

            // Apply gizmo-produced transformation to the model
            model.transform = MatrixMultiply(model.transform, transform);
        EndDrawing();
    }

    unloadGizmo();
    UnloadModel(model);
    CloseWindow();

    return 0;
}
```


Interactive example could be built and run like this:
```bash
cd examples \
&& gcc -o ./raygizmo ./raygizmo.c -lraylib -lm -lpthread -ldl \
&& ./raygizmo
```
