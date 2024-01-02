#include "raylib.h"
#include "rcamera.h"

#define RAYGIZMO_IMPLEMENTATION
#include "raygizmo.h"

#define CAMERA_ROT_SPEED 0.003f
#define CAMERA_MOVE_SPEED 0.01f
#define CAMERA_ZOOM_SPEED 1.0f

static void update_camera(Camera3D *camera) {
    bool is_mmb_down = IsMouseButtonDown(2);
    bool is_shift_down = IsKeyDown(KEY_LEFT_SHIFT);
    Vector2 mouse_delta = GetMouseDelta();

    if (is_mmb_down && is_shift_down) {
        CameraMoveRight(camera, -CAMERA_MOVE_SPEED * mouse_delta.x, true);

        Vector3 right = GetCameraRight(camera);
        Vector3 up = Vector3CrossProduct(
            Vector3Subtract(camera->position, camera->target), right
        );
        up = Vector3Scale(
            Vector3Normalize(up), CAMERA_MOVE_SPEED * mouse_delta.y
        );
        camera->position = Vector3Add(camera->position, up);
        camera->target = Vector3Add(camera->target, up);
    } else if (is_mmb_down) {
        CameraYaw(camera, -CAMERA_ROT_SPEED * mouse_delta.x, true);
        CameraPitch(
            camera, CAMERA_ROT_SPEED * mouse_delta.y, true, true, false
        );
    }

    CameraMoveToTarget(camera, -GetMouseWheelMove() * CAMERA_ZOOM_SPEED);
}

int main(void) {
    InitWindow(800, 450, "raygizmo");
    SetTargetFPS(60);

    Camera3D camera;
    camera.fovy = 45.0f;
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.position = (Vector3){5.0f, 5.0f, 5.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    Model model = LoadModelFromMesh(GenMeshTorus(0.3, 1.5, 16.0, 16.0));
    RGizmo gizmo = rgizmo_create();

    while (!WindowShouldClose()) {
        update_camera(&camera);

        BeginDrawing();
        {
            Vector3 position = {
                model.transform.m12, model.transform.m13, model.transform.m14};

            rgizmo_update(&gizmo, camera, position);
            model.transform = MatrixMultiply(
                model.transform, rgizmo_get_tranform(gizmo, position)
            );

            ClearBackground(DARKGRAY);
            rlEnableDepthTest();

            BeginMode3D(camera);
            {
                DrawModel(model, (Vector3){0.0, 0.0, 0.0}, 1.0, PURPLE);

                rlSetLineWidth(1.0);
                DrawGrid(100.0, 1.0);

                rlSetLineWidth(2.0);
                DrawLine3D(
                    (Vector3){-50.0f, 0.0f, 0.0f},
                    (Vector3){50.0f, 0.0f, 0.0f},
                    RED
                );
                DrawLine3D(
                    (Vector3){0.0f, -50.0f, 0.0f},
                    (Vector3){0.0f, 50.0f, 0.0f},
                    GREEN
                );
                DrawLine3D(
                    (Vector3){0.0f, 0.0f, -50.0f},
                    (Vector3){0.0f, 0.0f, 50.0f},
                    DARKBLUE
                );

                rgizmo_draw(gizmo, camera, position);
            }
            EndMode3D();

            DrawRectangle(0, 0, 280, 90, RAYWHITE);
            DrawText("CAMERA:", 5, 5, 20, RED);
            DrawText("    zoom: wheel", 5, 25, 20, RED);
            DrawText("    rotate: mmb", 5, 45, 20, RED);
            DrawText("    translate: shift + mmb", 5, 65, 20, RED);
        }
        EndDrawing();
    }

    rgizmo_unload();
    UnloadModel(model);
    CloseWindow();

    return 0;
}
