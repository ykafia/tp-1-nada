#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI (acos(-1))

typedef struct pixel
{
  unsigned char r, g, b;
} pixel;

typedef struct image
{
  int width;
  int height;
  struct pixel *data;
} image;

void print_hexcolor(pixel p){
  char hex[4] = {0,p.r,p.g,p.b};
  printf("0x");
  for (int i = 0; i < 4; i++)  {
    printf("%02x", (unsigned char) hex[i]);
  }
  printf("\n");
}

image *empty_image(int w, int h)
{
  // ça crée un bloc de pixel dans la ram, tout est mis à 0 avec calloc
  pixel *pixels = (pixel *)calloc(w * h, sizeof(pixel));
  // malloc réserve la mémoire sans mettre à 0, du coup c'est bien d'instancier les valeurs au cas où
  image *im = (image *)malloc(sizeof(image));
  im->height = h;
  im->width = w;
  im->data = pixels;

  for (int i = 0; i < w * h; i++)
  {
    pixel blue;
    blue.r = 0;
    blue.g = 0;
    blue.b = 255;
    pixels[i] = blue;
  }

  return im;
}

/* Écrit un entier (4 octets) dans un fichier binaire */
void fwrite_int(int data, FILE *out)
{
}

/* Écrit un octet dans un fichier binaire */
void fwrite_byte(unsigned char data, FILE *out)
{
}

void write_image(char *filename, image *img)
{
  FILE *fd = fopen(filename, "wb");

  fwrite_byte('B', fd); // BMP magic number
  fwrite_byte('M', fd);
  fwrite_int(14 + 40 + 3 * img->width * img->height, fd); // taille du fichier (en octets)
  fwrite_int(0, fd);                                      // application qui a créé l'image
  fwrite_int(14 + 40, fd);                                // taille des en-têtes

  fwrite_int(40, fd);             // taill de cet en-tête
  fwrite_int(img->width, fd);     // largeur de l'image
  fwrite_int(img->height, fd);    // hauteur de l'image
  fwrite_int(1 + (24 << 16), fd); // plans et bits pas pixel
  fwrite_int(0, fd);              // compression
  fwrite_int(0, fd);              // taille de l'image (redondant, 0 fonctionne bien)
  fwrite_int(0, fd);              // résolution horizontale
  fwrite_int(0, fd);              // résolution verticale
  fwrite_int(0, fd);              // nombre de couleurs dans la palette (inutilisé)
  fwrite_int(0, fd);              // nombre de couleurs importantes (inutilisé)

  // TODO : écrire chacun des pixels de l'image
  fclose(fd);
}

struct pixel decode_color(unsigned int color)
{
  struct pixel p = {.r = 0, .g = 0, .b = 0};
  return p;
}

/* Colore le pixel aux coordonnées (x,y) avec la couleur color. */
void draw_pixel(int x, int y, int color, image *img)
{
}

/* Remplit le rectangle dont le coin en bas à gauche est (x1,y1) et le coin en
   haut à droite est (x2,y2) avec la couleur bgcolor. */
void draw_rectangle(int x1, int y1, int x2, int y2, int bgcolor, image *img)
{
}

/* Remplit le disque de centre (xc,yc) et de rayon radius avec la couleur
   color. */
void draw_circle(int xc, int yc, int color, int radius, image *img)
{
}

int max(int a, int b)
{
  return (a < b) ? b : a;
}

/* Dessine une ligne du point (xfrom,yfrom) au point (xto,yto), de couleur color
   et d'épaisseur width. Ignorez l'épaisseur dans un premier temps. */
void draw_line(int xfrom, int yfrom, int xto, int yto, int color, int width, image *img)
{
}

/* Dessine un arc de cercle de centre (xc,yc), de rayon radius, depuis l'angle
   anglefrom jusqu'à angleto, de couleur color et d'épaisseur width. Ignorez
   l'épaisseur dans un premier temps. */
void draw_arc(int xc, int yc, int color, int radius, int anglefrom, int angleto, int width, image *img)
{
}

struct face_description
{
  int skin_color; // la couleur du visage

  int eye_bg_color; // la couleur du fond des yeux
  int eye_fg_color; // la couleur de la pupille

  int eyebrow_color; // la couleur des sourcils
  int eyebrow_width; // l'épaisseur des sourcils

  int nose_color; // la couleur du nez
  int nose_size;  // la taille du nez

  int mouth_color; // la couleur de la bouche
  int mouth_width; // l'épaisseur de la bouche

  int mask;              // présence du masque (0 ou 1)
  int mask_bg_color;     // la couleur du masque
  int mask_border_color; // la couleur de la bordure du masque
  int mask_link_color;   // la couleur des liens du masque
};

/* Dessine un visage. Ignorez le paramètre face_description pour le moment. */
void draw_face(int x, int y, struct face_description *f, image *img)
{
  // face
  draw_circle(x, y, f->skin_color, 40, img);

  // eyes
  draw_circle(x + 20, y + 10, f->eye_bg_color, 10, img);
  draw_circle(x - 20, y + 10, f->eye_bg_color, 10, img);
  draw_arc(x + 20, y + 10, 0x0, 10, 0, 360, 1, img);
  draw_arc(x - 20, y + 10, 0x0, 10, 0, 360, 1, img);

  draw_circle(x + 20, y + 5, f->eye_fg_color, 3, img);
  draw_circle(x - 20, y + 5, f->eye_fg_color, 3, img);

  // eyebrows
  draw_arc(x + 20, y + 10, f->eyebrow_color, 15, 45, 135, f->eyebrow_width, img);
  draw_arc(x - 20, y + 10, f->eyebrow_color, 15, 45, 135, f->eyebrow_width, img);

  // nose
  draw_circle(x, y - 5, f->nose_color, f->nose_size, img);

  // mouth
  draw_arc(x, y - 10, f->mouth_color, 20, 215, 325, f->mouth_width, img);

  // mask
  if (f->mask)
  {
    draw_rectangle(x - 21, y - 26, x + 21, y - 4, f->mask_border_color, img);
    draw_rectangle(x - 20, y - 25, x + 20, y - 5, f->mask_bg_color, img);
    draw_line(x - 20, y - 5, x - 38, y + 5, f->mask_link_color, 2, img);
    draw_line(x - 20, y - 25, x - 35, y - 15, f->mask_link_color, 2, img);
    draw_line(x + 20, y - 5, x + 38, y + 5, f->mask_link_color, 2, img);
    draw_line(x + 20, y - 25, x + 35, y - 15, f->mask_link_color, 2, img);
  }
}

/* Retourne une description de visage par défaut. */
struct face_description *default_face()
{
  struct face_description *f = malloc(sizeof(struct face_description));
  f->skin_color = 0xffff00;
  f->eye_bg_color = 0xffffff;
  f->eye_fg_color = 0x0000ff;
  f->eyebrow_color = 0x00000;
  f->eyebrow_width = 2;
  f->nose_size = 6;
  f->nose_color = 0x00fff0;
  f->mouth_width = 3;
  f->mouth_color = 0xff0000;
  f->mask = 1;
  f->mask_bg_color = 0xffffff;
  f->mask_border_color = 0x000000;
  f->mask_link_color = 0x000000;
  return f;
}

struct face_description *read_face(char *file)
{
  return NULL;
}

void test_decode_color(unsigned int color, unsigned char r, unsigned char g, unsigned char b)
{
  struct pixel p;
  p = decode_color(color);
  printf("decode %x: ", color);
  printf("R ");
  if (p.r == r)
    printf("OK");
  else
    printf("KO (expected %hhx, got %hhx) ", r, p.r);
  printf(" G ");
  if (p.g == g)
    printf("OK");
  else
    printf("KO (expected %hhx, got %hhx) ", g, p.g);
  printf(" B ");
  if (p.r == r)
    printf("OK");
  else
    printf("KO (expected %hhx, got %hhx) ", b, p.b);
  printf("\n");
}

void test_decode_colors()
{
  test_decode_color(0xffaae6, 0xff, 0xaa, 0xe6);
  test_decode_color(0x123456, 0x12, 0x34, 0x56);
}

int main()
{
  printf("Hello !\n");
  image* im = empty_image(3,3);
  for (int i = 0; i < im->height*im->width; i++)
  {
    print_hexcolor(im->data[i]);
  }
  free(im->data);
  free(im);
  return 0;
}
