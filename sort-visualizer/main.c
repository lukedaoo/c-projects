#include "pthread.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define DELAY 500

void sleep(int ms) {
    int pause = ms * CLOCKS_PER_SEC / 1000;

    while (clock() < pause) {
        sched_yield();
    }
}

void swap(int *i, int *j) {
    int temp = *i;
    *i = *j;
    *j = temp;
}

int *init_array(int len, int min, int max) {

    int *array = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        array[i] = rand() % (max - min + 1) + min;
    }

    return array;
}

pthread_mutex_t sort_array_read_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t sort_array_write_lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct Array {
    int *data;
    int len;
} Array;

typedef void (*Array_CallbackType)(Array, int);
static void _Array_default_callback(Array array, int index) {}
static Array_CallbackType _Array_at_callback = _Array_default_callback;
static Array_CallbackType _Array_set_callback = _Array_default_callback;

void Array_set_at_callback(Array_CallbackType callback) {
    _Array_at_callback = callback;
}

void Array_set_set_callback(Array_CallbackType callback) {
    _Array_set_callback = callback;
}

int _get_at(Array arr, int i) {
    if (i < arr.len) {
        int val = arr.data[i];
        _Array_at_callback(arr, i);
        return val;
    }

    return -1;
}

int _set_at(Array arr, int i, int value) {
    if (i < arr.len) {
        arr.data[i] = value;
        _Array_set_callback(arr, i);
        return arr.data[i];
    }

    return -1;
}

void swapArr(Array arr, int i, int j) {
    int val_at_i = _get_at(arr, i);
    int val_at_j = _get_at(arr, j);

    if (val_at_i > 0 && val_at_j > 0) {
        _set_at(arr, i, val_at_j);
        _set_at(arr, j, val_at_i);
    }
}

Array _init_array(int len, int min, int max) {
    Array arr;
    arr.len = len;
    arr.data = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++) {
        arr.data[i] = rand() % (max - min + 1) + min;
    }

    return arr;
    // pthread_t sort_thread;
    // pthread_create(&sort_thread, NULL, __sort, NULL);
}

int _find_max(Array arr) {
    int max = -1;
    for (int i = 0; i < arr.len; i++) {
        if (arr.data[i] > max) {
            max = arr.data[i];
        }
    }
    return max;
}

void lock(pthread_mutex_t *mutex) { pthread_mutex_lock(mutex); }

void unlock(pthread_mutex_t *mutex) { pthread_mutex_unlock(mutex); }

void _get_callback(Array arr, int i) {
    lock(&sort_array_read_lock);
    sleep(DELAY);
    unlock(&sort_array_read_lock);
    // sleep(DELAY);
} // printf("Hello from get %d \n", i); }

void _set_callback(Array arr, int i) {
    lock(&sort_array_write_lock);
    sleep(DELAY);
    unlock(&sort_array_write_lock);
} // printf("Hello from set %d \n", i); }

void _sort(Array arr) {
    for (int i = 0; i < arr.len - 1; i++) {
        for (int j = i + 1; j < arr.len; j++) {
            int val_i = _get_at(arr, i);
            int val_j = _get_at(arr, j);
            if (val_i > val_j) {
                swapArr(arr, i, j);
            }
        }
    }
}

void sort(int *array, int len, int *current) {
    int current_index = *current;
    if (current_index >= len) {
        return;
    } else {
        for (int i = current_index + 1; i < len; i++) {
            if (array[i] < array[current_index]) {
                swap(&array[i], &array[current_index]);
            }
        }
    }

    *(current) = current_index + 1;
}

int findMax(int *array, int len) {
    int max = -1;
    for (int i = 0; i < len; i++) {
        if (array[i] >= max) {
            max = array[i];
        }
    }
    return max;
}

void *myThreadFun(void *vargp) {
    Array *arr = (Array *)vargp;
    int len = arr->len;

    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            int val_i = _get_at(*arr, i);
            int val_j = _get_at(*arr, j);
            if (val_i > val_j) {
                swapArr(*arr, i, j);
            }
        }
    }
    printf("Printing GeeksQuiz from Thread \n");
    return NULL;
}

int main(void) {
    const int screenHeight = 450;
    const int MAX_LEN = 1000;
    // int *array = init_array(MAX_LEN, 0, 1000);
    //
    // int len = MAX_LEN;
    // int max = findMax(array, len);
    //
    Array array = _init_array(MAX_LEN, 1, 2000);
    printf("%d", array.data[0]);
    int max = _find_max(array);

    Array_set_at_callback(_get_callback);
    Array_set_set_callback(_set_callback);

    const int rec_width = 1;
    const int screenWidth = MAX_LEN * rec_width;
    InitWindow(screenWidth, screenHeight,
               "raylib [core] example - basic window");
    SetTargetFPS(144); // Set our game to run at 60 frames-per-second
    int current = 0;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, myThreadFun, (void *)&array);

    while (!WindowShouldClose()) {
        // sort(array, len, &current);

        // pthread_join(thread_id, NULL);
        // _sort(array);
        BeginDrawing();

        for (int i = 0; i < MAX_LEN; i++) {
            // pthread_mutex_lock(&sort_array_read_lock);
            int posX = i * rec_width;

            // float height_ratio = ((float)array[i] / max);
            float height_ratio = ((float)array.data[i] / max);
            int rec_height = (int)(height_ratio * screenHeight);

            int posY = screenHeight - rec_height;

            DrawRectangle(posX, posY, rec_width, rec_height, RED);
            // pthread_mutex_unlock(&sort_array_read_lock);
        }
        sleep(10);

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    //

    pthread_mutex_destroy(&sort_array_read_lock);

    return 0;
}
