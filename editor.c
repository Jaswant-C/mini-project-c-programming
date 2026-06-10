#include "shapes.h"
#include "canvas.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_SHAPES 100

/* Safe integer reader that prevents crashes, buffer overflows, or infinite loops */
static int get_int_input(const char *prompt, int min_val, int max_val) {
    int val;
    char buffer[100];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }
        
        char *endptr;
        val = (int)strtol(buffer, &endptr, 10);
        
        /* If no conversion was performed, or characters remain (excluding newlines/returns) */
        if (endptr == buffer || (*endptr != '\n' && *endptr != '\0' && *endptr != '\r')) {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        if (val < min_val || val > max_val) {
            printf("Input out of range. Must be between %d and %d.\n", min_val, max_val);
            continue;
        }
        return val;
    }
}

/* Outputs a formatted list of all active shapes and their coordinates */
static void list_shapes(const Shape *shapes, int count) {
    printf("\n=== Active Shapes (%d/%d) ===\n", count, MAX_SHAPES);
    if (count == 0) {
        printf("  No shapes drawn yet. Canvas is empty.\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        printf("  ID: %d | ", shapes[i].id);
        switch (shapes[i].type) {
            case SHAPE_LINE:
                printf("Line from (%d, %d) to (%d, %d)\n",
                       shapes[i].data.line.x1, shapes[i].data.line.y1,
                       shapes[i].data.line.x2, shapes[i].data.line.y2);
                break;
            case SHAPE_RECTANGLE:
                printf("Rectangle: top-left (%d, %d), width %d, height %d\n",
                       shapes[i].data.rect.x, shapes[i].data.rect.y,
                       shapes[i].data.rect.w, shapes[i].data.rect.h);
                break;
            case SHAPE_CIRCLE:
                printf("Circle: center (%d, %d), radius %d\n",
                       shapes[i].data.circle.xc, shapes[i].data.circle.yc,
                       shapes[i].data.circle.r);
                break;
            case SHAPE_TRIANGLE:
                printf("Triangle: vertices (%d, %d), (%d, %d), (%d, %d)\n",
                       shapes[i].data.triangle.x1, shapes[i].data.triangle.y1,
                       shapes[i].data.triangle.x2, shapes[i].data.triangle.y2,
                       shapes[i].data.triangle.x3, shapes[i].data.triangle.y3);
                break;
        }
    }
}

/* Menu logic to create a new shape and add it to the active list */
static void add_shape_menu(Shape *shapes, int *count, int *next_id) {
    if (*count >= MAX_SHAPES) {
        printf("\nError: Maximum shape limit reached (%d). Delete some shapes first.\n", MAX_SHAPES);
        return;
    }

    printf("\n--- Add a New Shape ---\n");
    printf("1. Line\n");
    printf("2. Rectangle\n");
    printf("3. Circle\n");
    printf("4. Triangle\n");
    printf("5. Cancel\n");
    int choice = get_int_input("Choose shape type: ", 1, 5);
    if (choice == 5) {
        return;
    }

    Shape new_shape;
    new_shape.id = *next_id;
    new_shape.type = (ShapeType)choice;

    switch (new_shape.type) {
        case SHAPE_LINE:
            printf("\nEnter line coordinates:\n");
            new_shape.data.line.x1 = get_int_input("  Start X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.line.y1 = get_int_input("  Start Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            new_shape.data.line.x2 = get_int_input("  End X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.line.y2 = get_int_input("  End Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
        case SHAPE_RECTANGLE:
            printf("\nEnter rectangle details:\n");
            new_shape.data.rect.x = get_int_input("  Top-Left X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.rect.y = get_int_input("  Top-Left Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            new_shape.data.rect.w = get_int_input("  Width (1-60): ", 1, CANVAS_WIDTH);
            new_shape.data.rect.h = get_int_input("  Height (1-20): ", 1, CANVAS_HEIGHT);
            break;
        case SHAPE_CIRCLE:
            printf("\nEnter circle details:\n");
            new_shape.data.circle.xc = get_int_input("  Center X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.circle.yc = get_int_input("  Center Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            new_shape.data.circle.r = get_int_input("  Radius (0-30): ", 0, 30);
            break;
        case SHAPE_TRIANGLE:
            printf("\nEnter triangle vertices:\n");
            new_shape.data.triangle.x1 = get_int_input("  Vertex 1 X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.triangle.y1 = get_int_input("  Vertex 1 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            new_shape.data.triangle.x2 = get_int_input("  Vertex 2 X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.triangle.y2 = get_int_input("  Vertex 2 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            new_shape.data.triangle.x3 = get_int_input("  Vertex 3 X (0-59): ", 0, CANVAS_WIDTH - 1);
            new_shape.data.triangle.y3 = get_int_input("  Vertex 3 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
    }

    shapes[*count] = new_shape;
    (*count)++;
    (*next_id)++;
    printf("\nShape added successfully with ID: %d\n", new_shape.id);
}

/* Menu logic to delete a shape by input ID */
static void delete_shape_menu(Shape *shapes, int *count) {
    if (*count == 0) {
        printf("\nNo shapes to delete.\n");
        return;
    }

    printf("\n--- Delete a Shape ---\n");
    int target_id = get_int_input("Enter the ID of the shape to delete: ", 1, 9999);

    int found_idx = -1;
    for (int i = 0; i < *count; i++) {
        if (shapes[i].id == target_id) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Shape with ID %d not found.\n", target_id);
        return;
    }

    /* Shift remaining elements to close the gap */
    for (int i = found_idx; i < *count - 1; i++) {
        shapes[i] = shapes[i + 1];
    }
    (*count)--;
    printf("Shape ID %d deleted successfully.\n", target_id);
}

/* Menu logic to modify parameters of an existing shape by ID */
static void modify_shape_menu(Shape *shapes, int count) {
    if (count == 0) {
        printf("\nNo shapes to modify.\n");
        return;
    }

    printf("\n--- Modify a Shape ---\n");
    int target_id = get_int_input("Enter the ID of the shape to modify: ", 1, 9999);

    int found_idx = -1;
    for (int i = 0; i < count; i++) {
        if (shapes[i].id == target_id) {
            found_idx = i;
            break;
        }
    }

    if (found_idx == -1) {
        printf("Shape with ID %d not found.\n", target_id);
        return;
    }

    Shape *sh = &shapes[found_idx];
    printf("\nModifying shape ID %d:\n", sh->id);

    switch (sh->type) {
        case SHAPE_LINE:
            printf("Current coordinates: (%d, %d) to (%d, %d)\n",
                   sh->data.line.x1, sh->data.line.y1, sh->data.line.x2, sh->data.line.y2);
            sh->data.line.x1 = get_int_input("  New Start X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.line.y1 = get_int_input("  New Start Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            sh->data.line.x2 = get_int_input("  New End X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.line.y2 = get_int_input("  New End Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
        case SHAPE_RECTANGLE:
            printf("Current details: top-left (%d, %d), width %d, height %d\n",
                   sh->data.rect.x, sh->data.rect.y, sh->data.rect.w, sh->data.rect.h);
            sh->data.rect.x = get_int_input("  New Top-Left X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.rect.y = get_int_input("  New Top-Left Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            sh->data.rect.w = get_int_input("  New Width (1-60): ", 1, CANVAS_WIDTH);
            sh->data.rect.h = get_int_input("  New Height (1-20): ", 1, CANVAS_HEIGHT);
            break;
        case SHAPE_CIRCLE:
            printf("Current details: center (%d, %d), radius %d\n",
                   sh->data.circle.xc, sh->data.circle.yc, sh->data.circle.r);
            sh->data.circle.xc = get_int_input("  New Center X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.circle.yc = get_int_input("  New Center Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            sh->data.circle.r = get_int_input("  New Radius (0-30): ", 0, 30);
            break;
        case SHAPE_TRIANGLE:
            printf("Current vertices: (%d, %d), (%d, %d), (%d, %d)\n",
                   sh->data.triangle.x1, sh->data.triangle.y1,
                   sh->data.triangle.x2, sh->data.triangle.y2,
                   sh->data.triangle.x3, sh->data.triangle.y3);
            sh->data.triangle.x1 = get_int_input("  New Vertex 1 X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.triangle.y1 = get_int_input("  New Vertex 1 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            sh->data.triangle.x2 = get_int_input("  New Vertex 2 X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.triangle.y2 = get_int_input("  New Vertex 2 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            sh->data.triangle.x3 = get_int_input("  New Vertex 3 X (0-59): ", 0, CANVAS_WIDTH - 1);
            sh->data.triangle.y3 = get_int_input("  New Vertex 3 Y (0-19): ", 0, CANVAS_HEIGHT - 1);
            break;
    }
    printf("Shape ID %d modified successfully.\n", target_id);
}

/* Main driver function for the Graphics Editor */
int main(void) {
    char canvas[CANVAS_WIDTH * CANVAS_HEIGHT];
    Shape shapes[MAX_SHAPES];
    int shape_count = 0;
    int next_id = 1;

    printf("====================================================\n");
    printf("         WELCOME TO THE 2D GRAPHICS EDITOR          \n");
    printf("====================================================\n");

    while (1) {
        /* Redraw shapes list on canvas dynamic representation */
        render_canvas(canvas, CANVAS_WIDTH, CANVAS_HEIGHT, shapes, shape_count);

        /* Print coordinate grid and pixels */
        display_canvas(canvas, CANVAS_WIDTH, CANVAS_HEIGHT);

        /* Print active shape listings */
        list_shapes(shapes, shape_count);

        /* Interactive Choice Selection */
        printf("\n====================================================\n");
        printf("                    ACTION MENU                     \n");
        printf("====================================================\n");
        printf("1. Add Shape\n");
        printf("2. Delete Shape\n");
        printf("3. Modify Shape\n");
        printf("4. Clear Canvas (Delete All)\n");
        printf("5. Help & Controls Info\n");
        printf("6. Exit Program\n");
        printf("====================================================\n");

        int choice = get_int_input("Choose action (1-6): ", 1, 6);

        if (choice == 6) {
            printf("\nExiting 2D Graphics Editor. Thank you for drawing!\n");
            break;
        }

        switch (choice) {
            case 1:
                add_shape_menu(shapes, &shape_count, &next_id);
                break;
            case 2:
                delete_shape_menu(shapes, &shape_count);
                break;
            case 3:
                modify_shape_menu(shapes, shape_count);
                break;
            case 4:
                shape_count = 0;
                printf("\nCanvas cleared: all shapes deleted.\n");
                break;
            case 5:
                printf("\n--- HELP & CONTROLS ---\n");
                printf("- Coordinate grid range: X axis is 0 to %d.\n", CANVAS_WIDTH - 1);
                printf("                         Y axis is 0 to %d.\n", CANVAS_HEIGHT - 1);
                printf("- (0,0) is located at the top-left corner.\n");
                printf("- Overlapping shapes are rendered in chronological order (layers on top).\n");
                printf("- Input validation prevents crashes: entering letters defaults to a correction prompt.\n");
                printf("\nPress [Enter] to return to the canvas editor...");
                
                /* Wait for user response to proceed */
                char pause_buf[100];
                fgets(pause_buf, sizeof(pause_buf), stdin);
                break;
        }
    }
    return 0;
}
