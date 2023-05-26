#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>

unsigned char g_red = 0;
unsigned char g_green = 0;
unsigned char g_blue = 0;

typedef unsigned char byte;
typedef unsigned short dbyte;
typedef unsigned long pixel;

struct Vector2
{
    int x;
    int y;
};

struct Image
{
    byte* Data;
    int Width;
    int Height;
    dbyte Pitch;
    dbyte _dummy;
};

extern int alpha_blending(byte* Data1, byte* Data2, int *x, int size);

void draw(ALLEGRO_BITMAP* pBitmap, ALLEGRO_BITMAP* pBitmap2, unsigned int x, unsigned int y)
{

    ALLEGRO_LOCKED_REGION* sourceRegion = al_lock_bitmap(pBitmap, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    if (sourceRegion == NULL)
    {
        printf("Cannot lock source bitmap.\n");
        return;
    }
    ALLEGRO_LOCKED_REGION* outputRegion = al_lock_bitmap(pBitmap2, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    if (outputRegion == NULL)
    {
        al_unlock_bitmap(pBitmap2);
        printf("Cannot lock output bitmap.\n");
        return;
    }

    byte* data1 = (byte*)sourceRegion->data;
    byte* data2 = (byte*)outputRegion->data;

    int tab[2] = { x, y };
    printf("x: %d, y: %d, pitch: %d, pixel_size: %d, offset: %d\n", x, y, -sourceRegion->pitch, sourceRegion->pixel_size, (y * (-sourceRegion->pitch) + x * sourceRegion->pixel_size));

    int imSize = al_get_bitmap_width(pBitmap);
    // Assembly function call

    alpha_blending(data1, data2, tab, imSize);

    al_unlock_bitmap(pBitmap);
    al_unlock_bitmap(pBitmap2);
}

int main(int argc, char* argv[])
{
    ALLEGRO_DISPLAY* pDisplay;
    ALLEGRO_BITMAP* pBitmap;
    ALLEGRO_BITMAP* pBitmap2;
    ALLEGRO_EVENT_QUEUE* pQueue;

    if (al_init() != true)
    {
        printf("Initialization error.\n");
        return -1;
    }

    if (al_init_image_addon() != true)
    {
        printf("Initialization (image addon) error.\n");
        return -1;
    }

    if (al_install_keyboard() != true)
    {
        printf("Keyboard initialization error.\n");
        return -1;
    }

    if (al_install_mouse() != true)
    {
        printf("Mouse initialization error.\n");
        return -1;
    }

    pQueue = al_create_event_queue();
    if (pQueue == NULL)
    {
        printf("Event queue initialization error.\n");
        return -1;
    }
    al_register_event_source(pQueue, al_get_keyboard_event_source());
    al_register_event_source(pQueue, al_get_mouse_event_source());

    pDisplay = al_create_display(600, 600);
    if (pDisplay == NULL)
    {
        printf("Display initialization error.\n");
        return -1;
    }

    if (argc < 2){
        printf("za malo arg\n");
    }
    pBitmap = al_load_bitmap(argv[1]);
    if (pBitmap == NULL)
    {
        printf("\"test.bmp\" load error.\n)");
        return -1;
    }
    //pBitmap = al_resize_bitmap(pBitmap, al_get_display_width(pDisplay), al_get_display_height(pDisplay));

    pBitmap2 = al_load_bitmap(argv[2]);
    if (pBitmap2 == NULL)
    {
        printf("\"test2.bmp\" load error.\n");
        return -1;
    }

    al_draw_bitmap(pBitmap, 0, 0, 0);
    al_flip_display();

    while (1)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(pQueue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            g_red = 0;
            g_green = 0;
            g_blue = 0;
            switch (event.keyboard.keycode)
            {
            case ALLEGRO_KEY_R:
                g_red = 0xFF;
                printf("red\n");
                break;
            case ALLEGRO_KEY_G:
                g_green = 0xFF;
                printf("green\n");
                break;
            case ALLEGRO_KEY_B:
            default:
                g_blue = 0xFF;
                printf("blue\n");
                break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            draw(pBitmap, pBitmap2, event.mouse.x, event.mouse.y);

            al_draw_bitmap(pBitmap, 0, 0, 0);
            al_flip_display();
            al_clear_to_color(al_map_rgba(0, 0, 0, 1));
        }
    }

    al_destroy_display(pDisplay);
    al_destroy_bitmap(pBitmap);

    return 0;
}